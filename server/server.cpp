#include "server.hpp"

namespace scott {
    namespace server {

        void Session::start() {
            boost::asio::async_read_until(*m_socket, m_request, scott::requests.delim, [this](const boost::system::error_code& ec, std::size_t bytes_transferred){
                on_request_received(ec, bytes_transferred);
            });
        }

        void Session::on_request_received(const boost::system::error_code &ec, std::size_t bytes_transferred) {
            if (ec != 0) {
                std::cout << "Server::Session: Error occurred! Error code = " << ec.value() << ". Message: " << ec.message() << std::endl;
                return;
            }
            m_response = process_request(m_request);
            boost::asio::async_write(*m_socket, boost::asio::buffer(m_response), [this](const boost::system::error_code& ec, std::size_t bytes_transferred){
                on_response_sent(ec, bytes_transferred);
            });
        }

        std::string Session::process_request(boost::asio::streambuf &request) {
            std::istream is(&request);
            std::string line, result;
            std::getline(is, line);
            BrainfuckIDE::server_delegate.parse_request(line, result);
            result += scott::requests.delim;
            return result;
        }

        void Session::on_response_sent(const boost::system::error_code &ec, std::size_t bytes_transferred) {
            if (ec != 0) {
                std::cout << "Server::Session: Error occurred! Error code = " << ec.value() << ". Message: " << ec.message() << std::endl;
                return;
            }
        }


        void Acceptor::init() {
            auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_ios);
            m_acceptor.async_accept(*socket.get(), [this, socket](const boost::system::error_code& ec){
                on_accept(ec, socket); });
        }

        void Acceptor::on_accept(const boost::system::error_code &ec, socket_ptr socket) {
            if (ec == 0) {
                sessions.push_back(std::make_shared<Session>(socket));
                sessions.back()->start();
            }
            else {
                std::cout << "Acceptor: Error occurred! Error code = " << ec.value() << ". Message: " << ec.message() << std::endl;
            }
            init();
        }
    }
}
