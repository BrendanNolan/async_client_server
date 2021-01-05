#ifndef CLIENT_H
#define CLIENT_H

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

#include "Message.h"
#include "TCPConnection.h"
#include "ThreadSafeDeque.h"

namespace utils
{
class Logger;
}

class Client
{
public:
    Client(
        std::shared_ptr<utils::Logger> logger);
    ~Client();

    void connect(const std::string& host, const int port);
    void send(utils::Message message);

    bool connectionBroken() const;

private:
    void handleResolve(
        const boost::system::error_code& error,
        boost::asio::ip::tcp::resolver::results_type results);

    void handleConnection(
        const boost::system::error_code& error,
        const boost::asio::ip::tcp::endpoint& endpoint);

private:
    boost::asio::ip::tcp::resolver resolver_;
    std::shared_ptr<utils::TCPConnection> connection_;

    std::shared_ptr<utils::Logger> logger_;

    mutable std::mutex preConnectionMutex_;
    utils::ThreadSafeDeque<utils::Message> preConnectionMessageQ_;
    bool connectionEstablished_ = false;

    std::atomic<bool> connectionBroken_;

    boost::asio::io_context iocontext_;
    std::thread contextThread_;
};

#endif// CLIENT_H
