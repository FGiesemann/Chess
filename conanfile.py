import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout


class ChessRecipe(ConanFile):
    name = "chess"
    version = "1.0.0"
    package_type = "library"

    license = "MIT License"
    author = "Florian Giesemann <fgiesemann@proton.me>"
    url = "https://github.com/FGiesemann/Chess.git"
    homepage = "https://github.com/FGiesemann/Chess"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.23]")

    def requirements(self):
        self.test_requires("catch2/3.7.1")

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
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if not self.conf.get("tools.build:skip_test", default=False):
            test_folder = os.path.join("test")
            if self.settings.os == "Windows":
                test_folder = os.path.join("test", str(self.settings.build_type))
            self.run(os.path.join(test_folder, "chess_core_tests"))

    def package(self):
        cmake = CMake(self)
        cmake.install()
