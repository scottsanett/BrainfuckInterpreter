#include "client.hpp"

namespace scott {
    namespace client {
        void handler(delegate & delegate, const std::string& response, const boost::system::error_code& ec) {
            if (ec == 0) {
                delegate.parse_response(response);
//                std::cout << "Handler: Request # " << " has completed. Response: " << response << std::endl;
            }
            else {
//                std::cout << "Handler: Request # " << " failed! Error code = " << ec.value() << ". Error message: " << ec.message() << std::endl;
            }
            return;
        }

    scott::client::delegate scott::client::client::delegate(0);

    client::client() {
        m_work.reset(new boost::asio::io_service::work(m_ios));
        m_thread.reset(new std::thread([this](){ m_ios.run(); }));
    }

    void client::on_connection(const boost::system::error_code &ec, session_ptr this_session, class delegate & delegate) {
        if (ec != 0) {
            std::cout << "client connection error occurred! Error code = " << ec.value() << ". Message: " << ec.message()
            << std::endl;
            return;
        }
        boost::asio::async_write(this_session->m_socket, boost::asio::buffer(this_session->m_request), [this, this_session, &delegate](const boost::system::error_code& ec, std::size_t bytes_transferred){
            this->on_write_complete(ec, bytes_transferred, this_session, delegate);
        });
    }

    void client::on_write_complete(const boost::system::error_code &ec, std::size_t bytes_transferred, session_ptr this_session, class delegate & delegate) {
        if (ec != 0) {
            std::cout << "client write error occurred! Error code = " << ec.value() << ". Message: " << ec.message()
            << std::endl;
            return;
        }
        boost::asio::async_read_until(this_session->m_socket, this_session->m_buffer, scott::requests.delim, [this, this_session, &delegate](const boost::system::error_code& ec, std::size_t bytes_transferred){
            this->on_read_complete(ec, bytes_transferred, this_session, delegate);
        });
    }

    void client::on_read_complete(const boost::system::error_code &ec, std::size_t bytes_transferred, session_ptr this_session, class delegate & delegate) {
        if (ec != 0) {
            this_session->m_ec = ec; }
        else {
            std::istream is(&this_session->m_buffer);
            std::getline(is, this_session->m_response);
        }
        this->on_request_complete(this_session, delegate);
    }

    void client::send_request(std::string const raw_ip_address, unsigned short port_num, std::string const request, unsigned int id, class delegate & delegate) {
        auto request_id = id;

        auto this_session = std::make_shared<session>(m_ios, raw_ip_address, port_num, request, request_id, handler);
        this_session->m_socket.open(this_session->m_endpoint.protocol());

        std::unique_lock<std::mutex> lock(m_session_guard);
        m_sessions[request_id] = this_session;
        lock.unlock();

        this_session->m_socket.async_connect(this_session->m_endpoint, [this, this_session, &delegate](const boost::system::error_code& ec){
            this->on_connection(ec, this_session, delegate);
        });
    }

    void client::on_request_complete(session_ptr session, class delegate & delegate) {

        boost::system::error_code ignored;
        session->m_socket.shutdown(boost::asio::socket_base::shutdown_both, ignored);

        std::unique_lock<std::mutex> lock(m_session_guard);
        auto it = m_sessions.find(session->m_index);
        if (it != m_sessions.end()) { m_sessions.erase(it); }
        lock.unlock();

        session->callback(delegate, session->m_response, session->m_ec);
    }

    }
}
