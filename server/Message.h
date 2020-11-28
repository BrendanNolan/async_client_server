#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <memory>
#include <vector>

class TCPConnection;

struct Message
{
public:
    Message(
        std::uint32_t type,
        std::vector<std::uint8_t> body,
        std::shared_ptr<TCPConnection> connection);

    const std::uint32_t type_;
    const std::vector<std::uint8_t> body_;
    const std::shared_ptr<TCPConnection> connection_;
};

#endif// MESSAGE_H
