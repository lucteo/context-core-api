cmake_minimum_required(VERSION 3.24)
project(context_core_api CXX C)

find_package(Boost COMPONENTS context CONFIG REQUIRED)

add_library(context_core_api src/context_core_api.cpp)
target_include_directories(context_core_api PUBLIC include)
target_compile_features(context_core_api PUBLIC cxx_std_17)
target_link_libraries(context_core_api PUBLIC Boost::context)
set_target_properties(context_core_api PROPERTIES PUBLIC_HEADER "include/context_core_api.h")

# Turn on warning-as-error
set_property(TARGET context_core_api PROPERTY COMPILE_WARNING_AS_ERROR ON)

message(STATUS "With tests: ${WITH_TESTS}")
if(${WITH_TESTS})
     add_subdirectory(test)
endif()

install(TARGETS context_core_api)
