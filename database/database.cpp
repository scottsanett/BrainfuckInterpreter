#include "database.hpp"

namespace scott {
    namespace database {

        /* class file */

        file::file(std::string const &  _filename, std::string const & code): filename(_filename) {
            add_new_history(code);
        }

        auto file::get_history_keys() -> std::string {
            std::string result;
            for (auto && i : history) { result += i.first + " "; }
            result = result.substr(0, result.length() - 1);
            return result;
        }

        auto file::get_history_content(const std::string &key) -> std::string {
            return history.find(key)->second;
        }

        auto file::get_file_content() -> std::string {
            return std::prev(history.end())->second;
        }

        auto file::get_file_name() -> std::string {
            return filename;
        }

        void file::add_new_history(std::string const & code) {
            std::ostringstream oss;
            std::time_t t = std::time(nullptr);
            std::tm now_time = *std::localtime(&t);
            oss << std::put_time(&now_time, "%Y%m%d%H%M%S");
            std::string time_stamp = oss.str();
            history.insert(std::make_pair(time_stamp, code));
        }


        /* class user */

        auto user::authenticate(const std::string & _pwd) -> bool {
            return (_pwd == password);
        }

        auto user::get_all_files() -> std::string {
            std::string result;
            for (auto && i : file_collection) { result += i.first + " "; }
            result = result.substr(0, result.length() - 1);
            return result;
        }

        auto user::open_file(std::string const & _filename) -> std::string {
            return file_collection.find(_filename)->second.get_file_content();
        }

        void user::save_file(const std::string & _filename, const std::string & _code) {
            auto f = file_collection.find(_filename);
            if (f != file_collection.end()) {
                file_collection.find(_filename)->second.add_new_history(_code);

//                throw std::logic_error(std::string(response_err.save_file_failure) + " " + std::string(response_err_info.file_exists)); // file_already_exists_error
            }
            else {
                file_collection.insert(std::make_pair(_filename, file(_filename, _code)));
            }
        }

        auto user::get_history_keys(const std::string & _filename) -> std::string {
            return file_collection.find(_filename)->second.get_history_keys();
        }

        auto user::get_history_content(std::string const & _filename, const std::string &_key) -> std::string {
            return file_collection.find(_filename)->second.get_history_content(_key);
        }



        /* class database */

        auto database::create_account(const std::string &_usr, const std::string &_pwd) -> enum create_account_err_code {
            if (users.find(_usr) != users.end()) {
                return create_account_err_code::account_exists;
            }
            else {
                users.insert(std::make_pair(_usr, user(_usr, _pwd)));
                return create_account_err_code::creation_successful;
            }
        }

        auto database::login(const std::string &_usr, const std::string &_pwd) -> enum authentication_err_code {
            auto itr = users.find(_usr);
            if (itr == users.end()) { return authentication_err_code::user_not_found; }
            else {
                if (itr->second.authenticate(_pwd)) { return authentication_err_code::login_successful; }
                else { return authentication_err_code::wrong_usr_or_pwd; }
            }
        }

        auto database::get_all_files(const std::string &_usr) -> std::string {
            return users.find(_usr)->second.get_all_files();
        }

        auto database::open_file(const std::string &_usr, const std::string &_filename) -> std::string {
            return users.find(_usr)->second.open_file(_filename);
        }

        void database::save_file(const std::string &_usr, const std::string &_filename, const std::string &_code) {
            users.find(_usr)->second.save_file(_filename, _code);
        }

        auto database::get_history_keys(const std::string &_usr, const std::string &_filename) -> std::string {
            return users.find(_usr)->second.get_history_keys(_filename);
        }

        auto database::get_history_content(const std::string &_usr, const std::string &_filename, const std::string &_key) -> std::string {
            return users.find(_usr)->second.get_history_content(_filename, _key);
        }

    }
}
