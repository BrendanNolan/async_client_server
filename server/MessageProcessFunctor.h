#ifndef MESSAGEPROCESSFUNCTOR_H
#define MESSAGEPROCESSFUNCTOR_H

#include "Message.h"

class MessageProcessFunctor
{
public:
  virtual ~MessageProcessFunctor() = default;

  virtual utils::Message operator()(const utils::Message &message) = 0;
};

#endif// MESSAGEPROCESSFUNCTOR_H
