#ifndef MESSAGE_H
#define MESSAGE_H

#include <algorithm>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace utils
{
struct MessageHeader
{
    MessageHeader() = default;
    MessageHeader(std::uint32_t type, std::uint32_t size);

    std::uint32_t type_ = static_cast<std::uint32_t>(0);
    std::uint32_t bodySize_ = static_cast<std::uint32_t>(0);
};

struct Message
{
public:
    Message() = default;
    Message(MessageHeader header, std::vector<std::uint8_t> body);

    MessageHeader header_;
    std::vector<std::uint8_t> body_;
};
void updateHeader(Message& message);
void resizeBodyAccordingToHeader(Message& message);

template<typename PODType>
Message& operator<<(Message& message, const PODType& data)
{
    static_assert(
        std::is_standard_layout<PODType>::value,
        "Data type not trivially serialisable");

    const auto oldBodySize = message.body_.size();
    message.body_.resize(oldBodySize + sizeof(PODType));

    std::memcpy(message.body_.data() + oldBodySize, &data, sizeof(PODType));

    updateHeader(message);

    return message;
}

template<typename PODType> Message& operator>>(Message& message, PODType& data)
{
    std::memcpy(
        &data,
        message.body_.data() + message.body_.size() - sizeof(PODType),
        sizeof(PODType))

        message.body_.resize(message.body_.size() - sizeof(PODType));

    updateHeader(message);

    return message;
}

}// namespace utils

#endif// MESSAGE_H
