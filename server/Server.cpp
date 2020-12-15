#include "Server.h"

#include <cstdint>
#include <ctime>
#include <iostream>
#include <functional>
#include <string>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace
{
void processMessage(const TaggedMessage& message);
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
std::vector<std::uint8_t> prepareOutgoingBody(const TaggedMessage& message)
{
    const std::string str{ "Hello, my name is server." };
    return std::vector<std::uint8_t>(str.begin(), str.end());
}

void processMessage(const TaggedMessage& message)
{
    if (!message.connection_)
        return;

    auto outgoingBody = prepareOutgoingBody(message);
    std::vector<std::uint8_t> outgoingBytes;
    outgoingBytes.reserve(4u + outgoingBody.size());
    std::move(
        outgoingBody.begin(),
        outgoingBody.end(),
        std::back_inserter(outgoingBytes));

    message.connection_->write(outgoingBytes);
}
}// namespace
