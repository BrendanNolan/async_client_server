#include "TaggedMessage.h"

#include <utility>

using namespace utils;

TaggedMessage::TaggedMessage(Message message, std::shared_ptr<TCPConnection> connection)
  : message_{ std::move(message) }, connection_{ std::move(connection) } {}
