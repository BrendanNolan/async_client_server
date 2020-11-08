#include "TCPConnection.h"

#include <ctime>
#include <chrono>
#include <iostream>
#include <thread>

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
    std::string s(100000, 'T');
    return s;
}
}// namespace

TCPConnection::TCPConnection(io_service& ioService, int id)
    : socket_(ioService)
    , id_{ id }
{
}

TCPConnection::pointer TCPConnection::create(io_service& ioService, int id)
{
    return pointer{ new TCPConnection{ ioService, id } };
}

tcp::socket& TCPConnection::socket()
{
    return socket_;
}

int TCPConnection::id() const
{
    return id_;
}

void TCPConnection::start()
{
    message_ = makeDaytimeString();
    async_write(
        socket_,
        buffer(message_),
        boost::bind(
            &TCPConnection::handleWrite,
            shared_from_this(),
            placeholders::error,
            placeholders::bytes_transferred));
}

void TCPConnection::handleWrite(
    const boost::system::error_code& /*error*/, size_t /*bytesTransferred*/)
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10s);
    std::cout << "Connection " << id_ << " finished writing. Thread "
              << std::this_thread::get_id() << std::endl;
}
