cmake_minimum_required(VERSION 3.18.4)
project(utils_proj)
set(CMAKE_BUILD_TYPE Release)
 
file(GLOB SOURCES "*.cpp")
 
add_library(utils STATIC ${SOURCES})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(utils PUBLIC cxx_std_17)

