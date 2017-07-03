#include "server_delegate.hpp"

namespace scott {
    namespace server {

        // filling all the information from the cache
        Delegate::Delegate():
            current_user(std::string{}),
            current_filename(std::string{})
        {
            logged_in = false;
            std::lock_guard<std::mutex> guard(mutex);
            std::ifstream users("./.userinfo");
            for(std::string line; std::getline(users, line); ) {
                std::istringstream iss(line);
                std::string username, password;
                iss >> username >> password;
                user_info.insert(std::make_pair(username, password));
            }
            users.close();
        }


    Delegate::~Delegate() {
        std::ofstream users("./.userinfo");
        for (auto && i : user_info) {
            std::string line = i.first + " " + i.second;
            users << line << std::endl;
        }
        users.close();
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
                save_new_file(filename, code);
                read_user_history_from_file();
                result = std::string(responses.file_saved) + " " + std::string(responses.delim);
            }
            else if (header == requests.save_file) {
                std::string code;
                iss >> code;
                save_code_history(code);
                read_user_history_from_file();
                result = std::string(responses.file_saved) + " " + std::string(responses.delim);
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
                current_filename.clear();
                result = std::string(responses.file_path) + " " + return_file_path() + std::string(responses.delim);
            }
            // return filepath
            else if (header == requests.choose_file) {
                std::string filepath;
                iss >> filepath;
                current_filename = filepath;
                std::string history_url;
                for (auto && i : user_info) {
                    auto path = get_hidden_history_filepath(i.first);
                    if (file_exists(path)) { history_url = path; break; }
                    else { continue; }
                }
                if (history_url.empty() || history_url.find(".bf_" + current_user + ".history") == std::string::npos) {
                    throw std::logic_error(std::string(response_err.file_access_failure) + " " + std::string(response_err_info.no_access));
                }
                read_user_history_from_file();
                result = std::string(responses.load_file) + " " + return_file_content(filepath) + std::string(responses.delim);
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
            else if (header == requests.choose_version) {
                std::string history_key;
                iss >> history_key;
                result = std::string(responses.history_code) + " " + return_history_code(current_user, history_key) + std::string(responses.delim);
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

    bool Delegate::file_exists(std::string path) {
        QFileInfo check_file(path.c_str());
        return check_file.exists() && check_file.isFile();
    }

    void Delegate::save_new_file(std::string const & filename, std::string const & code) {
        if (!current_user.empty()) {
            current_filename = filename;
            save_code_history(code);
            std::lock_guard<std::mutex> guard(mutex);
            std::ofstream file(filename);
            std::istringstream iss(code);
            std::string line;
            while (std::getline(iss, line)) {
                file << line << '\n';
            }
        }
        else {
            throw std::logic_error(std::string(response_err.save_file_failure) + " " + std::string(response_err_info.not_logged_in));
//            throw std::logic_error("Error: you're not logged in.\n");
        }
    }

    void Delegate::save_code_history(const std::string &code) {
        if (!current_user.empty()) {
            // if identical code isn't found, save it with a time stamp
            if (code_history.find(code) == code_history.end()) {
                std::ostringstream oss;
                std::time_t t = std::time(nullptr);
                std::tm now_time = *std::localtime(&t);
                oss << std::put_time(&now_time, "%Y%m%d%H%M%S");
                std::string time_string = oss.str();
                code_history.insert(std::make_pair(time_string, code));
            }
            save_user_history_to_file();
        }
        else {
            throw std::logic_error(std::string(response_err.authentication_failure) + " " + std::string(response_err_info.not_logged_in));
        }
    }

    void Delegate::save_user_history_to_file() {
        if (user_info.size() != 0) {
            if (code_history.empty()) { return; }
            auto filepath = get_hidden_history_filepath(current_user);
            std::ofstream history(filepath);
            for (auto && entry : code_history) {
                std::string entry_line = entry.first + " " + entry.second;
                history << entry_line << std::endl;
            }
            history.close();
        }
    }

    void Delegate::read_user_history_from_file() {
        auto filepath = get_hidden_history_filepath(current_user);
        std::ifstream history(filepath);
        std::string line, stamp, code;
        while (std::getline(history, line)) {
            std::istringstream iss(line);
            iss >> stamp;
            std::string temp;
            while (iss >> temp) { code += temp; }
            code_history.insert(std::make_pair(stamp, code));
        }
    }

    std::string Delegate::get_hidden_history_filepath(std::string const & username) {
        auto url_copy1 = current_filename;
        auto url_copy2 = current_filename;
        auto elements = std::string("/");
        url_copy1.erase(std::next(std::find_end(url_copy1.cbegin(), url_copy1.cend(), elements.cbegin(), elements.cend())), url_copy1.end());
        auto dir = url_copy1;
        url_copy2.erase(url_copy2.begin(), std::next(std::find_end(url_copy2.cbegin(), url_copy2.cend(), elements.cbegin(), elements.cend())));
        auto filename = url_copy2;
        std::string result = dir + "." + filename + "_" + username + ".history";
        return result;
    }


    std::string Delegate::run(const std::string & code) {
        std::unique_lock<std::mutex> guard(mutex);
        if (!current_user.empty()) {
            guard.unlock();
            auto result = BrainfuckIDE::interpreter.start(code);
            return result;
        }
        else {
            throw std::logic_error(std::string(response_err.authentication_failure) + " " + std::string(response_err_info.not_logged_in));
        }
    }

    void Delegate::log_out() {
        current_user.clear();
        logged_in = false;
    }

    void Delegate::create_account(const std::string & name, const std::string & password) {
        std::lock_guard<std::mutex> guard(mutex);
        auto itr = user_info.find(name);
        if (itr != user_info.end()) {
            throw std::logic_error(std::string(response_err.create_account_failure) + " " + std::string(response_err_info.user_exists));
        } // user already exists
        else {
            user_info[name] = password;
            current_user = name;
            throw std::logic_error(std::string(response_succ.create_account_success) + " " + std::string(response_succ_info.account_created));
        }
    }

    void Delegate::authenticate(const std::string & name, const std::string & password) {
        std::lock_guard<std::mutex> guard(mutex);
        if (logged_in == true)
            throw std::logic_error(std::string(response_succ.authentication_success) + " " + std::string(response_err_info.already_logged_in));
        auto itr = user_info.find(name);
        if (itr == user_info.end()) {
            throw std::logic_error(std::string(response_err.authentication_failure) + " " + std::string(response_err_info.user_not_found));
        } // user not found
        else if (itr->second == password) {
            current_user = name;
            logged_in = true;
            throw std::logic_error(std::string(response_succ.authentication_success) + " " + std::string(response_succ_info.logged_in));
        } // authentication successful
        else {
            throw std::logic_error(std::string(response_err.authentication_failure) + " " + std::string(response_err_info.wrong_password));
        } // wrong password
    }

    std::string Delegate::return_history_versions(const std::string & username) {
        if (current_user.empty()) {
            throw std::logic_error(std::string(response_err.authentication_failure) + " " + std::string(response_err_info.not_logged_in));
        }
        else if (current_filename.empty()) {
            throw std::logic_error("Error: You haven't saved the current file, or no file is opened.\n");
        }
        std::string result;
        for (auto && i : code_history) { result += i.first + " "; }
        result = result.substr(0, result.length() - 1);
        return result;
    }

    std::string Delegate::return_history_code(const std::string & username, const std::string &history_key) {
        auto itr = code_history.find(history_key);
        return itr->second;
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
