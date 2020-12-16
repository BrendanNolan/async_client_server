#include "TCPConnection.h"

#include <utility>

using namespace boost::asio;
using namespace utils;

TCPConnection::TCPConnection(io_context& ioContext)
    : socket_{ ioContext }
{
}

std::shared_ptr<TCPConnection> utils::TCPConnection::create(
    boost::asio::io_context& ioContext)
{
    return std::shared_ptr<TCPConnection>{ new TCPConnection{ ioContext } };
}

void TCPConnection::send(utils::Message message)
{
    std::lock_guard<std::mutex> lock{ sendMessageMutex_ };
    outgoingMessageQ_.emplace_back(std::move(message));

    if (outgoingMessageQ_.size() != 1u)
        return;
    grabNextOutgoingMessage();
    writeHeader();
}

void TCPConnection::startReading()
{
    readHeader();
}

boost::asio::ip::tcp::socket& utils::TCPConnection::socket()
{
    return socket_;
}

void utils::TCPConnection::setMessagePoster(
    std::unique_ptr<MessagePoster> poster)
{
    poster_ = std::move(poster);
}

void TCPConnection::writeHeader()
{
    async_write(
        socket_,
        buffer(&tempOutgoingMessage_.header_, sizeof(utils::MessageHeader)),
        [this](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            if (tempOutgoingMessage_.body_.empty())
            {
                writeHeader();
                return;
            }

            writeBody();
        });
}

void TCPConnection::writeBody()
{
    async_write(
        socket_,
        buffer(tempOutgoingMessage_.body_),
        [this](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            std::lock_guard<std::mutex> lock{ sendMessageMutex_ };

            if (outgoingMessageQ_.empty())
                return;
            grabNextOutgoingMessage();
            writeHeader();
        });
}

void TCPConnection::grabNextOutgoingMessage()
{
    tempOutgoingMessage_ = std::move(outgoingMessageQ_.front());
    outgoingMessageQ_.pop_front();
}

void TCPConnection::readHeader()
{
    async_read(
        socket_,
        buffer(&tempIncomingMessage_.header_, sizeof(utils::MessageHeader)),
        [this](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            if (tempIncomingMessage_.header_.bodySize_ == 0u)
            {
                poster_->post(std::move(tempIncomingMessage_));
                tempIncomingMessage_ = utils::Message{};
                readHeader();
                return;
            }
            readBody();
        });
}

void TCPConnection::readBody()
{
    resizeBodyAccordingToHeader(tempIncomingMessage_);
    async_read(
        socket_,
        buffer(tempIncomingMessage_.body_),
        [this](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            poster_->post(std::move(tempIncomingMessage_));
            tempIncomingMessage_ = utils::Message{};
            readHeader();
        });
}
