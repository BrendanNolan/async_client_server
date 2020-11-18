cmake_minimum_required(VERSION 3.18.4)
project(subdirs_proj)
set(CMAKE_BUILD_TYPE Release)
#set(Boost_DEBUG ON)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(utils)
