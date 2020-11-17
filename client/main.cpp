#include "Client.h"

#include <boost/asio.hpp>

int main()
{
    boost::asio::io_context iocontext;
    Client client(iocontext);
    client.start();
}
