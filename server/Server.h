#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <string>
#include <thread>

#include <boost/asio.hpp>

#include "TaggedMessage.h"
#include "ThreadSafeDeque.h"

class MessageProcessFunctor;

namespace utils
{
class TCPConnection;
}

class Server
{
public:
    Server(boost::asio::io_context& ioContext);
    ~Server();

    void setMessageProcessFunctor(
        std::unique_ptr<MessageProcessFunctor> functor);

private:
    void startAccept();
    void handleAccept(
        std::shared_ptr<utils::TCPConnection> newConnection,
        const boost::system::error_code& error);

private:
    void processRequests();

private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::io_context* ioContext_ = nullptr;

    std::vector<std::thread> threadPool_;
    utils::ThreadSafeDeque<TaggedMessage> messageDeque_;

    std::unique_ptr<MessageProcessFunctor> messageProcessFunctor_;
};

#endif// SERVER_H
