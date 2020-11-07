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
    std::cout << "Handling connection " << newConnection->id() << "..." << std::endl;
    if (!error)
        newConnection->start();
    
    startAccept();
}

void Server::startAccept()
{
    std::cout << "Waiting for new connection " << acceptCounter_ 
        << "..." << std::endl;
    auto newConnection = TCPConnection::create(acceptor_.get_io_service(), acceptCounter_);

    std::cout << "Asynchronously accepting connection " << acceptCounter_
        << "..." << std::endl;
    acceptor_.async_accept(
        newConnection->socket(),
        boost::bind(
            &Server::handleAccept,
            this,
            newConnection,
            boost::asio::placeholders::error));
    ++acceptCounter_;
}
