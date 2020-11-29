#include "Message.h"

#include <utility>

utils::MessageHeader::MessageHeader(std::uint32_t type, std::uint32_t size)
    : type_{ type }
    , size_{ size }
{
}

utils::Message::Message(
    const MessageHeader& header, std::vector<std::uint8_t> body)
    : header_{ header }
    , body_{ std::move(body) }
{
}