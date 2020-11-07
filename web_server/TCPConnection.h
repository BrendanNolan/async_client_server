#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <string>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>

class TCPConnection : 
    public boost::enable_shared_from_this<TCPConnection>
{
private:
    TCPConnection(boost::asio::io_service& ioService);

public:
    using pointer = boost::shared_ptr<TCPConnection>;

    static pointer create(boost::asio::io_service& ioService);

    boost::asio::ip::tcp::socket& socket();

    void start();

private:
    void handle_write(
        const boost::system::error_code& error,
        size_t bytesTransferred);

private:
    boost::asio::ip::tcp::socket socket_;
    std::string message_;
};

#endif// TCPCONNECTION_H
