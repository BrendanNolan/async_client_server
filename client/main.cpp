#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

#include <boost/asio.hpp>

#include "Client.h"
#include "Message.h"

void waitForConnection(const Client& client)
{
    if (!client.connectionEstablished())
        std::cout << "Connecting...";
    while (!client.connectionEstablished())
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(0.01s);
        std::cout << '.';
    }
    std::cout << std::endl;
}

int main()
{
    boost::asio::io_context iocontext;
    
    Client client(iocontext);
    client.start();

    std::thread thread{ [&iocontext]() { iocontext.run(); } };

    waitForConnection(client);

    for (auto i = 0; i < 10; ++i)
    {
        utils::Message message;
        message << i;
        client.send(std::move(message));
    }

    thread.join();
}
