#include "Server.h"

#include <iostream>
#include <thread>

#include <boost/asio.hpp>

int main()
{
    try
    {
        boost::asio::io_service ioService;
        Server server{ ioService };
        
        std::thread thread0([&ioService] { ioService.run(); });
        std::thread thread1([&ioService] { ioService.run(); });
        std::thread thread2([&ioService] { ioService.run(); });
        std::thread thread3([&ioService] { ioService.run(); });
        std::thread thread4([&ioService] { ioService.run(); });

        thread0.join();
        thread1.join();
        thread2.join();
        thread3.join();
        thread4.join();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
