#ifndef namespace_h
#define namespace_h

#include <map>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <utility>
#include <algorithm>
#include <exception>
#include <boost/asio.hpp>

namespace scott {
    static struct request_constants {
        const char * new_file = "S_NEW_FILE";
        const char * open_file = "S_OPEN";
        const char * save_file = "S_SAVE_FILE";
        const char * save_new_file = "S_SAVE_NEW_FILE";
        const char * choose_file = "S_CHOOSE_FILE";
        const char * get_version = "S_GET_VERSION";
        const char * choose_version = "S_CHOOSE_VERSION";
        const char * run = "S_RUN";
        const char * create_account = "S_CREATE_ACCOUNT";
        const char * login = "S_LOG_IN";
        const char * logout = "S_LOG_OUT";
        const char * undo = "S_UNDO";
        const char * redo = "S_REDO";
        const char * delim = "\r\n\r\n";
    } requests;
    
    static struct response_constants {
        const char * file_saved = "R_FILE_SAVED";
        const char * file_path = "R_FILE_PATH";
        const char * history_version = "R_HISTORY_VERSION";
        const char * history_code = "R_HISTORY_CODE";
        const char * load_file = "R_LOAD_FILE";
        const char * load_result = "R_LOAD_RESULT";
        const char * op_successful = "R_OPERATION_SUCCESSFUL";
        const char * op_fail = "R_OPERATION_FAILED";
        const char * delim = "\r\n\r\n";
    } responses;

    static struct response_success {
        const char * create_account_success = "D_CREATE_ACCOUNT_SUCCESS";
        const char * authentication_success = "D_AUTHENTICATION_SUCCESS";
        const char * file_access_success = "D_FILE_ACCESS_SUCCESS";
    } response_succ;

    static struct response_success_info {
        const char * logged_in = "You are now logged in.";
        const char * account_created = "Account created.";
    } response_succ_info;

    static struct response_errors {
//        const char * login_failure = "E_LOGIN_FAILURE";
        const char * create_account_failure = "E_CREATE_ACCOUNT_FAILURE";
        const char * authentication_failure = "E_AUTHENTICATION_FAILURE";
        const char * file_access_failure = "E_FILE_ACCESS_FAILURE";
        const char * save_file_failure = "E_SAVE_FILE_FAILURE";
    } response_err;

    static struct response_err_info {
        const char * not_logged_in = "You are not logged in.";
        const char * already_logged_in = "You are already logged in.";
        const char * user_exists = "User already exists.";
        const char * user_not_found = "User not found.";
        const char * wrong_password = "Wrong username or password.";
        const char * no_access = "You don't have access to this file.";
    } response_err_info;

    namespace client {
        class Session;
        class Delegate;
        class Intermediary;
    }
    
    namespace server {
        class Session;
        class Acceptor;
        class Delegate;
    }

    class Client;
    class Server;
    class Interpreter;

    using str_map = std::map<std::string, std::string>;
    using strvec = std::vector<std::string>;
    using session_ptr = std::shared_ptr<client::Session>;
    using socket_ptr = std::shared_ptr<boost::asio::ip::tcp::socket>;
    using cell = std::vector<char>::iterator;
    using char_pos = std::string::iterator;
}

#endif /* namespace_h */
