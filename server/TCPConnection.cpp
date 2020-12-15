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
    io_context& ioContext, utils::ThreadSafeDeque<TaggedMessage>& messageDeque)
    : socket_{ ioContext }
    , messageDeque_{ &messageDeque }
{
}

TCPConnection::Pointer TCPConnection::create(
    io_context& ioContext, utils::ThreadSafeDeque<TaggedMessage>& messageDeque)
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
    async_read(
        socket_,
        buffer(&advertisedHeaderSizeFromClient_, 4),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            bytesFromClient_.resize(advertisedHeaderSizeFromClient_);
            handleRead(error, bytesTransferred);
        });
}

void TCPConnection::write(utils::Message messageForClient)
{
    messageForClient_ = std::move(messageForClient);
    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(&messageForClient_.header_, sizeof(utils::MessageHeader)),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            handleHeaderWrite(error, bytesTransferred);
        });
}

void TCPConnection::handleHeaderWrite(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(messageForClient_.body_),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            handleBodyWrite(error, bytesTransferred);
        });
}

void TCPConnection::handleBodyWrite(
    const boost::system::error_code& error, std::size_t bytesTransferred)
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
    const boost::system::error_code& error, std::size_t bytesTransferred)
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

    messageDeque_->push_back(TaggedMessage{, shared_from_this() });
}
