import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import load
from pathlib import Path


class SLDMRecipe(ConanFile):
    name = "s-ldm"
    package_type = "application"

    # Optional metadata
    license = "GPL-2.0"
    author = "Francesco Raviglion francescorav.es483@gmail.com"
    url = "https://github.com/francescoraves483/S-LDM"
    description = "About S-LDM (Server Local Dynamic Map) - a highly-efficient, centralized Local Dynamic Map for V2X scenarios, developed in the context of the 5G-CARMEN project"
    topics = ("LDM", "C-ITS", "V2X")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    exports_sources = [
        "docs/*",
        "examples/*",
        "tests/*",
        "CMakeLists.txt",
    ]

    options = {
        "tests": [True, False],
        "docs": [True, False],
        "examples": [True, False]
    }

    default_options = {
        "tests": False,
        "docs": False,
        "examples": False
    }

    def set_version(self):
        self.version = load(self, os.path.join(self.recipe_folder, "VERSION.txt"))

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        #self.requires("amqp-cpp/4.3.24")
        self.requires("qpid-proton/0.39.0")
        self.requires("boost/1.83.0")
        self.requires("cpprestsdk/2.10.19")
        self.requires("geographiclib/2.3")
        self.requires("nlohmann_json/3.11.3")
        self.requires("log4cplus/2.1.1")
        self.requires("json-schema-validator/2.3.0")

    def configure(self):
        self.options['log4cplus'].unicode = False

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        # tc.user_presets_path = False

        tc.variables["ENABLE_TESTS"] = self.options.tests
        tc.variables["ENABLE_EXAMPLES"] = self.options.examples
        tc.variables["ENABLE_DOCS"] = self.options.docs

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()