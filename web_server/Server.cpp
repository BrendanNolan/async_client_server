#include "Server.h"

#include <iostream>
#include <functional>
#include <string>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

Server::Server(io_context& ioContext)
    : acceptor_{ ioContext, tcp::endpoint{ tcp::v4(), 2014 } }
    , ioContext_{&ioContext}
{
    startAccept();
}

void Server::handleAccept(
    TCPConnection::pointer newConnection,
    const boost::system::error_code& error)
{
    if (!error)
        newConnection->start();

    startAccept();
}

void Server::startAccept()
{
    auto newConnection = TCPConnection::create(
        *ioContext_,
        messageDeque_);

    acceptor_.async_accept(
        newConnection->socket(),
        [this, newConnection](const boost::system::error_code& error) {
            handleAccept(newConnection, error);
        });
}
