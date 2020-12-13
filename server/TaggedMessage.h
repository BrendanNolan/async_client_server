#ifndef TAGGEDMESSAGE_H
#define TAGGEDMESSAGE_H

#include <cstdint>
#include <memory>
#include <vector>

#include "Message.h"

class TCPConnection;

struct TaggedMessage
{
public:
    TaggedMessage(
        utils::Message message, std::shared_ptr<TCPConnection> connection);

    utils::Message message_;
    std::shared_ptr<TCPConnection> connection_;
};

#endif// TAGGEDMESSAGE_H
