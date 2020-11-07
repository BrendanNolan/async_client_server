#include "TCPConnection.h"

#include <ctime>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace
{
std::string makeDaytimeString()
{
    const auto now = std::time(nullptr);
    return { std::ctime(&now) };
}
}// namespace

TCPConnection::TCPConnection(io_service& ioService)
    : socket_(ioService)
{
}

TCPConnection::pointer TCPConnection::create(io_service& ioService)
{
    return pointer{ new TCPConnection{ ioService } };
}

tcp::socket& TCPConnection::socket()
{
    return socket_;
}

void TCPConnection::start()
{
    message_ = makeDaytimeString();
    boost::asio::async_write(
        socket_,
        buffer(message_),
        boost::bind(
            &TCPConnection::handle_write,
            shared_from_this(),
            placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::handleWrite(
    const boost::system::error_code& /*error*/, size_t /*bytesTransferred*/)
{
}
