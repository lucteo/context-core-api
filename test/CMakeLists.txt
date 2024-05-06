# The source files for the tests
set(sourceFiles
"test_context.cpp"
"test_c_compilation.c"
)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)


# Add the tests executable
add_executable(test.context_core_api ${sourceFiles})
target_link_libraries(test.context_core_api PRIVATE context_core_api Catch2::Catch2WithMain)
target_include_directories(test.context_core_api PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Turn on warning-as-error
set_property(TARGET test.context_core_api PROPERTY COMPILE_WARNING_AS_ERROR ON)

# Discover the Catch2 test built by the application
include(CTest)
include(Catch)
catch_discover_tests(test.context_core_api)

