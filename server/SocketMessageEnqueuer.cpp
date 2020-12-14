#include "SocketMessageEnqueuer.h"

#include "TCPConnection.h"

using namespace boost::asio;

SocketMessageEnqueuer::SocketMessageEnqueuer(
    ip::tcp::socket& socket, TCPConnection* connection)
    : socket_{ &socket }
    , connection_{ connection }
{
}

void SocketMessageEnqueuer::write(utils::Message message)
{
}

void SocketMessageEnqueuer::read()
{
    auto self = connection_ ? 
        connection_->shared_from_this() :
        std::shared_ptr<TCPConnection>{}; // https://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp
    
    async_read(
        socket_,
        buffer(&tempIncomingMessage_.header_, sizeof(utils::MessageHeader)),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) { 
            handleHeaderRead(error, bytesTransferred);
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
