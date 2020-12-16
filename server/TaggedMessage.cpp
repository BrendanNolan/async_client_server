#include "TaggedMessage.h"

#include <utility>

TaggedMessage::TaggedMessage(
    utils::Message message, std::shared_ptr<TCPConnection> connection)
    : message_{ std::move(message) }
    , connection_{ std::move(connection) }
{
}
