#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>

#include "Client.h"
#include "Message.h"
#include "MessagePostFunctor.h"
#include "PrintLogger.h"

namespace {

class ClientPostFunctor : public utils::MessagePostFunctor {
public:
  ClientPostFunctor(utils::Logger *logger) : logger_{ logger } {}

  void operator()(utils::Message message) const override {
    if (!logger_)
      return;

    int i;
    message >> i;

    logger_->log("Received " + std::to_string(i));
  }

private:
  utils::Logger *logger_ = nullptr;
};

}// namespace

int main() {
  Client client{ std::make_unique<utils::PrintLogger>() };
  client.setMessagePostFunctor(std::make_unique<ClientPostFunctor>(client.logger()));
  client.connect("192.168.1.12", 2014);

  for (auto i = 0; i < 10000000; ++i) {
    if (client.connectionBroken())
      break;

    utils::Message message;
    message << i;
    client.send(std::move(message));
  }
}
