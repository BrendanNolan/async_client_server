#ifndef SOCKETMESSAGEENQUEUER_H
#define SOCKETMESSAGEENQUEUER_H

#include <memory>

#include <boost/asio.hpp>

#include "Message.h"
#include "ThreadSafeDeque.h"

class TCPConnection;

class SocketMessageEnqueuer
{
public:
    SocketMessageEnqueuer(
        boost::asio::ip::tcp::socket& socket,
        TCPConnection* connection = nullptr);
    void write(utils::Message message);
    void read();

private:
    virtual void handleHeaderRead(
        const boost::system::error_code& error, std::size_t bytesTransferred);

    virtual void handleBodyRead(
        const boost::system::error_code& error, std::size_t bytesTransferred);

    virtual void handleHeaderWrite(
        const boost::system::error_code& error, std::size_t bytesTransferred);

    virtual void handleBodyWrite(
        const boost::system::error_code& error, std::size_t bytesTransferred);

private:
    utils::Message tempIncomingMessage_;
    utils::ThreadSafeDeque<utils::Message> incomingMessageQ_;
    utils::ThreadSafeDeque<utils::Message> outgoingMessageQ_;
    
    boost::asio::ip::tcp::socket* socket_ = nullptr;
    TCPConnection* connection_ = nullptr;
}

#endif// SOCKETMESSAGEENQUEUER_H
