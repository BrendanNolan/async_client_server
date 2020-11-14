#include "Message.h"

#include <utility>

Message::Message(
    const std::string& text,
    std::shared_ptr<TCPConnection> connection)
    : text_{text}
    , connection_{std::move(connection)}
{
}

const std::string& Message::text() const
{
    return text_;
}