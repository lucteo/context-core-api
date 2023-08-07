cmake_minimum_required(VERSION 3.15)
project(context_core_api CXX)

find_package(Boost COMPONENTS context CONFIG REQUIRED)

add_library(context_core_api src/context_core_api.cpp)
target_include_directories(context_core_api PUBLIC include)
target_compile_features(context_core_api PUBLIC cxx_std_17)
target_link_libraries(context_core_api PUBLIC Boost::context)
set_target_properties(context_core_api PROPERTIES PUBLIC_HEADER "include/context_core_api.h")

# Turn all warnings
target_compile_options(context_core_api PUBLIC
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)

message(STATUS "With tests: ${WITH_TESTS}")
if(${WITH_TESTS})
     add_subdirectory(test)
endif()

install(TARGETS context_core_api)
