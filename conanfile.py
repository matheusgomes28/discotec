from conan import ConanFile
from conan.tools.cmake import cmake_layout


class Discotec(ConanFile):
    generators = ("CMakeToolchain", "CMakeDeps")
    settings = ("os", "build_type", "arch", "compiler")

    def requirements(self):
        self.requires("brotli/1.1.0", override=True)
        self.requires("fmt/10.1.1")
        self.requires("qt/6.5.2")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.25]")

    def configure(self):
        self.options["qt/*"].shared = True

    def layout(self):
        cmake_layout(self)
