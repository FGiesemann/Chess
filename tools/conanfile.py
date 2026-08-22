from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps


class ChessToolsConan(ConanFile):
    name = "ChessTools"
    version = "1.0.0"
    package_type = "application"

    settings = "os", "compiler", "build_type", "arch"

    requires = ["chesscore/1.0.0", "chessgame/1.0.0", "chessengine/1.0.0"]

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

    def package(self):
        self.output.info("No packaging defined for my_chess_app (example app).")
