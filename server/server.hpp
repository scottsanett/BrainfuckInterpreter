#ifndef server_h
#define server_h

#include "server/server_delegate.hpp"
#include "gui/brainfuckide.hpp"

namespace scott {
    namespace server {

        using handler = void (*) (const boost::system::error_code& ec, std::size_t bytes_transferred);

        class Session {
        private:
            socket_ptr m_socket;
            std::string m_response;
            boost::asio::streambuf m_request;
//            static server::Delegate server_delegate;  // dealing with all the other stuff
            
        private:
            std::string process_request(boost::asio::streambuf& request);
            
            // callback handlers
            void on_request_received(const boost::system::error_code& ec, std::size_t bytes_transferred);
            void on_response_sent(const boost::system::error_code& ec, std::size_t bytes_transferred);
            
            
        public:
            Session(socket_ptr socket): m_socket(socket) {}
            void start();
        };

        
        class Acceptor {
        private:
            std::vector<std::shared_ptr<server::Session>> sessions;
            boost::asio::io_service& m_ios;
            boost::asio::ip::tcp::acceptor m_acceptor;
            
            void init();
            void on_accept(const boost::system::error_code& ec, socket_ptr socket);
            
        public:
            Acceptor(boost::asio::io_service& io, unsigned short port_num): m_ios(io), m_acceptor(m_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_num)) {}
            void start() { m_acceptor.listen(); init(); }
        };
    } /* namespace scott::server */


    class Server: public boost::noncopyable {
    private:
        const unsigned short m_port_num;
        
        boost::asio::io_service m_ios;
        std::unique_ptr<boost::asio::io_service::work> m_work;
        std::unique_ptr<server::Acceptor> m_acceptor;
        std::unique_ptr<std::thread> m_thread;
        
    public:
        void start() {
            m_acceptor.reset(new server::Acceptor(m_ios, m_port_num));
            m_acceptor->start();
            m_thread.reset(new std::thread([this](){ this->m_ios.run(); }));
        }
        
    public:
        Server(unsigned short port_num): m_port_num(port_num) {
            m_work.reset(new boost::asio::io_service::work(m_ios));
        }
        void stop() { m_ios.stop(); }
        ~Server() { if(m_thread->joinable()) m_thread->join(); }
    };
}


#endif /* server_h */
