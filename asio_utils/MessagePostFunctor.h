#ifndef MESSAGEPOSTFUNCTOR_H
#define MESSAGEPOSTFUNCTOR_H

#include "Message.h"

class MessagePostFunctor
{
public:
    virtual ~MessagePostFunctor() = default;
    virtual void operator()(utils::Message message) const = 0;
};

#endif// MESSAGEPOSTFUNCTOR_H
