#ifndef SOCKETMESSAGEHANDLER_H
#define SOCKETMESSAGEHANDLER_H

#include <memory>

#include <boost/asio.hpp>

#include "Message.h"
#include "ThreadSafeDeque.h"

// TCPConnection should go and this class should be a component of 
// ClientTCPConnection and a subclass of ServerTCPConnection.

class TCPConnectionExptal
    : public std::enable_shared_from_this<TCPConnectionExptal>
{
public:
    TCPConnectionExptal(boost::asio::ip::tcp::socket socket);
    virtual ~TCPConnectionExptal() = default;

    void send(utils::Message message);
    void startReading();

    utils::ThreadSafeDeque<utils::Message>& incomingMessageQ();

private:
    void writeHeader();
    void writeBody();

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

    boost::asio::ip::tcp::socket socket_;
};

#endif// SOCKETMESSAGEHANDLER_H
