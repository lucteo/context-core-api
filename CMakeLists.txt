cmake_minimum_required(VERSION 3.24)
project(context_core_api CXX C)

include(FetchContent)

set(BOOST_INCLUDE_LIBRARIES context)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
  URL_MD5 893b5203b862eb9bbd08553e24ff146a
  DOWNLOAD_EXTRACT_TIMESTAMP ON
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(Boost)

add_library(context_core_api src/context_core_api.cpp)
target_include_directories(context_core_api PUBLIC include)
target_compile_features(context_core_api PUBLIC cxx_std_17)
target_link_libraries(context_core_api PUBLIC Boost::context)
set_target_properties(context_core_api PROPERTIES PUBLIC_HEADER "include/context_core_api.h")

# Turn on warning-as-error
set_property(TARGET context_core_api PROPERTY COMPILE_WARNING_AS_ERROR ON)

option(WITH_TESTS "Build the tests" OFF)
message(STATUS "With tests: ${WITH_TESTS}")
if(${WITH_TESTS})
     add_subdirectory(test)
endif()

install(TARGETS context_core_api)
