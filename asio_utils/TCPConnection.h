#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>

#include <boost/asio.hpp>

#include "Message.h"
#include "ThreadSafeDeque.h"

namespace utils
{
class MessagePostFunctor;
class ErrorNotifyFunctor;
}// namespace utils

namespace utils
{

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(boost::asio::io_context& ioContext);

public:
    static std::shared_ptr<TCPConnection> create(
        boost::asio::io_context& ioContext);
    ~TCPConnection();

    void send(utils::Message message);
    void startReading();

    boost::asio::ip::tcp::socket& socket();

    void setMessagePostFunctor(
        std::unique_ptr<utils::MessagePostFunctor> poster);
    void setErrorNotifyFunctor(
        std::unique_ptr<utils::ErrorNotifyFunctor> notifier);

private:
    void writeHeader();
    void writeBody();

    void readHeader();
    void readBody();

    void postMessage(utils::Message message) const;
    void notifyOfError(const boost::system::error_code& error) const;

private:
    std::unique_ptr<utils::MessagePostFunctor> poster_;
    std::unique_ptr<utils::ErrorNotifyFunctor> notifier_;

    utils::Message tempIncomingMessage_;

    utils::ThreadSafeDeque<utils::Message> outQ_;

    boost::asio::ip::tcp::socket socket_;
};

}// namespace utils

#endif// TCPCONNECTION_H
