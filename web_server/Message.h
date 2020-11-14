#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include <boost/shared_ptr>

#include "TCPConnection.h"

class Message
{
public:
    explicit Message(
        const std::string& text,
        boost::shared_ptr<TCPConnection> connection);
    const std::string& text() const;

private:
    std::string text_;
    boost::shared_ptr<TCPConnection> connection_;
};

#endif// MESSAGE_H
