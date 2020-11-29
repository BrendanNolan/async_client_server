#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <vector>

namespace utils
{
struct MessageHeader
{
    MessageHeader(std::uint32_t type, std::uint32_t size);

    const std::uint32_t type_;
    const std::uint32_t size_;
};

struct Message
{
public:
    Message(
        MessageHeader header, std::vector<std::uint8_t> body);

    const MessageHeader header_;
    const std::vector<std::uint8_t> body_;
};
}

#endif// MESSAGE_H
