#include "client_delegate.hpp"

namespace scott {
    namespace client {
    void Delegate::parse_response(std::string const & response) {
        std::string header, body;
        std::istringstream iss(response);
        iss >> header;
        if (header == responses.file_path) {
            iss >> body;
            open_file_path(body);
        }
        else if (header == responses.history_version) {
            iss >> body;
            load_history_versions(body);
        }
        else if (header == responses.history_code) {
            iss >> body;
            load_history_code(body);
        }
        else if (header == responses.load_file) {
            iss >> body;
            load_file(body);
        }
        else if (header == responses.load_result) {
            iss >> body;
            load_result(body);
        }
        else if (header == responses.op_fail) {
            iss >> body;
            load_error_info(body);
        }
    }
    }
}
