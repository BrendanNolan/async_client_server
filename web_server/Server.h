#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

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
    boost::asio::ip::tcp::acceptor acceptor_;
    int acceptCounter_ = 0;
};

#endif// SERVER_H
