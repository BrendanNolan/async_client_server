#include <utility>

#include <boost/asio.hpp>

#include "Client.h"
#include "Message.h"

int main()
{
    boost::asio::io_context iocontext;
    Client client(iocontext);
    client.start();

    for (auto i = 0; i < 10; ++i)
    {
        utils::Message message;
        message << std::string{ "Hello " } << i;
        client.send(std::move(message));
    }
    iocontext.run();
}
