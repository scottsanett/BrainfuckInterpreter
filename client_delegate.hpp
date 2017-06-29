#ifndef client_delegate_h
#define client_delegate_h

#include "namespace.hpp"

namespace scott {
    namespace client {
        class Delegate {
        private:
            void open_file_path(std::string const & filepath) {}// notify GUI to open the file path
            void load_history_versions(std::string const & history_keys) {} // notify GUI to fill the menu with the vector
            void load_history_code(std::string const & code) {} // notify GUI to refill the text
            void load_file(std::string const & code) {} // load file when the client requires to open a file
            void load_result(std::string const & output) {} // load result
            void load_error_info(std::string const & info) {} // load error info
            
        public:
            void parse_response(std::string const & response);
            Delegate() = default;
        };
    }
}

#endif /* client_delegate_h */
