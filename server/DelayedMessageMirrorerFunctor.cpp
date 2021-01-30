#include "DelayedMessageMirrorerFunctor.h"

#include <chrono>
#include <thread>

utils::Message DelayedMessageMirrorerFunctor::operator()(const utils::Message& message) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(5s);

  return message;
}
