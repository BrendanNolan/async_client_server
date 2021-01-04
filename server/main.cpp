#include "Server.h"

#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "Message.h"
#include "DelayedMessageMirrorerFunctor.h"

int main()
{
    try
    {
        boost::asio::io_context ioContext;
        Server server{ ioContext };
        server.setMessageProcessFunctor(
            std::make_unique<DelayedMessageMirrorerFunctor>());

        ioContext.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
