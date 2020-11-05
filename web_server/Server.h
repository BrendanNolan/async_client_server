#ifndef SERVER_H
#define SERVER_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

class Server
{
public:
    void start();

private:
    void handleConnection(boost::asio::ip::tcp::socket& socket);

private:
    boost::asio::io_service ioservice_;
};

#endif // SERVER_H
