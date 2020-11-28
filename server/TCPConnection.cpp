#include "TCPConnection.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <utility>

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

TCPConnection::TCPConnection(
    io_context& ioContext, ThreadSafeDeque<Message>& messageDeque)
    : socket_{ ioContext }
    , messageDeque_{ &messageDeque }
{
}

TCPConnection::Pointer TCPConnection::create(
    io_context& ioContext, ThreadSafeDeque<Message>& messageDeque)
{
    return Pointer{ new TCPConnection{ ioContext, messageDeque } };
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
    socket_.async_read_some(// Hack to make reading work without filling buffer.
        buffer(bytesFromClient_),
        [this, self](
            const boost::system::error_code& error, size_t bytesTransferred) {
            handleRead(error, bytesTransferred);
        });
}

void TCPConnection::write(std::vector<std::uint8_t> messageForClient)
{
    messageForClient_ = std::move(messageForClient);
    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(messageForClient_),
        [this, self](
            const boost::system::error_code& error, size_t bytesTransferred) {
            handleWrite(error, bytesTransferred);
        });
}

void TCPConnection::handleWrite(
    const boost::system::error_code& error, size_t bytesTransferred)
{
    if (error)
    {
        std::cout << "TCPConnection::handleWrite(): " << error.message()
                  << std::endl;
        return;
    }

    std::cout << "Finished sending response to client.\n";
}

void TCPConnection::handleRead(
    const boost::system::error_code& error, size_t bytesTransferred)
{
    if (error)
    {
        std::cout << "TCPConnection::handleRead(): " << error.message()
                  << std::endl;
        return;
    }
    if (!messageDeque_)
        return;
    std::cout << "Queueing up message..." << std::endl;
    messageDeque_->push_back(
        Message{
            bytesFromClient_.front(),
            std::vector<std::uint8_t>(
                bytesFromClient_.begin() + 1, 
                bytesFromClient_.end()),
            shared_from_this() });
}
