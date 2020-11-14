find_package(Boost 1.65.1 REQUIRED COMPONENTS
    system thread)

add_executable(client main.cpp Client.cpp ClientTCPConnection.cpp)
target_compile_features(client PUBLIC cxx_std_17)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(client PRIVATE ${Boost_LIBRARIES})
