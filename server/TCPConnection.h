#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "Message.h"

class Server;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(boost::asio::io_context& ioContext, Server& server);

public:
    using Pointer = std::shared_ptr<TCPConnection>;

    static Pointer create(boost::asio::io_context& ioContext, Server& server);

    boost::asio::ip::tcp::socket& socket();

    void start();
    void write(const std::string& messageForClient);

private:
    void handleRead(
        const boost::system::error_code& error, size_t bytesTransferred);

    void handleWrite(
        const boost::system::error_code& error, size_t bytesTransferred);

private:
    boost::asio::ip::tcp::socket socket_;
    std::string messageForClient_;
    std::array<char, 100> bytesFromClient_;
    Server* server_ = nullptr;
};

#endif// TCPCONNECTION_H
