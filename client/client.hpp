#ifndef client_h
#define client_h

#include "client_delegate.hpp"

namespace scott {
    namespace client {
        void handler(delegate & delegate, const std::string& response, const boost::system::error_code& ec);

        // this class is designed to deal with every request that the user makes
        class session {
            using Callback = void(*)(delegate& delegate, const std::string& response, const boost::system::error_code& ec);
        public:
            boost::asio::io_service& m_ios;
            boost::asio::streambuf m_buffer;
            boost::asio::ip::tcp::endpoint m_endpoint;
            boost::asio::ip::tcp::socket m_socket;
            boost::system::error_code m_ec;
            
            std::string m_request;
            std::string m_response;
            
            unsigned int m_index;
            
            Callback callback;
            
            session(boost::asio::io_service& ios, std::string raw_ip_address, unsigned short port_num, const std::string & req, unsigned int index, Callback cb):
            m_ios(ios),
            m_endpoint(boost::asio::ip::address::from_string(raw_ip_address), port_num),
            m_socket(m_ios),
            m_request(req),
            m_index(index),
            callback(cb) { }
        };
    
    // one client needs to be able to hold many sessions, hence a session class is needed
    // the client class acts more like a container that holds these sessions
    class client: public boost::noncopyable {
    private:
        boost::asio::io_service m_ios;
        std::mutex m_session_guard;
        std::map<int, session_ptr> m_sessions;
        std::unique_ptr<boost::asio::io_service::work> m_work;
        std::unique_ptr<std::thread> m_thread;
        
        void on_connection(const boost::system::error_code& ec, session_ptr this_session, delegate & delegate);
        void on_write_complete(const boost::system::error_code& ec, std::size_t bytes_transferred, session_ptr this_session, delegate & delegate);
        void on_read_complete(const boost::system::error_code& ec, std::size_t bytes_transferred, session_ptr this_session, delegate & delegate);
        void on_request_complete(session_ptr session, delegate & delegate);
        void m_callback(const boost::system::error_code& ec);

    public:
        static scott::client::delegate delegate;

    public:
        client();
        ~client() { if (m_thread->joinable()) m_thread->join(); }
        void close() { m_work.reset(nullptr); }
        void send_request(std::string const raw_ip_address, unsigned short port_num, std::string const request, unsigned int id, class delegate & delegate);
    };

} // end of namespace scott::client
}


#endif /* client_h */
