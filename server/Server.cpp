#include "Server.h"

#include <cstdint>
#include <ctime>
#include <iostream>
#include <functional>
#include <string>
#include <utility>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "MessagePoster.h"
#include "TCPConnection.h"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace utils;

namespace
{
void processMessage(const TaggedMessage& message);

class TaggedMessageEnquer : public MessagePoster
{
public:
    TaggedMessageEnquer(
        TCPConnection& messageSource,
        ThreadSafeDeque<TaggedMessage>& targetQ)
        : messageSource_{ &messageSource }
        , targetQ_{ &targetQ }
    {
    }

    void post(Message message) const override
    {
        targetQ_->push_back(
            { std::move(message), messageSource_->shared_from_this() });
    }

private:
    TCPConnection* messageSource_;
    ThreadSafeDeque<TaggedMessage>* targetQ_ = nullptr;
};
}// namespace

Server::Server(io_context& ioContext)
    : acceptor_{ ioContext, tcp::endpoint{ tcp::v4(), 2014 } }
    , ioContext_{ &ioContext }
{
    startAccept();
    for (auto i = 0; i < 4; ++i)
        threadPool_.emplace_back([this]() { processRequests(); });
}

void Server::handleAccept(
    std::shared_ptr<TCPConnection> newConnection,
    const boost::system::error_code& error)
{
    if (!error)
        newConnection->startReading();

    startAccept();
}

void Server::startAccept()
{
    auto newConnection = TCPConnection::create(*ioContext_);
    newConnection->setMessagePoster(std::make_unique<TaggedMessageEnquer>(
        *newConnection, messageDeque_));

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

void processMessage(const TaggedMessage& taggedMessage)
{
    if (!taggedMessage.connection_)
        return;

    auto outgoingBody = prepareOutgoingBody(taggedMessage);
    std::vector<std::uint8_t> outgoingBytes;
    outgoingBytes.reserve(4u + outgoingBody.size());
    std::move(
        outgoingBody.begin(),
        outgoingBody.end(),
        std::back_inserter(outgoingBytes));

    taggedMessage.connection_->send(taggedMessage.message_);
}
}// namespace
