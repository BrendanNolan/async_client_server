#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <boost/asio.hpp>

class Client
{
public:
    void start();

private:
    void resolve_handler(
        const boost::system::error_code& ec, tcp::resolver::iterator it);
    void connect_handler(const boost::system::error_code& ec);
    void read_handler(
        const boost::system::error_code& ec, std::size_t bytes_transferred);

private:
    boost::asio::io_service ioservice_;
    std::string logFile_ = "web_client.log";
};

#endif// CLIENT_H
