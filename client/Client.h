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
    void resolveHandler(
        const boost::system::error_code& ec,
        boost::asio::ip::tcp::resolver::iterator it);
    void connectHandler(const boost::system::error_code& ec);
    void readHandler(
        const boost::system::error_code& ec, std::size_t bytes_transferred);

    void handleConnection(boost::asio::ip::tcp::socket& socket);

private:
    boost::asio::io_context* iocontext_ = nullptr;
    std::string logFile_ = "web_client.log";
};

#endif// CLIENT_H
