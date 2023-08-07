from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd

class ContextCoreApiRecipe(ConanFile):
    name = "context_core_api"
    version = "1.0.0"
    package_type = "library"

    # Optional metadata
    license = "Boost"
    author = "Lucian Radu Teodorescu"
    url = "https://github.com/lucteo/context-core-api"
    description = "Exposing Boost.Context core API into a standalone library"
    topics = ("coroutines", "api")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "with_tests": [True, False]}
    default_options = {"shared": False, "fPIC": True, "with_tests": False}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def validate(self):
        check_min_cppstd(self, "17")
        
    def requirements(self):
        self.requires("boost/1.80.0")
        self.requires("catch2/3.4.0")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["WITH_TESTS"] = self.options.with_tests
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

# from <root>/build/ directory, run:
#   > conan install .. --build=missing -s compiler.cppstd=17 -o with_tests=True
#
# then:
#   > conan build .. -s compiler.cppstd=17
#
# publish and test the package with:
#   > conan export ..
#   > conan test ../test_package context_core_api/1.0.0 --build=missing
#
# or, to run everything in one go:
#   > conan create .. --build=missing -s compiler.cppstd=17
#
# Note: changing `with_tests` value requires deleting the temporary build files.
