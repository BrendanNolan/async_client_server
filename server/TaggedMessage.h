#ifndef TAGGEDMESSAGE_H
#define TAGGEDMESSAGE_H

#include <cstdint>
#include <memory>
#include <vector>

#include "Message.h"
#include "TCPConnection.h"

struct TaggedMessage {
public:
  TaggedMessage(utils::Message message, std::shared_ptr<utils::TCPConnection> connection);

  utils::Message message_;
  std::shared_ptr<utils::TCPConnection> connection_;
};

#endif// TAGGEDMESSAGE_H
