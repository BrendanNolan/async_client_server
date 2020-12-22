#ifndef CLIENT_H
#define CLIENT_H

#include <atomic>
#include <cstdint>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

#include "ThreadSafeDeque.h"
#include "Message.h"
#include "TCPConnection.h"

// There is probably a race condition about queueing message up before 
// connection established.
class Client
{
public:
    Client(boost::asio::io_context& iocontext);
    void start();
    void send(utils::Message message);

    bool connectionEstablished() const;

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
    utils::ThreadSafeDeque<utils::Message> preConnectionMessageQ_;
    std::atomic<bool> connectionEstablished_ = false;
};

#endif// CLIENT_H
