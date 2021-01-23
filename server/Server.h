#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <string>
#include <thread>

#include <boost/asio.hpp>

#include "TaggedMessage.h"
#include "ThreadSafeDeque.h"

class MessageProcessFunctor;

namespace utils {
class TCPConnection;
}

class Server {
public:
  Server(int workerCount);
  ~Server();

  void setMessageProcessFunctor(std::unique_ptr<MessageProcessFunctor> functor);

private:
  void startAccept();
  void handleAccept(std::shared_ptr<utils::TCPConnection> newConnection, const boost::system::error_code &error);

private:
  void processRequests();

private:
  boost::asio::io_context        ioContext_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::thread                    contextRunThread_;// May not need to run this in a thread.

  std::vector<std::thread>              workerPool_;
  utils::ThreadSafeDeque<TaggedMessage> messageDeque_;

  std::unique_ptr<MessageProcessFunctor> messageProcessFunctor_;
};

#endif// SERVER_H
