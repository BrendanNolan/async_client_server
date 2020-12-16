#include "Client.h"

#include <boost/asio.hpp>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "MessagePoster.h"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace utils;

namespace
{

void emptyLogFile(const std::string& logFile)
{
    std::ofstream ofs;
    ofs.open(logFile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void log(const std::string& text, const std::string& /*logFile*/)
{
    std::cout << text << '\n';
}

class ClientPoster : public MessagePoster
{
public:
    void post(Message message) const override
    { 
        for (const auto& byte : message.body_)
            std::cout << byte;
    }
};

}// namespace

Client::Client(io_context& iocontext)
    : connection_{ TCPConnection::create(iocontext) }
    , resolver_{ iocontext }
{
    connection_->setMessagePoster(std::make_unique<ClientPoster>());
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
