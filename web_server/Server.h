#ifndef SERVER_H
#define SERVER_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "TCPConnection.h"

class Server
{
public:
    Server(boost::asio::io_service& ioService);

private:
    void startAccept();
    void handleAccept(
        TCPConnection::pointer newConnection,
        const boost::system::error_code& error);

private:
    boost::asio::io_service ioService_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif// SERVER_H
