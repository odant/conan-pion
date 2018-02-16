project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

#find_package(websocketpp REQUIRED)
#find_package(Boost REQUIRED COMPONENTS
#    system
#)
#find_package(Threads REQUIRED)

enable_testing()

add_executable(test_custom_plugin test_custom_plugin.cpp)
target_link_libraries(test_custom_plugin
    CONAN_PKG::pion
#    Boost::system
#    Threads::Threads
)
set_target_properties(test_custom_plugin
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_custom_plugin
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
    COMMAND test_custom_plugin
)

add_executable(test_plugin test_plugin.cpp)
target_link_libraries(test_plugin
    CONAN_PKG::pion
#    Boost::system
#    Threads::Threads
)
set_target_properties(test_plugin
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_plugin
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
    COMMAND test_plugin
)
