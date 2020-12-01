#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <vector>

namespace utils
{
struct MessageHeader
{
    MessageHeader(std::uint32_t type, std::uint32_t size);

    std::uint32_t type_;
    std::uint32_t size_;
};

struct Message
{
public:
    Message(
        MessageHeader header, std::vector<std::uint8_t> body);

    MessageHeader header_;
    std::vector<std::uint8_t> body_;
};

template <typename PODType>
Message& operator<<(Message& message, const PODType& data)
{

}

template<typename PODType>
Message& operator>>(Message& message, PODType& data)
{

}

}

#endif// MESSAGE_H
