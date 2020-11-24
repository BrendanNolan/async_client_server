#include "Server.h"

#include <iostream>
#include <functional>
#include <string>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace
{
void processMessage(const Message& message);
}

Server::Server(io_context& ioContext)
    : acceptor_{ ioContext, tcp::endpoint{ tcp::v4(), 2014 } }
    , ioContext_{ &ioContext }
{
    startAccept();
    for (auto i = 0; i < 4; ++i)
        threadPool_.emplace_back([this]() { processRequests(); });
}

void Server::handleAccept(
    TCPConnection::Pointer newConnection,
    const boost::system::error_code& error)
{
    if (!error)
        newConnection->start();

    startAccept();
}

void Server::startAccept()
{
    auto newConnection = TCPConnection::create(*ioContext_, messageDeque_);

    acceptor_.async_accept(
        newConnection->socket(),
        [this, newConnection](const boost::system::error_code& error) {
            handleAccept(newConnection, error);
        });
}

void Server::processRequests()
{
    while (true)
    {
        const auto data = messageDeque_.wait_and_pop_front();
        processMessage(data);
    }
}

namespace
{
void processMessage(const Message& message)
{
    if (auto connection = message.connection())
        connection->write("Hello, my name is server.");
}
}// namespace
