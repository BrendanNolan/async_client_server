#include "Message.h"

#include <utility>

utils::MessageHeader::MessageHeader(std::uint32_t type, std::uint32_t size)
    : type_{ type }
    , bodySize_{ size }
{
}

utils::Message::Message(MessageHeader header, std::vector<std::uint8_t> body)
    : header_{ std::move(header) }
    , body_{ std::move(body) }
{
}

utils::Message& utils::operator<<(utils::Message& message, const std::string& data)
{
    for (const auto& c : data)
        message << c;
    return message;
}

void utils::updateHeader(Message& message)
{
    message.header_.bodySize_ =
        static_cast<std::uint32_t>(message.body_.size());
}

void utils::resizeBodyAccordingToHeader(Message& message)
{
    message.body_.resize(message.header_.bodySize_);
}
