#ifndef ERRORNOTIFYFUNCTOR_H
#define ERRORNOTIFYFUNCTOR_H

#include <boost/asio.hpp>

namespace utils {
class ErrorNotifyFunctor {
public:
  virtual ~ErrorNotifyFunctor() = default;

  virtual void operator()(const boost::system::error_code &ec) const = 0;
};
}// namespace utils

#endif// ERRORNOTIFYFUNCTOR_H
