#include "Server.h"

#include <iostream>
#include <functional>
#include <string>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

Server::Server(io_service& ioService)
    : acceptor_{ ioService, tcp::endpoint{ tcp::v4(), 2014 } }
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
    auto newConnection = TCPConnection::create(acceptor_.get_io_service());

    acceptor_.async_accept(
        newConnection->socket(),
        boost::bind(
            &Server::handleAccept,
            this,
            newConnection,
            boost::asio::placeholders::error));
}
