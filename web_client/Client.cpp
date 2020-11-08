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

Client::Client()
{
    socket_.in
}

void log(const std::string& text, const std::string& logFile)
{
    std::ofstream ofs(logFile, std::ios_base::app);
    ofs << text;
}
}// namespace

void Client::start()
{
    tcp::resolver::query q{ "192.168.1.12", "2014" };
    tcp::resolver resolver{ ioservice_ };
    tcp::socket socket{ ioservice_ };
    resolver.async_resolve(
        q, 
        [this](const boost::system::error_code& ec, tcp::resolver::iterator it) {
            if (!ec)
                tcp_socket.async_connect(*it, connect_handler);
        });
    for (auto i = 0; i < 100; ++i)
    {
        log("\nCreating socket.\n", logFile_);
        tcp::socket socket(ioservice_);

        log("Connecting socket.\n", logFile_);
        boost::asio::connect(socket, it);

        log("Handling connection.\n", logFile_);
        handleConnection(socket);
    }
}

void Client::resolveHandler(
    const boost::system::error_code& ec, tcp::resolver::iterator it)
{

}

void connectHandler(const boost::system::error_code& ec)
{

}

void readHandler(
    const boost::system::error_code& ec, std::size_t bytes_transferred)
{
    
}

void Client::handleConnection(tcp::socket& socket)
{
    std::array<char, 4096> bytes;
    auto moreToRead = true;
    while (moreToRead)
    {
        try
        {
            socket.read_some(buffer(bytes));
            log("Socket read succeeded.\n", logFile_);
        }
        catch (const boost::system::system_error& error)
        {
            log(std::string("Socket read_some failed with error message: ")
                    + std::string(error.what()) + '\n',
                logFile_);
            moreToRead = false;
        }
    }
    log(bytes.data(), logFile_);
}
