cmake_minimum_required(VERSION 3.18.4)
project(utils_proj)
 
add_library(utils INTERFACE)
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(utils INTERFACE cxx_std_17)

