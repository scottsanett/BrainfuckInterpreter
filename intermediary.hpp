#ifndef intermediary_h
#define intermediary_h

#include "client.hpp"

namespace scott {
    namespace client {
        class Intermediary {
        private:
            static scott::Client client;
            const char * IP_ADDRESS = "127.0.0.1";
            const short port_num = 3333;
            std::string file_name;
//            std::stack<std::string> code; // for undo and redo
            int id;
            
        public:
            /*------handlers that may involved client::Delegate class------*/
            void new_file(); // sends request to the server, expects directory filepath as resposne
            void save_file(std::string const filename, std::string const code); // sends code to the server to receive directory path
            void choose_file(std::string const filepath); // send to server the final path of the file, expects code content as response
            void open_file(); // sends open file request, expects a directory path as response
            void get_version(); // sends see versions request, expects history versions as response
            void choose_version(std::string const & history_key); // a version is clicked, sends history name as request, expects history code as response
            
            /*------normal handlers------*/
            void on_run_clicked(std::string const code, std::string const input); // RUN
            void on_create_account_clicked(std::string const username, std::string const password);
            void on_login_clicked(std::string const username, std::string const password);
            void on_logout_clicked(); // LOGOUT
            void on_text_edited(); // when text changes
            
            /*------these methods can be implemented in the UI class------*/
            /*
            void on_undo_clicked(std::string const code);
            void on_redo_clicked();
            */
             
        public:
            Intermediary() = default;
            void close() { client.close(); }
        };
        
    }
}


#endif /* intermediary_h */
