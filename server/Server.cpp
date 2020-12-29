#include "Server.h"

#include <cstdint>
#include <ctime>
#include <iostream>
#include <functional>
#include <mutex>
#include <string>
#include <utility>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "MessagePostFunctor.h"
#include "TCPConnection.h"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace utils;

namespace
{
void processMessage(const TaggedMessage& message);

class TaggedMessagePostFunctor : public MessagePostFunctor
{
public:
    TaggedMessagePostFunctor(
        TCPConnection& messageSource, ThreadSafeDeque<TaggedMessage>& targetQ)
        : messageSource_{ &messageSource }
        , targetQ_{ &targetQ }
    {
    }

    void operator()(Message message) const override
    {
        auto copy = message;
        auto i = -1;
        copy >> i;
        if (i % 5000 == 0)
        {
            std::cout << "Received " << i << " via " << messageSource_
                      << std::endl;
        }

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
    newConnection->setMessagePostFunctor(
        std::make_unique<TaggedMessagePostFunctor>(
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

void processMessage(const TaggedMessage& taggedMessage)
{
    if (!taggedMessage.connection_)
        return;

    auto copy = taggedMessage.message_;
    auto i = -1;
    copy >> i;
    if (i % 5000 == 0)
    {
        std::cout << "Sending " << i << " via "
                  << taggedMessage.connection_.get() << std::endl;
    }

    taggedMessage.connection_->send(taggedMessage.message_);
}

}// namespace
