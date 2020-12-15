#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "Message.h"
#include "TaggedMessage.h"
#include "ThreadSafeDeque.h"

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(
        boost::asio::io_context& ioContext,
       utils::ThreadSafeDeque<TaggedMessage>& messageDeque);

public:
    using Pointer = std::shared_ptr<TCPConnection>;

    static Pointer create(
        boost::asio::io_context& ioContext,
       utils::ThreadSafeDeque<TaggedMessage>& messageDeque);

    boost::asio::ip::tcp::socket& socket();

    void start();
    void write(utils::Message messageForClient);

private:
    void handleRead(
        const boost::system::error_code& error, std::size_t bytesTransferred);

    void handleHeaderWrite(
        const boost::system::error_code& error, std::size_t bytesTransferred);
    void handleBodyWrite(
        const boost::system::error_code& error, std::size_t bytesTransferred);

private:
    boost::asio::ip::tcp::socket socket_;
    utils::Message messageForClient_;
    std::vector<std::uint8_t> bytesFromClient_;
    std::uint32_t advertisedHeaderSizeFromClient_ = 0u;
   utils::ThreadSafeDeque<TaggedMessage>* messageDeque_ = nullptr;
};

#endif// TCPCONNECTION_H
