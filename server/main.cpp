#include "Server.h"

#include <cstdlib>
#include <iostream>
#include <memory>

#include "DelayedMessageMirrorerFunctor.h"
#include "Message.h"

int main(int argc, char *argv[]) {
  if (argc != 2)
    return -1;

  try {
    Server server{ std::atoi(argv[1]) };
    server.setMessageProcessFunctor(std::make_unique<DelayedMessageMirrorerFunctor>());
  } catch (const std::exception &e) { std::cerr << e.what() << std::endl; }

  return 0;
}
