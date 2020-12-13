#include "Client.h"

#include <boost/asio.hpp>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace boost::asio;
using namespace boost::asio::ip;

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
}// namespace

Client::Client(io_context& iocontext)
    : iocontext_{ &iocontext }
    , socket_{ iocontext }
    , resolver_{ *iocontext_ }
{
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

void Client::handleRead(
    const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (error)
    {
        std::cout << "handleRead(): " << error.message() << std::endl;
        return;
    }
    std::cout << "Received " << bytes_transferred
              << " from server; here they are: \n"
              << std::string(
                     messageFromServer_.begin(),
                     messageFromServer_.begin() + bytes_transferred)
              << std::endl;
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
        socket_,
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
    std::cout << "About to kick off asynchronously writing to socket.\n";
    std::string str{ "Hello, I am the client.\n" };
    messageForServer_.header_.type_ = 0;
    messageForServer_ << str;
    write();
}

void Client::write()
{
    async_write(
        socket_,
        buffer(&messageForServer_.header_, sizeof(utils::MessageHeader)),
        [this](
            const boost::system::error_code& error,
            std::size_t bytes_transferred) {
            handleHeaderWrite(error, bytes_transferred);
        });
}

void Client::handleHeaderWrite(
    const boost::system::error_code& error, std::size_t bytes_transferred)
{
    async_write(
        socket_,
        buffer(messageForServer_.body_),
        [this](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            handleBodyWrite(error, bytesTransferred);
        });
}

void Client::handleBodyWrite(
    const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (error)
    {
        std::cout << "handleWrite(): " << error.message() << std::endl;
        return;
    }
    socket_.async_read_some(// Hack to make reading work without filling buffer.
        buffer(messageFromServer_),
        [this](
            const boost::system::error_code& ec,
            std::size_t bytes_transferred) {
            handleRead(ec, bytes_transferred);
        });
}
