#ifndef server_delegate_h
#define server_delegate_h

#include "interpreter/interpreter.hpp"
#include "database/database.hpp"
#include "gui/brainfuckide.hpp"
#include <QFileInfo>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

namespace scott {
    namespace server {
        class delegate {
        private:
            scott::database::database database;
            scott::interpreter::interpreter interpreter;
            std::string current_user;
            std::string current_filename;
            bool logged_in;
            
        private:
            /* methods that deal with files */
            bool file_exists(std::string path);

            std::string run(std::string const & code); // return result
            
            void save_database() const;
            void restore_database();

        public:
            delegate();
            ~delegate();
            void parse_request(const std::string & request, std::string & result);
        };
    }
}


#endif /* server_delegate_h */
