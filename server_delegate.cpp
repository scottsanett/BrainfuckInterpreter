#include "server_delegate.hpp"

namespace scott {
    namespace server {

        // filling all the information from the cache
        Delegate::Delegate() {
            std::lock_guard<std::mutex> guard(mutex);
            std::ifstream users("./.userinfo");
            for(std::string line; std::getline(users, line); ) {
                std::istringstream iss(line);
                std::string username, password;
                iss >> username >> password;
                user_info.insert(std::make_pair(username, password));
                std::ifstream history("./.history_" + username);
                code_history.insert(std::make_pair(username, str_map()));
                auto this_user = code_history.find(username);
                for (std::string code; std::getline(history, code); ) {
                    std::istringstream iss_code(line);
                    std::string key, value, temp;
                    iss_code >> key;
                    while (iss_code >> temp) { value += temp + " "; }
                    this_user->second.insert(std::make_pair(key, value));
                }
                history.close();
            }
            users.close();
        }


    Delegate::~Delegate() {
        std::lock_guard<std::mutex> guard(mutex);
        if (user_info.size() != 0) {
            std::ofstream users("./.userinfo");
            for (auto && user : user_info) {
                auto username = user.first;
                std::string line = username + " " + user.second;
                users << line << std::endl;
                auto this_history = code_history.find(username); // all entries for a specific user
                if (this_history == code_history.end()) { continue; } // no entry for the user exists
                else if (this_history->second.empty()) { continue; } // the user has no history entry
                std::ofstream history("./.history_" + username);
                for (auto && entry : this_history->second) {
                    std::string line = entry.first + " " + entry.second;
                    history << line << std::endl;
                }
                history.close();
            }
            users.close();
        }
    }

    void Delegate::parse_request(const std::string &request, std::string &result) {
        std::string header;
        std::istringstream iss(request);
        iss >> header;
        try {
            // only the code is saved, the input is ignored.
            if (header == requests.save_new_file) {
                std::string filename, code;
                iss >> filename >> code;
                std::cout << "hello";
                save(filename, code);
            }
            else if (header == requests.save_file) {
                std::string code;
                iss >> code;
                save(code);
            }
            else if (header == requests.run) {
                std::string str, code;
                while (iss >> str) { code += str + " "; }
                code = code.substr(0, code.length() - 1);
                result = run(code);
                result = std::string(responses.load_result) + " " + result + std::string(responses.delim);
            }
            // return filepath
            // send open directory response
            else if (header == requests.new_file) {
                result = std::string(responses.file_path) + " " + return_file_path() + std::string(responses.delim);
            }
            // return filepath
            else if (header == requests.choose_file) {
                std::string filepath;
                iss >> filepath;
                result = return_file_content(filepath);
            }
            // return filepath
            // send open directory response
            else if (header == requests.open_file) {
                result = std::string(responses.file_path) + " " + return_file_path() + std::string(responses.delim);
            }
            // return version code as a single string
            else if (header == requests.get_version) {
                result = std::string(responses.history_version) + " " + return_history_versions(current_user) + std::string(responses.delim);
            }
            else if (header == requests.create_account) {
                std::string user, pwd;
                iss >> user >> pwd;
                create_account(user, pwd);
            }
            else if (header == requests.login) {
                std::string user, pwd;
                iss >> user >> pwd;
                authenticate(user, pwd);
            }
            else if (header == requests.logout) { log_out(); }
        }
        catch (std::logic_error& e) {
            result = std::string(responses.op_fail) + " " + e.what() + std::string(responses.delim);
        }
        catch (...) {
            result = std::string(responses.op_fail) + " " + "Unknown error occurred.\n" + std::string(responses.delim);
        }
    }

    void Delegate::save(std::string const & code) {
        if (!current_user.empty()) {
            std::lock_guard<std::mutex> guard(mutex);
            auto && user_repo = code_history.find(current_user)->second;
            // if identical code isn't found, save it with a time stamp
            if (user_repo.find(code) == user_repo.end()) {
                std::ostringstream oss;
                std::time_t t = std::time(nullptr);
                std::tm now_time = *std::localtime(&t);
                oss << std::put_time(&now_time, "%Y%m%e%H%M%S");
                std::string time_string = oss.str();
                user_repo.insert(std::make_pair(time_string, code));
            }
        }
        else { throw std::logic_error("Error: you're not logged in.\n"); }
    }

    void Delegate::save(std::string const & filename, std::string const & code) {
        if (!current_user.empty()) {
            std::lock_guard<std::mutex> guard(mutex);
            save(code);
            std::ofstream file(filename);
            std::istringstream iss(code);
            std::string line;
            while (std::getline(iss, line)) {
                std::cout << line << std::endl;
                file << line << '\n';
            }
            // save and write file with Qt Library
        }
        else { throw std::logic_error("Error: you're not logged in.\n"); }
    }

    std::string Delegate::run(const std::string & code) {
        std::unique_lock<std::mutex> guard(mutex);
        if (!current_user.empty()) {
            guard.unlock();
            auto interpreter = Interpreter(code);
            auto result = interpreter.start();
            return result;
        }
        else { throw std::logic_error("Error: you're not logged in.\n"); }
    }

    void Delegate::create_account(const std::string & name, const std::string & password) {
        std::lock_guard<std::mutex> guard(mutex);
        auto itr = user_info.find(name);
        if (itr != user_info.end()) { throw std::logic_error("Error: user already exists.\n"); } // user already exists
        else { user_info[name] = password; current_user = name; }
    }

    void Delegate::authenticate(const std::string & name, const std::string & password) {
        std::lock_guard<std::mutex> guard(mutex);
        auto itr = user_info.find(name);
        if (itr == user_info.end()) { throw std::logic_error("Error: user not found.\n"); } // user not found
        else if (itr->second == password) { current_user = name; } // authentication successful
        else { throw std::logic_error("Error: wrong password.\n");  } // wrong password
    }

    std::string Delegate::return_history_versions(const std::string & username) {
        std::string result;
        auto itr = code_history.find(username);
        for (auto && entry : itr->second) {
            result += entry.first + " ";
        }
        result = result.substr(0, result.length() - 1);
        return result;
    }

    std::string Delegate::return_history_code(const std::string & username, const std::string &history_key) {
        std::string result;
        auto itr = code_history.find(username);
        auto pair = itr->second.find(history_key);
        return pair->second;
    }

    std::string Delegate::return_file_content(const std::string &filepath) {
        std::ifstream file(filepath);
        std::string result, line;
        while (std::getline(file, line)) {
            result += line + '\n';
        }
        return result;
    }

    }
}
