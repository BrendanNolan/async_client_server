#include "TCPConnection.h"

#include <algorithm>
#include <ctime>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>

#include "Server.h"

using namespace boost::asio;
using namespace boost::asio::ip;

TCPConnection::TCPConnection(io_context& ioContext, Server& server)
    : socket_{ ioContext }
    , server_{ &server }
{
}

TCPConnection::Pointer TCPConnection::create(
    io_context& ioContext, Server& server)
{
    return Pointer{ new TCPConnection{ ioContext, server } };
}

tcp::socket& TCPConnection::socket()
{
    return socket_;
}

void TCPConnection::start()
{
    auto self =
        shared_from_this();// See
                           // https://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp
    async_read(
        socket_,
        buffer(bytesFromClient_),
        [this, self](
            const boost::system::error_code& /*error*/,
            size_t /*bytesTransferred*/) { handleRead(); });
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
        [this, self](
            const boost::system::error_code& /*error*/,
            size_t /*bytesTransferred*/) { handleWrite(); });
}

void TCPConnection::handleWrite()
{
    std::cout << "Finished sending response to client.\n";
}

void TCPConnection::handleRead()
{
    if (!server_)
        return;
    std::cout << "Queueing up message..." << std::endl;
    server_->enqueue(
        Message{ std::string(bytesFromClient_.begin(), bytesFromClient_.end()),
                 shared_from_this() });
}
