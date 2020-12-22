#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

#include <boost/asio.hpp>

#include "Client.h"
#include "Message.h"

int main()
{
    boost::asio::io_context iocontext;
    
    Client client(iocontext);
    client.start();

    std::thread thread{ [&iocontext]() { iocontext.run(); } };

    for (auto i = 0; i < 10; ++i)
    {
        utils::Message message;
        message << i;
        client.send(std::move(message));
    }

    thread.join();
}
