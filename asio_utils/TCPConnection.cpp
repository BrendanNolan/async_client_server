#include "TCPConnection.h"

#include <cassert>
#include <iostream>
#include <utility>

#include "MessagePostFunctor.h"

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

TCPConnection::~TCPConnection()
{
}

void TCPConnection::send(Message message)
{
    std::scoped_lock lock{ outQMutex_ };
    outQ_.push_back(std::move(message));
    if (outQ_.size() != 1u)
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

void TCPConnection::writeHeader()
{
    assert(!outQ_.empty());

    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(&(outQ_.front()->header_), sizeof(MessageHeader)),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            if (outQ_.front()->body_.empty())
            {
                if (error)
                {
                    std::cout << "Could not write header" << std::endl;
                    std::cout << error.message() << std::endl;
                    return;
                }
                std::scoped_lock lock{ outQMutex_ };
                outQ_.try_pop_front();
                if (!outQ_.empty())
                    writeHeader();
                return;
            }

            writeBody();
        });
}

void TCPConnection::writeBody()
{
    assert(!outQ_.empty());

    auto self = shared_from_this();
    async_write(
        socket_,
        buffer(outQ_.front()->body_),
        [this, self](
            const boost::system::error_code& error,
            std::size_t bytesTransferred) {
            if (error)
            {
                std::cout << "Could not write body" << std::endl;
                std::cout << error.message() << std::endl;
                return;
            }
            std::scoped_lock lock{ outQMutex_ };
            outQ_.try_pop_front();
            if (!outQ_.empty())
                writeHeader();
        });
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
            if (error)
            {
                std::cout << "Could not read header" << std::endl;
                std::cout << error.message() << std::endl;
                return;
            }
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
            if (error)
            {
                std::cout << "Could not read body" << std::endl;
                std::cout << error.message() << std::endl;
                return;
            }
            const auto& post = *poster_;
            post(std::move(tempIncomingMessage_));
            tempIncomingMessage_ = Message{};
            readHeader();
        });
}

}// namespace utils
