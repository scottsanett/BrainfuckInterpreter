#include "server_delegate.hpp"

namespace scott {
    namespace server {

        // filling all the information from the cache
        delegate::delegate():
            current_user(std::string{}),
            current_filename(std::string{})
        {
            logged_in = false;
            restore_database();
        }


    delegate::~delegate() {}

    void delegate::save_database() const {
        std::ofstream ofs(".db", std::ios_base::binary);
        boost::archive::binary_oarchive oa(ofs);
        oa << database;
    }

    void delegate::restore_database() {
        if (!file_exists(".db"))
            return;
        std::ifstream ifs(".db", std::ios_base::binary);
        boost::archive::binary_iarchive ia(ifs);
        ia >> database;
    }

    void delegate::parse_request(const std::string &request, std::string &result) {
        std::string header;
        std::istringstream iss(request);
        iss >> header;
        try {
            // only the code is saved, the input is ignored.
            if (header == requests.save_file_request) {
                auto all_files = database.get_all_files(current_user);
                result = std::string(responses.save_file_request) + " " + all_files + std::string(responses.delim);
            }
            else if (header == requests.open_file_request) {
                auto all_files = database.get_all_files(current_user);
                result = std::string(responses.open_file_request) + " " + all_files + std::string(responses.delim);
            }
            else if (header == requests.new_file_request) {
                current_filename.clear();
            }
            else if (header == requests.save_new_file) {
                std::string filename, code;
                iss >> filename >> code;    
                current_filename = filename;
                database.save_file(current_user, current_filename, code);
                save_database();
                result = std::string(responses.file_saved) + " " + std::string(responses.delim);
            }
            else if (header == requests.save_file) {
                std::string code;
                iss >> code;
                database.save_file(current_user, current_filename, code);
                save_database();
                result = std::string(responses.file_saved) + " " + std::string(responses.delim);
            }
            else if (header == requests.run) {
                std::string str, code;
                while (iss >> str) { code += str + " "; }
                code = code.substr(0, code.length() - 1);
                result = run(code);
                result = std::string(responses.load_result) + " " + result + std::string(responses.delim);
            }
            else if (header == requests.choose_file) {
                std::string filepath;
                iss >> filepath;
                current_filename = filepath;
                auto content = database.open_file(current_user, current_filename);
                result = std::string(responses.load_file) + " " + content + std::string(responses.delim);
            }
            else if (header == requests.get_version) {
                auto keys = database.get_history_keys(current_user, current_filename);
                result = std::string(responses.history_version) + " " + keys + std::string(responses.delim);
            }
            else if (header == requests.choose_version) {
                std::string history_key;
                iss >> history_key;

                auto content = database.get_history_content(current_user, current_filename, history_key);
                result = std::string(responses.history_code) + " " + content + std::string(responses.delim);
            }
            else if (header == requests.create_account) {
                std::string user, pwd;
                iss >> user >> pwd;

                auto err_code = database.create_account(user, pwd);
                save_database();
                if (err_code == create_account_err_code::creation_successful) {
                    throw std::logic_error(std::string(response_succ.create_account_success) + " " + std::string(response_succ_info.account_created));
                }
                else if (err_code == create_account_err_code::account_exists) {
                    throw std::logic_error(std::string(response_err.create_account_failure) + " " + std::string(response_err_info.user_exists));
                }
            }
            else if (header == requests.login) {
                std::string user, pwd;
                iss >> user >> pwd;
                auto err_code = database.login(user, pwd);
                if (err_code == authentication_err_code::login_successful) {
                    current_user = user;
                    throw std::logic_error(std::string(response_succ.authentication_success) + " " + std::string(response_succ_info.logged_in));
                }
                else if (err_code == authentication_err_code::wrong_usr_or_pwd) {
                    throw std::logic_error(std::string(response_err.authentication_failure) + " " + std::string(response_err_info.wrong_password));
                }
                else if (err_code == authentication_err_code::user_not_found) {
                    throw std::logic_error(std::string(response_err.authentication_failure) + " " + std::string(response_err_info.user_not_found));
                }
            }
            else if (header == requests.logout) {
                current_user.clear();
                logged_in = false;
            }
        }
        catch (std::logic_error& e) {
            result = std::string(responses.op_fail) + " " + e.what() + std::string(responses.delim);
        }
        catch (...) {
            result = std::string(responses.op_fail) + " " + "Unknown error occurred.\n" + std::string(responses.delim);
        }
    }

    bool delegate::file_exists(std::string path) {
        QFileInfo check_file(path.c_str());
        return check_file.exists() && check_file.isFile();
    }

    std::string delegate::run(const std::string & code) {
        if (!current_user.empty()) {
            auto result = interpreter.start(code);
            return result;
        }
        else {
            throw std::logic_error(std::string(response_err.authentication_failure) + " " + std::string(response_err_info.not_logged_in));
        }
    }

    }
}
