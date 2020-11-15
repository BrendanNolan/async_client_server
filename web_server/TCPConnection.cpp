#include "TCPConnection.h"

#include <algorithm>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

TCPConnection::TCPConnection(
    io_service& ioService,
    ThreadSafeDeque<Message>& messageDeque)
    : socket_{ioService}
    , messageDeque_{&messageDeque}
{
}

TCPConnection::pointer TCPConnection::create(
    io_service& ioService,
    ThreadSafeDeque<Message>& messageDeque)
{
    return pointer{ new TCPConnection{ 
        ioService,
        messageDeque } };
}

tcp::socket& TCPConnection::socket()
{
    return socket_;
}

void TCPConnection::start()
{
    async_read(
        socket_,
        buffer(bytesFromClient_),
        boost::bind(
            &TCPConnection::handleRead,
            shared_from_this(),
            placeholders::error,
            placeholders::bytes_transferred));
}

void TCPConnection::write(const std::string& messageForClient)
{
    std::copy(
        messageForClient.begin(),
        messageForClient.begin() + 100, 
        messageForClient_.begin());
    async_write(
        socket_,
        buffer(messageForClient_),
        boost::bind(
            &TCPConnection::handleWrite,
            shared_from_this(),
            placeholders::error,
            placeholders::bytes_transferred));
}

void TCPConnection::handleWrite(
    const boost::system::error_code& /*error*/, size_t /*bytesTransferred*/)
{
}

void TCPConnection::handleRead(
    const boost::system::error_code& /*error*/, size_t /*bytesTransferred*/)
{
    if (!messageDeque_)
        return;
    
    messageDeque_->push_back({
        std::string(bytesFromClient_.begin(), bytesFromClient_.end()),
        shared_from_this()});
}
