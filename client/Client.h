#ifndef CLIENT_H
#define CLIENT_H

#include <cstdint>
#include <vector>

#include <boost/asio.hpp>

#include "Message.h"

class Client
{
public:
    Client(boost::asio::io_context& iocontext);
    void start();

private:
    void handleConnection(const boost::system::error_code& error);

    void write();

    void handleRead(
        const boost::system::error_code& error, std::size_t bytes_transferred);
    void handleResolve(
        const boost::system::error_code& error,
        boost::asio::ip::tcp::resolver::results_type results);

    void handleConnection(
        const boost::system::error_code& error,
        const boost::asio::ip::tcp::endpoint& endpoint);

    void handleHeaderWrite(
        const boost::system::error_code& error, std::size_t bytes_transferred);
    void handleBodyWrite(
        const boost::system::error_code& error, std::size_t bytes_transferred);

private:
    boost::asio::io_context* iocontext_ = nullptr;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_;
    utils::Message messageForServer_;
    std::vector<std::uint8_t> messageFromServer_;
};

#endif// CLIENT_H
