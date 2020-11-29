#ifndef TAGGEDMESSAGE_H
#define TAGGEDMESSAGE_H

#include <cstdint>
#include <memory>
#include <vector>

class TCPConnection;

struct TaggedMessage
{
public:
    TaggedMessage(
        std::uint32_t type,
        std::vector<std::uint8_t> body,
        std::shared_ptr<TCPConnection> connection);

    const std::uint32_t type_;
    const std::vector<std::uint8_t> body_;
    const std::shared_ptr<TCPConnection> connection_;
};

#endif// TAGGEDMESSAGE_H