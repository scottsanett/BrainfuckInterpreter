#ifndef server_delegate_h
#define server_delegate_h

#include "interpreter.hpp"

namespace scott {
    namespace server {
        class Delegate {
        private:
            str_map user_info;
            std::map<std::string, str_map> code_history;
            std::string current_user;
            std::mutex mutex;
            
            void save(std::string const & code);
            void save(std::string const & filename, std::string const & code);
            void log_out() { current_user.clear(); }
            void authenticate(std::string const & name, std::string const & password);
            void create_account(std::string const & name, std::string const & password);
            
            std::string run(std::string const & code); // return result
            std::string return_history_versions(std::string const & username); // all history versions
            std::string return_history_code(std::string const & username, std::string const & history_key); // code of a particular history version
            std::string return_file_path() { return "./"; }; // return file path to the directory where the files are saved
            std::string return_file_content(std::string const & filepath);
            
        public:
            Delegate();
            ~Delegate();
            void parse_request(const std::string & request, std::string & result);
        };
    }
}


#endif /* server_delegate_h */
