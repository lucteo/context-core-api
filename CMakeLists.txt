cmake_minimum_required(VERSION 3.15)
project(context_core_api CXX)

add_library(context_core_api src/context_core_api.cpp)
target_include_directories(context_core_api PUBLIC include)
target_compile_features(context_core_api PUBLIC cxx_std_17)

set_target_properties(context_core_api PROPERTIES PUBLIC_HEADER "include/context_core_api.h")
install(TARGETS context_core_api)
