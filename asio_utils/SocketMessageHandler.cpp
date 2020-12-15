#include "SocketMessageHandler.h"

#include <utility>

using namespace boost::asio;
using namespace utils;

SocketMessageHandler::SocketMessageHandler(ip::tcp::socket socket)
    : socket_{ std::move(socket) }
{
}

void SocketMessageHandler::send(utils::Message message)
{
    std::lock_guard<std::mutex> lock{ sendMessageMutex_ };
    outgoingMessageQ_.emplace_back(std::move(message));

    if (outgoingMessageQ_.size() != 1u)
        return;
    grabNextOutgoingMessage();
    writeHeader();
}

void SocketMessageHandler::startReading()
{
    readHeader();
}

boost::asio::ip::tcp::socket& utils::SocketMessageHandler::socket()
{
    return socket_;
}

utils::ThreadSafeDeque<utils::Message>& SocketMessageHandler::incomingMessageQ()
{
    return incomingMessageQ_;
}

void SocketMessageHandler::writeHeader()
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

void SocketMessageHandler::writeBody()
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

void SocketMessageHandler::grabNextOutgoingMessage()
{
    tempOutgoingMessage_ = std::move(outgoingMessageQ_.front());
    outgoingMessageQ_.pop_front();
}

void SocketMessageHandler::readHeader()
{
    async_read(
        socket_,
        buffer(&tempIncomingMessage_.header_, sizeof(utils::MessageHeader)),
        [this](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            if (tempIncomingMessage_.header_.bodySize_ == 0u)
            {
                readHeader();
                return;
            }
            readBody();
        });
}

void SocketMessageHandler::readBody()
{
    resizeBodyAccordingToHeader(tempIncomingMessage_);
    async_read(
        socket_,
        buffer(tempIncomingMessage_.body_),
        [this](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            incomingMessageQ_.push_back(std::move(tempIncomingMessage_));
            tempIncomingMessage_ = utils::Message{};
            readHeader();
        });
}
