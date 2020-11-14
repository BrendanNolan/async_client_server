#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include <memory>

#include "TCPConnection.h"

class Message
{
public:
    explicit Message(
        const std::string& text,
        std::shared_ptr<TCPConnection> connection);
    const std::string& text() const;

private:
    std::string text_;
    std::shared_ptr<TCPConnection> connection_;
};

#endif// MESSAGE_H
