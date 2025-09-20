from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps


class ChessGuiConan(ConanFile):
    name = "chessgui"
    version = "1.0.0"
    package_type = "library"

    license = "MIT License"
    author = "Florian Giesemann <florian.giesemann@gmail.com>"
    url = "https://github.com/FGiesemann/ChessCore.git"
    homepage = "https://github.com/FGiesemann/ChessGui"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
        "src/*",
        "include/*",
        "LICENSE",
    )

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.20]")

    def requirements(self):
        self.requires("chesscore/1.0.0")
        self.requires("qt/6.8.3")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure_qt(self):
        self.options["qt"].qtbase = True
        self.options["qt"].qtsvg = True
        self.options["qt"].qtgui = True
        self.options["qt"].qtwidgets = True
        self.options["qt"].qtgraphicsview = True

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        self.configure_qt()

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
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["ChessGui"]
        self.cpp_info.set_property("cmake_target_name", "ChessGui::ChessGui")
