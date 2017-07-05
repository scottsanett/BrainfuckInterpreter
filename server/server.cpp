#include "server.hpp"

namespace scott {
    namespace server {

        delegate session::delegate;

        void session::start() {
            boost::asio::async_read_until(*m_socket, m_request, scott::requests.delim, [this](const boost::system::error_code& ec, std::size_t bytes_transferred){
                on_request_received(ec, bytes_transferred);
            });
        }

        void session::on_request_received(const boost::system::error_code &ec, std::size_t bytes_transferred) {
            if (ec != 0) {
                std::cout << "server::session: Error occurred! Error code = " << ec.value() << ". Message: " << ec.message() << std::endl;
                return;
            }
            m_response = process_request(m_request);
            boost::asio::async_write(*m_socket, boost::asio::buffer(m_response), [this](const boost::system::error_code& ec, std::size_t bytes_transferred){
                on_response_sent(ec, bytes_transferred);
            });
        }

        std::string session::process_request(boost::asio::streambuf &request) {
            std::istream is(&request);
            std::string line, result;
            std::getline(is, line);
            delegate.parse_request(line, result);
            result += scott::requests.delim;
            return result;
        }

        void session::on_response_sent(const boost::system::error_code &ec, std::size_t bytes_transferred) {
            if (ec != 0) {
                std::cout << "server::session: Error occurred! Error code = " << ec.value() << ". Message: " << ec.message() << std::endl;
                return;
            }
        }


        void acceptor::init() {
            auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_ios);
            m_acceptor.async_accept(*socket.get(), [this, socket](const boost::system::error_code& ec){
                on_accept(ec, socket); });
        }

        void acceptor::on_accept(const boost::system::error_code &ec, socket_ptr socket) {
            if (ec == 0) {
                sessions.push_back(std::make_shared<session>(socket));
                sessions.back()->start();
            }
            else {
                std::cout << "acceptor: Error occurred! Error code = " << ec.value() << ". Message: " << ec.message() << std::endl;
            }
            init();
        }
    }
}
