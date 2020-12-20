#include <chrono>
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

    while (!client.connectionEstablished())
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    std::vector<std::string> messages = { 
        "Dirty", 
        "old",  
        "river",
        ",",     
        "must", 
        "you", 
        "keep",  
        "rolling",
        "rolling", 
        "on"
        };

    for (const auto& msg : messages)
    {
        utils::Message message;
        message << msg;
        client.send(std::move(message));
    }

    thread.join();
}
