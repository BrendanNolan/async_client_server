#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

class Client
{
public:
    void start();

private:
    void handleConnection(boost::asio::ip::tcp::socket& socket);

private:
    boost::asio::io_service ioservice_;
    std::string logFile_ = "web_client.log";
};

#endif// CLIENT_H
