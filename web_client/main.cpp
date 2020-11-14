#include "Client.h"

#include <boost/asio.hpp>

int main()
{
    boost::asio::io_service ioservice;
    Client client(ioservice);
    client.start();
}
