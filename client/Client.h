#ifndef CLIENT_H
#define CLIENT_H

#include <cstdint>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

#include "Message.h"
#include "TCPConnection.h"

class Client
{
public:
    Client(boost::asio::io_context& iocontext);
    void start();

private:
    void handleResolve(
        const boost::system::error_code& error,
        boost::asio::ip::tcp::resolver::results_type results);

    void handleConnection(
        const boost::system::error_code& error,
        const boost::asio::ip::tcp::endpoint& endpoint);

private:
    boost::asio::ip::tcp::resolver resolver_;
    std::shared_ptr<utils::TCPConnection> connection_;
};

#endif// CLIENT_H
