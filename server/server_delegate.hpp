#ifndef server_delegate_h
#define server_delegate_h

#include "interpreter/interpreter.hpp"
#include <QFileInfo>

namespace scott {
    namespace server {
        class Delegate {
        private:
            str_map user_info;
            str_map code_history;
//            std::map<std::string, str_map> code_history;
            std::string current_user;
            std::string current_filename;
            std::mutex mutex;
            bool logged_in;
            
            /* methods that deal with files */
            bool file_exists(std::string path);
            void save_new_file(std::string const & filename, std::string const & code);
            void save_code_history(std::string const & code);
            void save_user_history_to_file();
            void read_user_history_from_file();
            std::string get_hidden_history_filepath(const std::string & username);

            void log_out();
            void authenticate(std::string const & name, std::string const & password);
            void create_account(std::string const & name, std::string const & password);

            std::string run(std::string const & code); // return result
            std::string return_history_versions(std::string const & username); // all history versions
            std::string return_history_code(std::string const & username, std::string const & history_key); // code of a particular history version
            std::string return_file_path() { return "~"; } // return file path to the directory where the files are saved
            std::string return_file_content(std::string const & filepath);

            
        public:
            Delegate();
            ~Delegate();
            void parse_request(const std::string & request, std::string & result);
        };
    }
}


#endif /* server_delegate_h */
