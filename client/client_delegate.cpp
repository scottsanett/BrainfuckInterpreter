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
            std::istringstream iss(body);
            std::string err_type, err_info, word;
            iss >> err_type;
            while (iss >> word) { err_info += word + " "; }
            err_info = err_info.substr(0, err_info.length() - 1);
            if (err_type == response_err.create_account_failure) {
                emit signal_create_account_error(err_info.c_str());
            }
            else if (err_type == response_err.authentication_failure) {
                emit signal_authentication_error(err_info.c_str());
            }
            else if (err_type == response_err.file_access_failure) {
                emit signal_file_access_failure(err_info.c_str());
            }
            else if (err_type == response_err.save_file_failure) {
                emit signal_save_file_failure(err_info.c_str());
            }
            else if (err_type == response_succ.create_account_success) {
                emit signal_create_account_success(err_info.c_str());
            }
            else if (err_type == response_succ.authentication_success) {
                emit signal_authentication_success(err_info.c_str());
            }
            else if (err_type == response_succ.file_access_success) {
                emit signal_file_access_success(err_info.c_str());
            }
        }
    }
    }
}
