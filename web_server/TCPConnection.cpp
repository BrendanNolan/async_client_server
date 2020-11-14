#include "TCPConnection.h"

#include <ctime>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

TCPConnection::TCPConnection(
    io_service& ioService,
    ThreadSafeDeque<std::string>& messageDeque)
    : socket_{ioService}
    , messageDeque_{&messageDeque}
{
}

TCPConnection::pointer TCPConnection::create(
    io_service& ioService,
    ThreadSafeDeque<std::string>& messageDeque)
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
        buffer(messageFromClient_),
        boost::bind(
            &TCPConnection::handleRead,
            shared_from_this(),
            placeholders::error,
            placeholders::bytes_transferred));
    );
}

void TCPConnection::handleWrite(
    const boost::system::error_code& /*error*/, size_t /*bytesTransferred*/)
{
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
