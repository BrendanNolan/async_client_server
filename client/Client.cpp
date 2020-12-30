#include "Client.h"

#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>

#include "MessagePostFunctor.h"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace utils;

namespace
{
class ClientPostFunctor : public MessagePostFunctor
{
public:
    void operator()(Message message) const override
    {
        std::scoped_lock lock{ coutMutex_ };

        int i = -1;
        message >> i;
        if (i == 0 || (i % 5000 == 0))
            std::cout << "Received " << i << std::endl;
    }

private:
    mutable std::mutex coutMutex_;
};

}// namespace

Client::Client(io_context& iocontext)
    : connection_{ TCPConnection::create(iocontext) }
    , resolver_{ iocontext }
{
    connection_->setMessagePostFunctor(std::make_unique<ClientPostFunctor>());
}

void Client::start()
{
    resolver_.async_resolve(
        "192.168.1.12",
        "2014",
        [this](
            const boost::system::error_code& error,
            boost::asio::ip::tcp::resolver::results_type results) {
            handleResolve(error, results);
        });
}

void Client::send(utils::Message message)
{
    std::scoped_lock lock{ preConnectionMutex_ };

    if (!connectionEstablished_)
    {
        preConnectionMessageQ_.push_back(std::move(message));
        return;
    }

    connection_->send(std::move(message));
}

bool Client::connectionEstablished() const
{
    std::scoped_lock lock{ preConnectionMutex_ };
    return connectionEstablished_;
}

void Client::handleResolve(
    const boost::system::error_code& error,
    boost::asio::ip::tcp::resolver::results_type results)
{
    if (error)
    {
        std::cout << "handleResolve(): " << error.message() << std::endl;
        return;
    }
    async_connect(
        connection_->socket(),
        results,
        [this](
            const boost::system::error_code& error,
            const boost::asio::ip::tcp::endpoint& endpoint) {
            handleConnection(error, endpoint);
        });
}

void Client::handleConnection(
    const boost::system::error_code& error, const tcp::endpoint& endpoint)
{
    if (error)
    {
        std::cout << "handleConnection(): " << error.message() << std::endl;
        return;
    }
    connectionEstablished_ = true;
    connection_->startReading();
    while (!preConnectionMessageQ_.empty())
    {
        if (auto message = preConnectionMessageQ_.try_pop_front())
            send(std::move(message.value()));
    }
}
