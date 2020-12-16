#ifndef SOCKETMESSAGEHANDLER_H
#define SOCKETMESSAGEHANDLER_H

#include <deque>
#include <memory>
#include <mutex>

#include <boost/asio.hpp>

#include "Message.h"
#include "MessagePoster.h" // May be able to forward declare
#include "ThreadSafeDeque.h"

// TCPConnection should go and this class should be a component of
// ClientTCPConnection and a subclass of ServerTCPConnection.

namespace utils
{
// Beware: SocketMessageHandler relies on containing classes to ensure that
// it is still alive to execute handlers.
class SocketMessageHandler
{
public:
    SocketMessageHandler(boost::asio::io_context& ioContext);

    void send(utils::Message message);
    void startReading();

    boost::asio::ip::tcp::socket& socket();

    utils::ThreadSafeDeque<utils::Message>& incomingMessageQ();

private:
    void writeHeader();
    void writeBody();
    void grabNextOutgoingMessage();

    void readHeader();
    void readBody();

private:
    std::unique_ptr<MessagePoster> poster_;

    utils::Message tempIncomingMessage_;

    std::mutex sendMessageMutex_;
    utils::Message tempOutgoingMessage_;
    std::deque<utils::Message> outgoingMessageQ_;

    boost::asio::ip::tcp::socket socket_;
};

}// namespace utils

#endif// SOCKETMESSAGEHANDLER_H
