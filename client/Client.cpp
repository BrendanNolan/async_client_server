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
    messageForServer_ = "Hello, I am the client.\n";
    async_write(
        socket_,
        buffer(messageForServer_),
        [this](
            const boost::system::error_code& error,
            std::size_t bytes_transferred) {
            handleWrite(error, bytes_transferred);
        });
    std::cout << "Kicked off asynchronously writing to socket.\n ";
}

void Client::handleWrite(
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

// void Client::writeHandler()
//{
//    while (moreToRead)
//    {
//        try
//        {
//            socket.read_some(buffer(bytes));
//            log("Socket read succeeded.\n", logFile_);
//        }
//        catch (const boost::system::system_error& error)
//        {
//            log(std::string("Socket read_some failed with error message: ")
//                    + std::string(error.what()) + '\n',
//                logFile_);
//            moreToRead = false;
//        }
//    }
//    log(bytes.data(), logFile_);
//}
