#include "client_delegate.hpp"

namespace scott {
    namespace client {
    void Delegate::parse_response(std::string const & response) {
        std::string header, body, word;
        std::istringstream iss(response);
        iss >> header;
        while (iss >> word) { body += word + " "; }
        body = body.substr(0, body.size() - 1);
        if (header == responses.file_saved) {
            emit signal_file_saved();
        }
        else if (header == responses.file_path) {
            emit signal_load_file_path(body.c_str());
        }
        else if (header == responses.history_version) {
            emit signal_load_history_version(body.c_str());
        }
        else if (header == responses.history_code) {
            emit signal_load_history_code(body.c_str());
        }
        else if (header == responses.load_file) {
            emit signal_load_file(body.c_str());
        }
        else if (header == responses.load_result) {
            emit signal_load_result(body.c_str());
        }
        else if (header == responses.op_fail) {
            emit signal_load_err_info(body.c_str());
        }
    }
    }
}
