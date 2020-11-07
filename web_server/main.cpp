#include "Server.h"

#include <iostream>

#include <boost/asio/io_service.hpp>

int main()
{
    try
    {
        boost::asio::io_service ioService;
        Server server{ ioService };

        ioService.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
