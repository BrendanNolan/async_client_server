#include "SocketMessageHandler.h"

using namespace boost::asio;

SocketMessageHandler::SocketMessageHandler(ip::tcp::socket socket)
    : socket_{ std::move(socket) }
{
}

void SocketMessageHandler::send(utils::Message message)
{
    outgoingMessageQ_.push_back(std::move(message));
    writeHeader();
}

void SocketMessageHandler::startReading()
{
    auto self =
        shared_from_this();// See
                           // https://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp
    async_read(
        socket_,
        buffer(&tempIncomingMessage_.header_, sizeof(utils::MessageHeader)),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            handleHeaderRead(error, bytesTransferred);
        });
}

utils::ThreadSafeDeque<utils::Message>& SocketMessageHandler::incomingMessageQ()
{
    return incomingMessageQ_;
}

void SocketMessageHandler::writeHeader()
{
    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(&messageForClient_.header_, sizeof(utils::MessageHeader)),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            handleHeaderWrite(error, bytesTransferred);
        });
}

void SocketMessageHandler::handleHeaderRead(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void SocketMessageHandler::handleBodyRead(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void SocketMessageHandler::handleHeaderWrite(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void SocketMessageHandler::handleBodyWrite(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}
