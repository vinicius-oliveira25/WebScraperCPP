from conan import ConanFile
from conan.tools.gnu import PkgConfigDeps
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout

class WebScraperConan(ConanFile):
    name = "WebScraper"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    requires = "beauty/1.0.4", "lexbor/2.3.0"
    generators = "CMakeDeps", "CMakeToolchain", "PkgConfigDeps"

    def layout(self):
        cmake_layout(self)