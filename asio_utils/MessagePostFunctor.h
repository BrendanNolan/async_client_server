#ifndef MESSAGEPOSTFUNCTOR_H
#define MESSAGEPOSTFUNCTOR_H

#include "Message.h"

namespace utils {

class MessagePostFunctor {
public:
  virtual ~MessagePostFunctor()                         = default;
  virtual void operator()(utils::Message message) const = 0;
};

}// namespace utils
#endif// MESSAGEPOSTFUNCTOR_H
