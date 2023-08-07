# The source files for the tests
set(sourceFiles
"test_smoke.cpp"
)

find_package(Catch2 REQUIRED CONFIG)

# Add the tests executable
add_executable(test.context_core_api ${sourceFiles})
target_link_libraries(test.context_core_api PRIVATE context_core_api Catch2::Catch2WithMain)
target_include_directories(test.context_core_api PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Turn all warnings
target_compile_options(test.context_core_api PUBLIC
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)

# Discover the Catch2 test built by the application
include(CTest)
include(Catch)
catch_discover_tests(test.context_core_api)

