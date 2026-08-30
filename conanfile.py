import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class ChessRecipe(ConanFile):
    name = "chess"
    version = "1.0.0"
    package_type = "library"

    license = "MIT License"
    author = "Florian Giesemann <fgiesemann@proton.me>"
    url = "https://github.com/FGiesemann/Chess.git"
    homepage = "https://github.com/FGiesemann/Chess"

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_gui": [True, False],
    }
    default_options = {"shared": False, "fPIC": True, "with_gui": True}

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.23]")
        self.test_requires("catch2/3.7.1")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def export_sources(self):
        copy(
            self,
            "CMakeLists.txt",
            src=self.recipe_folder,
            dst=self.export_sources_folder,
        )
        copy(
            self,
            "libs/*",
            src=self.recipe_folder,
            dst=self.export_sources_folder,
            excludes=["**/test/**"],
        )
        copy(self, "apps/*", src=self.recipe_folder, dst=self.export_sources_folder)
        copy(self, "tools/*", src=self.recipe_folder, dst=self.export_sources_folder)
        copy(self, "cmake/*", src=self.recipe_folder, dst=self.export_sources_folder)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTING"] = False
        tc.variables["WITH_CHESS_GUI"] = self.options.with_gui
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "Chess")

        self.cpp_info.components["chess_core"].set_property(
            "cmake_target_name", "Chess::chess_core"
        )
        self.cpp_info.components["chess_core"].libs = ["chess_core"]

        self.cpp_info.components["chess_game"].set_property(
            "cmake_target_name", "Chess::chess_game"
        )
        self.cpp_info.components["chess_game"].libs = ["chess_game"]
        self.cpp_info.components["chess_game"].requires = ["chess_core"]

        self.cpp_info.components["chess_uci"].set_property(
            "cmake_target_name", "Chess::chess_uci"
        )
        self.cpp_info.components["chess_uci"].libs = ["chess_uci"]
        self.cpp_info.components["chess_uci"].requires = ["chess_core"]

        if self.options.with_gui:
            self.cpp_info.components["chess_gui"].set_property(
                "cmake_target_name", "Chess::chess_gui"
            )
            self.cpp_info.components["chess_gui"].libs = ["chess_gui"]
            self.cpp_info.components["chess_gui"].requires = ["chess_core"]

        self.cpp_info.components["chess_engine"].set_property(
            "cmake_target_name", "Chess::chess_engine"
        )
        self.cpp_info.components["chess_engine"].libs = ["chess_engine"]
        self.cpp_info.components["chess_engine"].requires = ["chess_core", "chess_uci"]
