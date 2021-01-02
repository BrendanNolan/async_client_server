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

    std::thread runThread;

    {
        boost::asio::io_context::work{iocontext};
        runThread = std::thread{ [&iocontext]() {
            iocontext.run();
            std::cout << "ran\n";
        } };

        std::vector<std::thread> workerThreads;
        const auto threadCount = std::atoi(argv[1]);
        for (auto i = 0; i < threadCount; ++i)
        {
            workerThreads.push_back(std::thread([&iocontext, printLogger]() {
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

        for (auto& t : workerThreads)
            t.join();
    }

    runThread.join();
}
