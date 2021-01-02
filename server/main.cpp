#include "Server.h"

#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "Message.h"
#include "MessageProcessFunctor.h"

namespace
{
    class MessageMirrorerFunctor : public MessageProcessFunctor
    {
        virtual utils::Message operator()(const utils::Message& message) override
        {
            return message;
        }
    };
}

int main()
{
    try
    {
        boost::asio::io_context ioContext;
        Server server{ ioContext };
        server.setMessageProcessFunctor(std::make_unique<MessageMirrorerFunctor>());

        ioContext.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
