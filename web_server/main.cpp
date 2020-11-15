#include "Server.h"

#include <iostream>

#include <boost/asio.hpp>

int main()
{
    try
    {
        boost::asio::io_context ioContext;
        Server server{ ioContext };

        ioContext.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
