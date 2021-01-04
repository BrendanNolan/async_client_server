#ifndef DELAYEDMESSAGEMIRRORFUNCTOR_H
#define DELAYEDMESSAGEMIRRORFUNCTOR_H

#include "Message.h"
#include "MessageProcessFunctor.h"

class DelayedMessageMirrorerFunctor : public MessageProcessFunctor
{
public:
    utils::Message operator()(const utils::Message& message) override;
};

#endif // DELAYEDMESSAGEMIRRORFUNCTOR_H
