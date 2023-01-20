find_package(Boost 1.73.0 REQUIRED COMPONENTS
    system thread)

if (WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif (WIN32)

file (GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")
add_executable(server ${SOURCES} ${HEADERS})

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(server PRIVATE ${Boost_LIBRARIES})
target_link_libraries(server PRIVATE utils)
target_link_libraries(server PRIVATE asio_utils)

if (WIN32)
    target_link_libraries(server PRIVATE $ENV{BOOST_LIBRARYDIR}/libboost_date_time-vc141-mt-gd-x64-1_73.lib)
    target_link_libraries(server PRIVATE $ENV{BOOST_LIBRARYDIR}/libboost_regex-vc141-mt-gd-x64-1_73.lib)
endif (WIN32)
