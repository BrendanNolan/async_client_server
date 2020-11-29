#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <cstdint>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "ThreadSafeDeque.h"
#include "TaggedMessage.h"

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(
        boost::asio::io_context& ioContext, 
        ThreadSafeDeque<TaggedMessage>& messageDeque);

public:
    using Pointer = std::shared_ptr<TCPConnection>;

    static Pointer create(
        boost::asio::io_context& ioContext, 
        ThreadSafeDeque<TaggedMessage>& messageDeque);

    boost::asio::ip::tcp::socket& socket();

    void start();
    void write(std::vector<std::uint8_t> messageForClient);

private:
    void handleRead(
        const boost::system::error_code& error, size_t bytesTransferred);

    void handleWrite(
        const boost::system::error_code& error, size_t bytesTransferred);

private:
    boost::asio::ip::tcp::socket socket_;
    std::vector<std::uint8_t> messageForClient_;
    std::vector<std::uint8_t> bytesFromClient_;
    std::uint32_t advertisedHeaderSizeFromClient_ = 0u;
    ThreadSafeDeque<TaggedMessage>* messageDeque_ = nullptr;
};

#endif// TCPCONNECTION_H
