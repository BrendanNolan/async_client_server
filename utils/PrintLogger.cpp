#include "PrintLogger.h"

#include <iostream>

namespace utils {

void PrintLogger::log(const std::string &text) {
  std::scoped_lock lock{ coutMutex_ };
  std::cout << text << std::endl;
}

}// namespace utils
