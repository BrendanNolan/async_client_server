#include "TaggedMessage.h"

#include <utility>

#include "TCPConnection.h"

TaggedMessage::TaggedMessage(
    std::uint32_t type,
    std::vector<std::uint8_t> body,
    std::shared_ptr<TCPConnection> connection)
    : type_{ type }
    , body_{ std::move(body) }
    , connection_{ std::move(connection) }
{
}
