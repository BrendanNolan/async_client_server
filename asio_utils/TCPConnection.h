#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <mutex>

#include <boost/asio.hpp>

#include "Message.h"
#include "MessagePostFunctor.h"// May be able to forward declare
#include "ThreadSafeDeque.h"

// TCPConnection should go and this class should be a component of
// ClientTCPConnection and a subclass of ServerTCPConnection.

namespace utils
{
// Beware: TCPConnection relies on containing classes to ensure that
// it is still alive to execute handlers.
class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(boost::asio::io_context& ioContext);

public:
    static std::shared_ptr<TCPConnection> create(
        boost::asio::io_context& ioContext);

    void send(utils::Message message);
    void startReading();

    boost::asio::ip::tcp::socket& socket();

    void setMessagePostFunctor(std::unique_ptr<MessagePostFunctor> poster);

private:
    void writeHeader();
    void writeBody();

    void readHeader();
    void readBody();

private:
    std::unique_ptr<MessagePostFunctor> poster_;

    utils::Message tempIncomingMessage_;

    std::mutex sendMutex_;
    utils::ThreadSafeDeque<utils::Message> outQ_;

    boost::asio::ip::tcp::socket socket_;
};

}// namespace utils

#endif// TCPCONNECTION_H
