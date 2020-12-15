#ifndef SOCKETMESSAGEHANDLER_H
#define SOCKETMESSAGEHANDLER_H

#include <deque>
#include <memory>
#include <mutex>

#include <boost/asio.hpp>

#include "Message.h"
#include "ThreadSafeDeque.h"

// TCPConnection should go and this class should be a component of
// ClientTCPConnection and a subclass of ServerTCPConnection.

class SocketMessageHandler
    : public std::enable_shared_from_this<SocketMessageHandler>
{
public:
    SocketMessageHandler(boost::asio::ip::tcp::socket socket);
    virtual ~SocketMessageHandler() = default;

    void send(utils::Message message);
    void startReading();

    utils::ThreadSafeDeque<utils::Message>& incomingMessageQ();

private:
    void writeHeader();
    void writeBody();
    void grabNextOutgoingMessage();

    void readHeader();
    void readBody();

private: 
    std::mutex receiveMessageMutex_;
    utils::Message tempIncomingMessage_;
    utils::ThreadSafeDeque<utils::Message> incomingMessageQ_;

    std::mutex sendMessageMutex_;
    utils::Message tempOutgoingMessage_;
    std::deque<utils::Message> outgoingMessageQ_;

    boost::asio::ip::tcp::socket socket_;
};

#endif// SOCKETMESSAGEHANDLER_H
