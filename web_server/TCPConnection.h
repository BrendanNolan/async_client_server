#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>

#include "ThreadSafeDeque.h"

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(
        boost::asio::io_service& ioService,
        ThreadSafeDeque<std::string>& messageDeque);

public:
    using pointer = boost::shared_ptr<TCPConnection>;

    static pointer create(
        boost::asio::io_service& ioService,
        ThreadSafeDeque<std::string>& messageDeque);

    boost::asio::ip::tcp::socket& socket();

    void start();

private:
    void handleWrite(
        const boost::system::error_code& error, size_t bytesTransferred);

private:
    boost::asio::ip::tcp::socket socket_;
    std::string messageFromClient_;
    std::string messageForClient_;
    ThreadSafeDeque<std::string>* messageDeque_ = nullptr;
};

#endif// TCPCONNECTION_H
