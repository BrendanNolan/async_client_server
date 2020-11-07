#ifndef SERVER_H
#define SERVER_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

class Server
{
public:
    Server(boost::asio::io_context& ioContext);

private:
    void startAccept();
    void handleAccept(
        tcp_connection::pointer new_connection,
        const boost::system::error_code& error);

private:
    boost::asio::io_context ioContext_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif// SERVER_H
