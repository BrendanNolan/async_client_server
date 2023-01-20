find_package(Boost 1.73.0 REQUIRED COMPONENTS
    system thread)
project(asio_utils_project)

if (WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif (WIN32)

file (GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")
add_library(asio_utils STATIC ${SOURCES} ${HEADERS})

target_compile_features(asio_utils PUBLIC cxx_std_17)
target_include_directories(asio_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(asio_utils PRIVATE ${Boost_LIBRARIES})
target_link_libraries(asio_utils PRIVATE utils)

if (WIN32)
    target_link_libraries(asio_utils PRIVATE $ENV{BOOST_LIBRARYDIR}/libboost_date_time-vc141-mt-gd-x64-1_73.lib)
    target_link_libraries(asio_utils PRIVATE $ENV{BOOST_LIBRARYDIR}/libboost_regex-vc141-mt-gd-x64-1_73.lib)
endif (WIN32)

