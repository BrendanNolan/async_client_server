#ifndef SERVER_H
#define SERVER_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

#include <boost/asio.hpp>

#include "Message.h"
#include "TCPConnection.h"
#include "ThreadSafeDeque.h"

class Server
{
public:
    Server(boost::asio::io_context& ioContext);
    void enqueue(const Message& message);

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

    std::mutex mutex_;
    std::condition_variable condVar_;
    ThreadSafeDeque<Message> messageDeque_;
};

#endif// SERVER_H
