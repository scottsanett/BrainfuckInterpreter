#include "client.hpp"

//scott::client::Delegate scott::client::Session::delegate(0);

namespace scott {
    namespace client {
        void handler(client::Delegate & delegate, const std::string& response, const boost::system::error_code& ec) {
            if (ec == 0) {
                delegate.parse_response(response);
                std::cout << "Handler: Request # " << " has completed. Response: " << response << std::endl;
            }
            else {
                std::cout << "Handler: Request # " << " failed! Error code = " << ec.value() << ". Error message: " << ec.message() << std::endl;
            }
            return;
        }
    }

    Client::Client() {
        m_work.reset(new boost::asio::io_service::work(m_ios));
        m_thread.reset(new std::thread([this](){ m_ios.run(); }));
    }

    void Client::on_connection(const boost::system::error_code &ec, session_ptr this_session, client::Delegate & delegate) {
        if (ec != 0) {
            std::cout << "Client connection error occurred! Error code = " << ec.value() << ". Message: " << ec.message()
            << std::endl;
            return;
        }
        boost::asio::async_write(this_session->m_socket, boost::asio::buffer(this_session->m_request), [this, this_session, &delegate](const boost::system::error_code& ec, std::size_t bytes_transferred){
            this->on_write_complete(ec, bytes_transferred, this_session, delegate);
        });
    }

    void Client::on_write_complete(const boost::system::error_code &ec, std::size_t bytes_transferred, session_ptr this_session, client::Delegate & delegate) {
        if (ec != 0) {
            std::cout << "Client write error occurred! Error code = " << ec.value() << ". Message: " << ec.message()
            << std::endl;
            return;
        }
        boost::asio::async_read_until(this_session->m_socket, this_session->m_buffer, scott::requests.delim, [this, this_session, &delegate](const boost::system::error_code& ec, std::size_t bytes_transferred){
            this->on_read_complete(ec, bytes_transferred, this_session, delegate);
        });
    }

    void Client::on_read_complete(const boost::system::error_code &ec, std::size_t bytes_transferred, session_ptr this_session, client::Delegate & delegate) {
        if (ec != 0) {
            this_session->m_ec = ec; }
        else {
            std::istream is(&this_session->m_buffer);
            std::getline(is, this_session->m_response);
        }
        this->on_request_complete(this_session, delegate);
    }

    void Client::send_request(std::string const raw_ip_address, unsigned short port_num, std::string const request, unsigned int id, client::Delegate & delegate) {
        auto request_id = id;

        auto this_session = std::make_shared<client::Session>(m_ios, raw_ip_address, port_num, request, request_id, client::handler);
        this_session->m_socket.open(this_session->m_endpoint.protocol());

        std::unique_lock<std::mutex> lock(m_session_guard);
        m_sessions[request_id] = this_session;
        lock.unlock();

        this_session->m_socket.async_connect(this_session->m_endpoint, [this, this_session, &delegate](const boost::system::error_code& ec){
            this->on_connection(ec, this_session, delegate);
        });
    }

    void Client::on_request_complete(session_ptr session, client::Delegate & delegate) {

        boost::system::error_code ignored;
        session->m_socket.shutdown(boost::asio::socket_base::shutdown_both, ignored);

        std::unique_lock<std::mutex> lock(m_session_guard);
        auto it = m_sessions.find(session->m_index);
        if (it != m_sessions.end()) { m_sessions.erase(it); }
        lock.unlock();

        session->callback(delegate, session->m_response, session->m_ec);
    }

    void Client::m_callback(const boost::system::error_code &ec) {
        if (ec != 0) {
            std::cout << "Client: Error occurred! Error code = " << ec.value() << ". Message: " << ec.message() << std::endl;
        }
        else {
            std::cout << "Client: Request succceeded." << std::endl;
        }
    }
}
