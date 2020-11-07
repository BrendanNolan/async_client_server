#include "TCPConnection.h"

#include <ctime>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace
{
std::string makeDaytimeString()
{
    const auto now = std::time(nullptr);
    return { std::ctime(&now) };
}
}

TCPConnection::TCPConnection(
    io_service& ioService)
    : socket_(ioService)
{
}

TCPConnection::pointer TCPConnection::create(
    io_service& ioService)
{
    return boost::make_shared<TCPConnection>(ioService);
}

socket& TCOConnection::socket()
{
    return socket_;
}

void TCPConnection::start()
{
    message_ = makeDaytimeString();
    async_write(
        socket_, 
        buffer(message_),
        boost::bind(
            &tcp_connection::handle_write, 
            shared_from_this(),
            placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::handle_write(
    const boost::system::error_code& /*error*/,
    size_t /*bytesTransferred*/)
{
}
