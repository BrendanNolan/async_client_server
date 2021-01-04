#include "Server.h"

#include <cstdlib>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "Message.h"
#include "DelayedMessageMirrorerFunctor.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
        return -1;

    try
    {
        boost::asio::io_context ioContext;
        Server server{ ioContext, std::atoi(argv[1]) };
        server.setMessageProcessFunctor(
            std::make_unique<DelayedMessageMirrorerFunctor>());

        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
