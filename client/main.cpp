#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include <boost/asio.hpp>

#include "Client.h"
#include "PrintLogger.h"
#include "Message.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
        return -1;

    boost::asio::io_context iocontext;

    auto printLogger = std::make_shared<utils::PrintLogger>();

    std::vector<std::thread> threads;
    
    const auto threadCount = std::atoi(argv[1]);
    for (auto i = 0; i < threadCount; ++i)
    {
        threads.push_back(std::thread([&iocontext, printLogger, i]() {
            Client client(iocontext, std::move(printLogger));
            printLogger->log("Trying to connect Client #" + std::to_string(i));
            client.start();
            while (!client.connected())
            {
                printLogger->log(
                    "Not yet managed to connect Client #" + std::to_string(i));
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1s);
            }
            printLogger->log("Connected Client #" + std::to_string(i));
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
