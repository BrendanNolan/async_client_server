#include "Server.h"

#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <utility>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "Message.h"
#include "MessagePostFunctor.h"
#include "MessageProcessFunctor.h"
#include "TCPConnection.h"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace utils;

namespace {

class TaggedMessagePostFunctor : public MessagePostFunctor {
public:
  TaggedMessagePostFunctor(TCPConnection& messageSource, ThreadSafeDeque<TaggedMessage>& targetQ)
    : messageSource_{ &messageSource }, targetQ_{ &targetQ } {}

  void operator()(Message message) const override {
    targetQ_->push_back({ std::move(message), messageSource_->shared_from_this() });
  }

private:
  TCPConnection* messageSource_;
  ThreadSafeDeque<TaggedMessage>* targetQ_ = nullptr;
};

}// namespace

Server::Server(int workerCount) : acceptor_{ ioContext_, tcp::endpoint{ tcp::v4(), 2014 } } {
  startAccept();
  for (auto i = 0; i < workerCount; ++i)
    workerPool_.emplace_back([this]() { processRequests(); });

  contextRunThread_ = std::thread{ [this]() { ioContext_.run(); } };
}

Server::~Server() {
  if (contextRunThread_.joinable())
    contextRunThread_.join();
}

void Server::setMessageProcessFunctor(std::unique_ptr<MessageProcessFunctor> functor) {
  messageProcessFunctor_ = std::move(functor);
}

void Server::handleAccept(std::shared_ptr<TCPConnection> newConnection,
  const boost::system::error_code& error) {
  if (!error)
    newConnection->startReading();

  startAccept();
}

void Server::startAccept() {
  auto newConnection = TCPConnection::create(ioContext_);
  newConnection->setMessagePostFunctor(
    std::make_unique<TaggedMessagePostFunctor>(*newConnection, messageDeque_));

  acceptor_.async_accept(
    newConnection->socket(), [this, newConnection](const boost::system::error_code& error) {
      handleAccept(newConnection, error);
    });
}

void Server::processRequests() {
  while (true) {
    const auto taggedMessageToProcess = messageDeque_.wait_and_pop_front();
    auto& processMessage = *messageProcessFunctor_;

    taggedMessageToProcess.connection_->send(processMessage(taggedMessageToProcess.message_));
  }
}
