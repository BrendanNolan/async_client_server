from conans import ConanFile, CMake

class async_client_server(ConanFile):
    name = "async_client_server"
    requires = "boost/1.73.0"
    generators = "cmake_find_package"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_EXPORT_COMPILE_COMMANDS"] = 1
        cmake.configure()
        cmake.build()

