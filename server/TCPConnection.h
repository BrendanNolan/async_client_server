#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "Message.h"
#include "SocketMessageHandler.h"
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

    void write(utils::Message);
    void read();

private:
    void handleRead(
        const boost::system::error_code& error, std::size_t bytesTransferred);

    void handleWrite(
        const boost::system::error_code& error, std::size_t bytesTransferred);

private : 
    utils::SocketMessageHandler messageHandler_;
    utils::ThreadSafeDeque<TaggedMessage>* messageDeque_ = nullptr;
};

#endif// TCPCONNECTION_H
