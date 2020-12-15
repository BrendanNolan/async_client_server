#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <thread>

#include <boost/asio.hpp>

#include "TaggedMessage.h"
#include "TCPConnection.h"
#include "ThreadSafeDeque.h"

class Server
{
public:
    Server(boost::asio::io_context& ioContext);

private:
    void startAccept();
    void handleAccept(
        TCPConnection::Pointer newConnection,
        const boost::system::error_code& error);

private:
    void processRequests();

private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::io_context* ioContext_ = nullptr;

    std::vector<std::thread> threadPool_;
    utils::ThreadSafeDeque<TaggedMessage> messageDeque_;
};

#endif// SERVER_H
