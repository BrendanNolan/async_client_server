#include "Server.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <iostream>
#include <string>
#include <ctime>

using namespace boost::asio;
using namespace boost::asio::ip;

Server::Server(io_context& ioContext)
    : ioContext_{ ioContext }
    , acceptor_{ ioContext, tcp::endpoint(tcp::v4(), 2014) }
{
}

void Server::handleConnection(tcp::socket& socket)
{
    auto now = std::time(nullptr);
    std::string data{ std::ctime(&now) };
    write(socket, buffer(data));
    socket.shutdown(tcp::socket::shutdown_send);
}

void Server::start()
{
    tcp::endpoint endpoint{ tcp::v4(), 2014 };
    tcp::acceptor acceptor{ ioservice_, endpoint };
    while (true)
    {
        acceptor.listen();
        tcp::socket socket{ ioservice_ };
        acceptor.accept(socket);
        handleConnection(socket);
    }
}
