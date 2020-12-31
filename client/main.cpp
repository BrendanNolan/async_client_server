#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include <boost/asio.hpp>

#include "Client.h"
#include "PrintLogger.h"
#include "Message.h"

int main()
{
    boost::asio::io_context iocontext;

    auto printLogger = std::make_shared<utils::PrintLogger>();

    std::vector<std::thread> threads;

    for (auto i = 0; i < 20; ++i)
    {
        threads.push_back(
            std::thread([&iocontext, printLogger]() {
                Client client(iocontext, std::move(printLogger));
                client.start();        
                for (auto i = 0; i < 10000000; ++i)
                {
                    utils::Message message;
                    message << i;
                    client.send(std::move(message));
                }
            }));
        
    }

    std::thread thread{ [&iocontext]() { iocontext.run(); } };

    for (auto& t : threads)
        t.join();

    thread.join();
}
