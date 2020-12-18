#include "TCPConnection.h"

#include <utility>

using namespace boost::asio;

namespace utils
{

TCPConnection::TCPConnection(io_context& ioContext)
    : socket_{ ioContext }
{
}

std::shared_ptr<TCPConnection> TCPConnection::create(
    boost::asio::io_context& ioContext)
{
    return std::shared_ptr<TCPConnection>{ new TCPConnection{ ioContext } };
}

void TCPConnection::send(Message message)
{
    std::lock_guard<std::mutex> lock{ sendMessageMutex_ };
    outgoingMessageQ_.emplace_back(std::move(message));

    if (outgoingMessageQ_.size() != 1u)
        return;
    writeHeader();
}

void TCPConnection::startReading()
{
    readHeader();
}

boost::asio::ip::tcp::socket& TCPConnection::socket()
{
    return socket_;
}

void TCPConnection::setMessagePostFunctor(
    std::unique_ptr<MessagePostFunctor> poster)
{
    poster_ = std::move(poster);
}

void TCPConnection::hasMessageToSend() const
{
    return !outgoingMessageQ_.empty();
}

void TCPConnection::writeHeader()
{
    if (!hasMessageToSend())
	return;
    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(outgoingMessage().header_, sizeof(MessageHeader)),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            if (outgoingMessage().body_.empty())
            {
                outgoingMessageQ_.pop_front();
                writeHeader();
                return;
            }

            writeBody();
        });
}

void TCPConnection::writeBody()
{
    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(outgoingMessage().body_),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            std::lock_guard<std::mutex> lock{ sendMessageMutex_ };

            if (outgoingMessageQ_.empty())
                return;
            outgoingMessageQ_.pop_front();
            writeHeader();
        });
}

const Message& TCPConnection::outgoingMessage()
{
    return outgoingMessageQ_.front();
}

void TCPConnection::readHeader()
{
    auto self = shared_from_this();
    async_read(
        socket_,
        buffer(&tempIncomingMessage_.header_, sizeof(MessageHeader)),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            if (tempIncomingMessage_.header_.bodySize_ == 0u)
            {
                const auto& post = *poster_;
                post(std::move(tempIncomingMessage_));
                tempIncomingMessage_ = Message{};
                readHeader();
                return;
            }
            readBody();
        });
}

void TCPConnection::readBody()
{
    resizeBodyAccordingToHeader(tempIncomingMessage_);
    auto self = shared_from_this();
    async_read(
        socket_,
        buffer(tempIncomingMessage_.body_),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            const auto& post = *poster_;
            post(std::move(tempIncomingMessage_));
            tempIncomingMessage_ = Message{};
            readHeader();
        });
}

}// namespace utils
