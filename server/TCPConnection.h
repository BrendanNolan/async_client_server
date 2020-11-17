#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "Message.h"
#include "ThreadSafeDeque.h"

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(
        boost::asio::io_context& ioContext,
        ThreadSafeDeque<Message>& messageDeque);

public:
    using pointer = std::shared_ptr<TCPConnection>;

    static pointer create(
        boost::asio::io_context& ioContext,
        ThreadSafeDeque<Message>& messageDeque);

    boost::asio::ip::tcp::socket& socket();

    void start();
    void write(const std::string& messageForClient);

private:
    void handleRead();

    void handleWrite();

private:
    boost::asio::ip::tcp::socket socket_;
    std::string messageForClient_;
    std::array<char, 100> bytesFromClient_;
    ThreadSafeDeque<Message>* messageDeque_ = nullptr;
};

#endif// TCPCONNECTION_H
