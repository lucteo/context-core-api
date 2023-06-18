from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class ContextCoreApiConan(ConanFile):
    name = "context_core_api"
    version = "1.0.0"

    # Optional metadata
    license = "Boost"
    author = "Lucian Radu Teodorescu"
    url = "https://github.com/lucteo/context-core-api"
    description = "Exposing Boost.Context core API into a standalone library"
    topics = ("coroutines", "api")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {}
    default_options = {}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def requirements(self):
        self.requires("boost/1.79.0")

    def config_options(self):
        pass

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["context_core_api"]

    # cpp_info.components["mycomp"].includedirs
    # cpp_info.components["mycomp"].libdirs

# from <root>/build/ directory, run:
#   > conan install .. --build=missing
#
# then:
#   > conan build ..
#
# publish and test the package with:
#   > conan export ..
#   > conan test ../test_package context_core_api/1.0.0 --build=missing
