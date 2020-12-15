cmake_minimum_required(VERSION 3.18.4)
project(async_client_server)
#set(Boost_DEBUG ON)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(asio_utils)
add_subdirectory(utils)

