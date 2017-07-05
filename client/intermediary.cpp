#include "client/intermediary.hpp"

namespace scott {
    namespace client {

    void Intermediary::close() {
        BrainfuckIDE::client.close();
    }

    void Intermediary::open_file_request() {
        std::string request = std::string(scott::requests.open_file_request) + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::save_file_request() {
        std::string request = std::string(scott::requests.save_file_request) + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::new_file_request() {
        std::string request = std::string(scott::requests.new_file_request) + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::save_new_file(std::string const filename, std::string const code) {
        std::string request = std::string(scott::requests.save_new_file) + " " + filename + " " + code + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::save_file(const std::string code) {
        std::string request = std::string(scott::requests.save_file) + " " + code + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::choose_file(std::string const filepath) {
        std::string request = std::string(scott::requests.choose_file) + " " + filepath + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }


    void Intermediary::get_version() {
        std::string request = std::string(scott::requests.get_version) + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::choose_version(std::string const & history_key) {
        std::string request = std::string(scott::requests.choose_version) + " " + history_key + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::on_run_clicked(std::string const code, std::string const input) {
        std::string request = std::string(scott::requests.run) + " " + code + " " + input + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::on_create_account_clicked(std::string const username, std::string const password) {
        std::string request = std::string(scott::requests.create_account) + " " + username + " " + password + " " + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::on_login_clicked(std::string const username, std::string const password) {
        std::string request = std::string(scott::requests.login) + " " + username + " " + password + " " + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    void Intermediary::on_logout_clicked() {
        std::string request = std::string(scott::requests.logout) + std::string(scott::requests.delim);
        BrainfuckIDE::client.send_request(IP_ADDRESS, port_num, request, ++id, BrainfuckIDE::client_delegate);
    }

    }
}
