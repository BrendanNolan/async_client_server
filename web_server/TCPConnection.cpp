#include "TCPConnection.h"

#include <algorithm>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

TCPConnection::TCPConnection(
    io_context& ioContext,
    ThreadSafeDeque<Message>& messageDeque)
    : socket_{ioContext}
    , messageDeque_{&messageDeque}
{
}

TCPConnection::pointer TCPConnection::create(
    io_context& ioContext,
    ThreadSafeDeque<Message>& messageDeque)
{
    return pointer{ new TCPConnection{ 
        ioContext,
        messageDeque } };
}

tcp::socket& TCPConnection::socket()
{
    return socket_;
}

void TCPConnection::start()
{
    auto self = shared_from_this(); // See https://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp
    async_read(
        socket_,
        buffer(bytesFromClient_),
        [this, self](const boost::system::error_code& /*error*/, size_t /*bytesTransferred*/) {
            handleRead();
        });
}

void TCPConnection::write(const std::string& messageForClient)
{
    std::copy(
        messageForClient.begin(),
        messageForClient.begin() + 100, 
        messageForClient_.begin());
    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(messageForClient_),
        [this, self](const boost::system::error_code& /*error*/, size_t /*bytesTransferred*/) {
            handleWrite();
        });
}

void TCPConnection::handleWrite()
{
}

void TCPConnection::handleRead()
{
    if (!messageDeque_)
        return;
    
    messageDeque_->push_back({
        std::string(bytesFromClient_.begin(), bytesFromClient_.end()),
        shared_from_this()});
}
