cmake_minimum_required(VERSION 3.18.4)
project(subdirs_proj)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(web_client)
add_subdirectory(web_server)
add_subdirectory(utils)
