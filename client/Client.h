#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <boost/asio.hpp>

class Client
{
public:
    Client(boost::asio::io_context& iocontext);
    void start();

private:
    void handleConnection(const boost::system::error_code& error);
    void handleRead(
        const boost::system::error_code& error, std::size_t bytes_transferred);
    void handleResolve(
        const boost::system::error_code& error,
        boost::asio::ip::tcp::resolver::results_type results);

    void handleConnection(
        const boost::system::error_code& error,
        const boost::asio::ip::tcp::endpoint& endpoint);

    void handleWrite(
        const boost::system::error_code& error, std::size_t bytes_transferred);

private:
    boost::asio::io_context* iocontext_ = nullptr;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_;
    std::string messageForServer_;
    std::array<char, 256> messageFromServer_;
};

#endif// CLIENT_H
