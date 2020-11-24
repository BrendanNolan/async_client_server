#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <cstdint>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "ThreadSafeDeque.h"
#include "Message.h"

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(
        boost::asio::io_context& ioContext, 
        ThreadSafeDeque<Message>& messageDeque);

public:
    using Pointer = std::shared_ptr<TCPConnection>;

    static Pointer create(
        boost::asio::io_context& ioContext, 
        ThreadSafeDeque<Message>& messageDeque);

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
    std::vector<std::uint8_t> bytesFromClient_;
    ThreadSafeDeque<Message>* messageDeque_ = nullptr;
};

#endif// TCPCONNECTION_H
