#include <utility>

#include <boost/asio.hpp>

#include "Client.h"
#include "Message.h"

int main()
{
    boost::asio::io_context iocontext;
    Client client(iocontext);
    client.start();
    for (auto i = 0; i < 100; ++i)
    {
        utils::Message message;
        message << i;
        client.send(std::move(message));
    }
    iocontext.run();
}
