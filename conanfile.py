from conan import ConanFile


class Discotec(ConanFile):
    generators = ("CMakeToolchain", "CMakeDeps")
    settings = ("os", "build_type", "arch", "compiler")

    def requirements(self):
        #self.requires("qt/[>=6.5 <6.6]")
        self.requires("audiofile/1.1.1")
        self.requires("benchmark/1.8.4")
        self.requires("brotli/1.1.0", override=True)
        self.requires("fmt/10.1.1")
        self.requires("gtest/1.14.0")
        self.requires("ms-gsl/4.0.0")
        self.requires("qt/6.5.2")
        self.requires("raylib/5.0")
        self.requires("unordered_dense/4.4.0")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.25]")

    def configure(self):
        self.options["qt/*"].shared = False
        self.options["qt/*"].qtmultimedia = True

    def layout(self):
        self.folders.generators = ""
