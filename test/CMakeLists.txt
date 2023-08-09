# The source files for the tests
set(sourceFiles
"test_context.cpp"
)

find_package(Catch2 REQUIRED CONFIG)

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

