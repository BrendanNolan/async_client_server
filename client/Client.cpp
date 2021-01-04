#include "Client.h"

#include <boost/asio.hpp>
#include <chrono>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

#include "MessagePostFunctor.h"
#include "Logger.h"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace utils;

namespace
{
class ClientPostFunctor : public MessagePostFunctor
{
public:
    ClientPostFunctor(utils::Logger* logger)
        : logger_{ logger }
    {
    }

    void operator()(Message message) const override
    {
        if (!logger_)
            return;

        int i;
        message >> i;

        logger_->log("Received " + std::to_string(i));
    }

private:
    utils::Logger* logger_ = nullptr;
};

}// namespace

Client::Client(io_context& iocontext, std::shared_ptr<utils::Logger> logger)
    : connection_{ TCPConnection::create(iocontext) }
    , logger_{ std::move(logger) }
    , resolver_{ iocontext }
{
    connection_->setMessagePostFunctor(
        std::make_unique<ClientPostFunctor>(logger_.get()));
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
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(6s);
    {
        std::scoped_lock lock{ preConnectionMutex_ };

        if (!connectionEstablished_)
        {
            preConnectionMessageQ_.push_back(std::move(message));
            return;
        }
    }

    connection_->send(std::move(message));
}

void Client::handleResolve(
    const boost::system::error_code& error,
    boost::asio::ip::tcp::resolver::results_type results)
{
    if (error)
    {
        if (logger_)
            logger_->log("handleResolve(): " + error.message());
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
        if (logger_)
            logger_->log("handleConnection(): " + error.message());
        return;
    }
    std::scoped_lock lock{ preConnectionMutex_ };
    connectionEstablished_ = true;
    connection_->startReading();
    while (!preConnectionMessageQ_.empty())
    {
        if (auto message = preConnectionMessageQ_.try_pop_front())
            connection_->send(std::move(message.value()));
    }
}
