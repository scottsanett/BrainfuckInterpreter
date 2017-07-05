#ifndef DATABASE_H
#define DATABASE_H

#include "namespace.hpp"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

namespace scott {
    namespace database {
        class file {
            /* class that contains the history for one file
             * the key of entries is the history code/time stamp for the entry
             * the value of entries is the code
             */
            friend class boost::serialization::access;

        private:
            std::string filename;
            std::map<std::string, std::string> history;

        private:
            template<typename Archive>
            void serialize(Archive& ar, const unsigned int) { ar & filename & history; }

        public:
            file() = default;
            file(std::string const & filename, std::string const & code);
            auto get_file_name() -> std::string;
            auto get_file_content() -> std::string;
            void add_new_history(std::string const & code);
            auto get_history_keys() -> std::string;
            auto get_history_content(std::string const & key) -> std::string;
        };

        class user {
            /* class that contains user info including username, password
             * and all the files that belong to this user
             */
            friend class boost::serialization::access;

        private:
            std::string username;
            std::string password;
            std::map<std::string, file> file_collection;

        private:
            template<typename Archive>
            void serialize(Archive& ar, const unsigned int) {
                ar.register_type(static_cast<file*>(nullptr));
                ar & username & password & file_collection;
            }


        public:
            user() = default;
            user(std::string const & _usr, std::string const & _pwd): username(_usr), password(_pwd) {}
            auto authenticate(std::string const & _pwd) -> bool;
            auto get_all_files() -> std::string; // function to return all files of this user
            void save_file(std::string const & _filename, std::string const & _code);
            auto open_file(std::string const & _filename) -> std::string;
            auto get_history_keys(std::string const & _filename) -> std::string;
            auto get_history_content(std::string const & _filename, std::string const & _key) -> std::string;
        };



        class database {
            /* class that contains all users
             * supports create user, login, logout, get history, get code.
             */
            friend class boost::serialization::access;

        private:
            std::map<std::string, user> users;

        private:
            template<typename Archive>
            void serialize(Archive& ar, const unsigned int) {
                ar.register_type(static_cast<user*>(nullptr));
                ar & this->users;
            }

        public:
            database() = default;
            auto create_account(std::string const & _usr, std::string const & _pwd) -> enum create_account_err_code;
            auto login(std::string const & _usr, std::string const & _pwd) -> enum authentication_err_code;
            auto get_all_files(std::string const & _usr) -> std::string;
            auto open_file(std::string const & _usr, std::string const & _filename) -> std::string;
            void save_file(std::string const & _usr, std::string const & _filename, std::string const & _code);
            auto get_history_keys(std::string const & _usr, std::string const & _filename) -> std::string;
            auto get_history_content(std::string const & _usr, std::string const & _filename, std::string const & _key) -> std::string;
        };

    }
}

#endif // DATABASE_H
