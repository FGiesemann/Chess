from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
import os


class chesscoreRecipe(ConanFile):
    name = "chesscore"
    version = "1.0.0-dev"
    package_type = "library"

    license = "MIT License"
    author = "Florian Giesemann <florian.giesemann@gmail.com>"
    url = "https://github.com/FGiesemann/ChessCore.git"
    homepage = "https://github.com/FGiesemann/ChessCore"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
        "src/*",
        "include/*",
        "test/*",
        "LICENSE",
    )

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.15]")

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
        self.cpp.source.components["chesscore"].srcdirs = ["src/chesscore"]
        self.cpp.source.components["chesscore_io"].srcdirs = ["src/chesscore_io"]
        bt = "."
        self.cpp.build.components["chesscore"].libdirs = [bt]
        self.cpp.build.components["chesscore_io"].libdirs = [bt]

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
            self.run(os.path.join(test_folder, "chesscore_tests"))

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.components["chesscore"].libs = ["chesscore"]
        self.cpp_info.components["chesscore"].set_property(
            "cmake_target_name", "chesscore::chesscore"
        )

        self.cpp_info.components["chesscore_io"].libs = ["chesscore_io"]
        self.cpp_info.components["chesscore_io"].set_property(
            "cmake_target_name", "chesscore::chesscore_io"
        )

        self.cpp_info.components["chesscore_io"].requires = ["chesscore"]
