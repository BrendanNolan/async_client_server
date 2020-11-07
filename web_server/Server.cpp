#include "Server.h"

#include <iostream>
#include <functional>
#include <string>
#include <ctime>

#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

Server::Server(io_service& ioService)
    : ioService_{ ioService }
    , acceptor_{ ioService, tcp::endpoint{ tcp::v4(), 2014 } }
{
    startAccept();
}

void Server::handleAccept(
        tcpConnection::pointer newConnection,
        const boost::system::error_code& error)
{
    if (!error)
        newConnection->start();

    startAccept();
}

void Server::startAccept()
{
    auto newConnection = TCPConnection::create(ioService_);

    acceptor_.async_accept(
        newConnection->socket(),
        boost::bind(
            &tcp_server::handle_accept, 
            this, 
            newConnection,
            boost::asio::placeholders::error));
}
