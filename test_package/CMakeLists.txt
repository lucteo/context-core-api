cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(context_core_api CONFIG REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example context_core_api::context_core_api)
