#ifndef SOCKETMESSAGEENQUEUER_H
#define SOCKETMESSAGEENQUEUER_H

#include <memory>

#include <boost/asio.hpp>

class TCPConnection;

class SocketMessageEnqueuer
{
public:
    SocketMessageEnqueuer(
        boost::asio::ip::tcp::socket& socket,
        TCPConnection* connection = nullptr);
    void write(utils::Message message) const;
    void read() const;

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
    ThreadSafeQueue<utils::Message> incomingMessageQ_;
    ThreadSafeQueue<utils::Message> outgoingMessageQ_;
    TCPConnection* connection_ = nullptr;
}

#endif// SOCKETMESSAGEENQUEUER_H
