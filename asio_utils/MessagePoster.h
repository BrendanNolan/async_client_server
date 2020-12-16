#ifndef MESSAGEPOSTER_H
#define MESSAGEPOSTER_H

#include "Message.h"

class MessagePoster
{
public:
    virtual void post(utils::Message) const = 0;
};

#endif// MESSAGEPOSTER_H
