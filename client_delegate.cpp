#include "client_delegate.hpp"

namespace scott {
    namespace client {
    void Delegate::parse_response(std::string const & response) {
        std::string header, body;
        std::istringstream iss(response);
        iss >> header;
        if (header == responses.file_path) {
            std::string word;
            while (iss >> word) { body += word + " "; }
            emit signal_open_file_path(body.c_str());
        }
        else if (header == responses.history_version) {
            std::string word;
            while (iss >> word) { body += word + " "; }
            emit signal_load_history_version(body.c_str());
        }
        else if (header == responses.history_code) {
            std::string word;
            while (iss >> word) { body += word + " "; }
            load_history_code(body);
        }
        else if (header == responses.load_file) {
            iss >> body;
            load_file(body);
        }
        else if (header == responses.load_result) {
            std::string word;
            while (iss >> word) { body += word + " "; }
            emit signal_load_result(body.c_str());
        }
        else if (header == responses.op_fail) {
            std::string word;
            while (iss >> word) { body += word + " "; }
            emit signal_load_err_info(body.c_str());
        }
    }


    }
}
