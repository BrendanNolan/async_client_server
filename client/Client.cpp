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
std::mutex coutMutex;
class ClientPostFunctor : public MessagePostFunctor
{
public:
    void operator()(Message message) const override
    {
        std::lock_guard<std::mutex> lock{ coutMutex };
        std::cout << "Received a message" << std::endl;
    }
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
    connection_->send(std::move(message));
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
    connection_->startReading();
}
