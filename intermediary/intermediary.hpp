#ifndef intermediary_h
#define intermediary_h

#include "client/client.hpp"
#include "gui/brainfuckide.hpp"

namespace scott {
    namespace intermediary {
        class intermediary {
        private:
            const char * IP_ADDRESS = "127.0.0.1";
            const short port_num = 3333;
            std::string file_name;
            int id;
            
        public:
            /*------handlers that may involved client::Delegate class------*/
            void open_file_request();
            void save_file_request();
            void new_file_request();
            void save_new_file(std::string const filename, std::string const code); // sends code to the server to receive directory path
            void save_file(std::string const code);
            void choose_file(std::string const filepath); // send to server the final path of the file, expects code content as response
            void get_version(); // sends see versions request, expects history versions as response
            void choose_version(std::string const & history_key); // a version is clicked, sends history name as request, expects history code as response
            void is_logged_in();

            /*------normal handlers------*/
            void on_run_clicked(std::string const code, std::string const input); // RUN
            void on_create_account_clicked(std::string const username, std::string const password);
            void on_login_clicked(std::string const username, std::string const password);
            void on_logout_clicked(); // LOGOUT

             
        public:
            intermediary() = default;
            void close();
        };
        
    }
}


#endif /* intermediary_h */
