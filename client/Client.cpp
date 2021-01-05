#include "Client.h"

#include <boost/asio.hpp>
#include <chrono>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

#include "MessagePostFunctor.h"
#include "ErrorNotifyFunctor.h"
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

class SetBrokenConnectionFlagFunctor : public ErrorNotifyFunctor
{
public:
    SetBrokenConnectionFlagFunctor(std::atomic<bool>& flag)
        : flag_{ &flag }
    {
    }

    void operator()(const boost::system::error_code& error) const override
    {
        *flag_ = true;
    }

private:
    std::atomic<bool>* flag_;
};

}// namespace

Client::Client(std::shared_ptr<utils::Logger> logger)
    : connection_{ TCPConnection::create(iocontext_) }
    , logger_{ std::move(logger) }
    , resolver_{ iocontext_ }
{
    connection_->setMessagePostFunctor(
        std::make_unique<ClientPostFunctor>(logger_.get()));
    connection_->setErrorNotifyFunctor(
        std::make_unique<SetBrokenConnectionFlagFunctor>(connectionBroken_));

    contextThread_ = std::thread{ [this]() { iocontext_.run(); } };
}

Client::~Client()
{
    iocontext_.stop();
    
    if (contextThread_.joinable())
        contextThread_.join();
}

void Client::connect(const std::string& host, const int port)
{
    resolver_.async_resolve(
        host,
        std::to_string(port),
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

bool Client::connectionBroken() const
{
    return connectionBroken_;
}

void Client::handleResolve(
    const boost::system::error_code& error,
    boost::asio::ip::tcp::resolver::results_type results)
{
    if (error)
    {
        if (logger_)
            logger_->log("handleResolve(): " + error.message());
        connectionBroken_ = true;
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
        connectionBroken_ = true;
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
