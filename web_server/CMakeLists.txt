find_package(Boost 1.65.1 REQUIRED COMPONENTS
    system thread)

add_executable(server main.cpp Server.cpp TCPConnection.cpp)
target_compile_features(server PUBLIC cxx_std_17)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(server PRIVATE ${Boost_LIBRARIES})
target_link_libraries(server PRIVATE utils)

