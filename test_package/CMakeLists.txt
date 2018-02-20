project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS
    filesystem
    regex
    thread
    system
)
find_package(Pion REQUIRED COMPONENTS services)

enable_testing()

add_executable(test_custom_plugin test_custom_plugin.cpp)
target_link_libraries(test_custom_plugin
    Pion::pion
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
    Pion::services
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
