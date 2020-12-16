#ifndef SOCKETCONNECTOR_H
#define SOCKETCONNECTOR_H

#include <boost/asio.hpp>

class SocketConnector
{
public:
    virtual void connect(boost::asio::ip::tcp::socket& socket) const = 0;
};

#endif// SOCKETCONNECTOR_H
