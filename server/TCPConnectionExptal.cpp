#include "TCPConnectionExptal.h"

using namespace boost::asio;

TCPConnectionExptal::TCPConnectionExptal(ip::tcp::socket socket)
    : socket_{ std::move(socket) }
{
}

void TCPConnectionExptal::send(utils::Message message)
{
    outgoingMessageQ_.push_back(std::move(message));
    writeHeader();
}

void TCPConnectionExptal::startReading()
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

utils::ThreadSafeDeque<utils::Message>& TCPConnectionExptal::incomingMessageQ()
{
    return incomingMessageQ_;
}

void TCPConnectionExptal::writeHeader()
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

void TCPConnectionExptal::handleHeaderRead(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void TCPConnectionExptal::handleBodyRead(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void TCPConnectionExptal::handleHeaderWrite(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}

void TCPConnectionExptal::handleBodyWrite(
    const boost::system::error_code& error, std::size_t bytesTransferred)
{
}
