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

class TCPConnection : public SocketMessageHandler
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

private:
    utils::ThreadSafeDeque<TaggedMessage>* messageDeque_ = nullptr;
};

#endif// TCPCONNECTION_H
