import os

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import can_run
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.env import Environment, VirtualBuildEnv, VirtualRunEnv
from conan.tools.files import apply_conandata_patches, copy, export_conandata_patches, get, rmdir, rm
from conan.tools.layout import basic_layout
from conan.tools.scm import Version

required_conan_version = ">=1.53.0"

class QpidProtonConan(ConanFile):
    name = 'qpid-proton'
    version = '0.39.0'
    package_type = "library"

    # Optional metadata
    license = "https://github.com/apache/qpid-proton/blob/master/LICENSE"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "fPIC": [True, False],
        "shared": [True, False],
        "include_pdbs": [True, False],
        "tests": [True, False]
    }

    default_options = {
        "fPIC": True,
        "shared": False,
        "include_pdbs": False,
        "tests": False
    }

    sources = "https://github.com/apache/qpid-proton.git"
    source_dir = "qpid-proton"

    exports_sources = [
        "CMakeLists.txt"
    ]

    def layout(self):
        cmake_layout(self, src_folder="src")

    def requirements(self):
        self.requires("openssl/3.2.1")

    def source(self):
        get(self, **self.conan_data["sources"][self.version], strip_root=True)

    def generate(self):
        tc = CMakeToolchain(self)
        #tc.user_presets_path = False
        tc.generate()
        tc.cache_variables["BUILD_EXAMPLES"] = False
        tc.cache_variables["BUILD_TESTING"] = False
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.libdirs = ['lib64']
        self.cpp_info.resdir = ['share']
        self.cpp_info.libs = [
            'qpid-proton-core',
            'qpid-proton-cpp',
            'qpid-proton-proactor',
            'qpid-proton'
        ]