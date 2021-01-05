#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>

#include "Client.h"
#include "Message.h"
#include "PrintLogger.h"

int main()
{
    Client client{ std::make_shared<utils::PrintLogger>() };
    client.connect("192.168.1.12", 2014);

    for (auto i = 0; i < 10000000; ++i)
    {
        if (client.connectionBroken())
            break;

        utils::Message message;
        message << i;
        client.send(std::move(message));
    }
}
