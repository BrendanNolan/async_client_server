#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <boost/asio.hpp>

class Client
{
public:
    void start();

private:
    void resolveHandler(
        const boost::system::error_code& ec,
        boost::asio::ip::tcp::resolver::iterator it);
    void connectHandler(const boost::system::error_code& ec);
    void readHandler(
        const boost::system::error_code& ec, std::size_t bytes_transferred);

private:
    boost::asio::io_service ioservice_;
    boost::asio::ip::tcp::socket socket_;
    std::string logFile_ = "web_client.log";
};

#endif// CLIENT_H
