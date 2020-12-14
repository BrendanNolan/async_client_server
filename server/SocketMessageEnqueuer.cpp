#include "SocketMessageEnqueuer.h"

#include "TCPConnection.h"

SocketMessageEnqueuer::SocketMessageEnqueuer(
    boost::asio::ip::tcp::socket& socket, TCPConnection* connection)
    : socket_{ &socket }
    , connection_{ connection }
{
}

void SocketMessageEnqueuer::write(utils::Message message) const
{
}

void SocketMessageEnqueuer::read() const
{
    auto self = connection_
        ? connection_
              ->shared_from_this();// See
                                   // https://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp
    async_read(
        socket_,
        buffer(&advertisedHeaderSizeFromClient_, 4),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            bytesFromClient_.resize(advertisedHeaderSizeFromClient_);
            handleRead(error, bytesTransferred);
        });
}

void SocketMessageEnqueuer::handleHeaderRead(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void SocketMessageEnqueuer::handleBodyRead(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void SocketMessageEnqueuer::handleHeaderWrite(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void SocketMessageEnqueuer::handleBodyWrite(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}
