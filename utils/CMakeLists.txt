cmake_minimum_required(VERSION 3.18.4)
project(utils_proj)

file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")
 
add_library(utils STATIC ${SOURCES} ${HEADERS})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

