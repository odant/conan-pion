from conan import ConanFile, tools
import os

class PionConan(ConanFile):
    name = "pion"
    version = "5.0.7+19"
    license = "Boost Software License 1.0 - https://raw.githubusercontent.com/splunk/pion/develop/COPYING"
    description = "C++ framework for building lightweight HTTP interfaces"
    url = "https://github.com/odant/conan-pion"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "ninja": [True, False]
    }
    default_options = {
        "ninja": True
    }
    exports_sources = "src/*", "fix_cmake_version.patch", "remove_multipart_parse.patch"
    no_copy_source = True
    build_policy = "missing"
    package_type = "static-library"
    
    def layout(self):
        tools.cmake.cmake_layout(self, src_folder="src")

    def configure(self):
        # Only C++11
        if "libcxx" in self.settings.compiler.fields:
            if self.settings.compiler.libcxx == "libstdc++":
                raise Exception("This package is only compatible with libstdc++11")

    def build_requirements(self):
        if self.options.ninja:
            self.tool_requires("ninja/[>=1.12.1]")

    def requirements(self):
        self.requires("zlib-ng/[>=2.2.4]@%s/stable" % self.user)
        self.requires("openssl/[>=3.0.16]@%s/stable" % self.user)
        self.requires("boost/[>=1.70.0]@%s/testing" % self.user)
        
    def source(self):    
        tools.files.patch(self, patch_file="fix_cmake_version.patch")
        tools.files.patch(self, patch_file="remove_multipart_parse.patch")
        
    def generate(self):
        benv = tools.env.VirtualBuildEnv(self)
        benv.generate()
        renv = tools.env.VirtualRunEnv(self)
        renv.generate()
        if tools.microsoft.is_msvc(self):
            vc = tools.microsoft.VCVars(self)
            vc.generate()
        deps = tools.cmake.CMakeDeps(self)    
        deps.set_property("openssl", "cmake_additional_variables_prefixes", ["OPENSSL"])
        deps.generate()
        cmakeGenerator = "Ninja" if self.options.ninja else None
        tc = tools.cmake.CMakeToolchain(self, generator=cmakeGenerator)
        #
        tc.variables["CMAKE_INSTALL_PREFIX"] = self.package_folder.replace("\\", "/")
        tc.variables["CMAKE_POSITION_INDEPENDENT_CODE"] = "ON"
        tc.variables["BUILD_SHARED_LIBS"] = "OFF"
        #
        tc.variables["BUILD_SPDY"] = "OFF"
        tc.variables["BUILD_UT"] = "OFF"
        tc.variables["BUILD_PIOND"] = "OFF"
        tc.variables["BUILD_HELLOSERVER"] = "OFF"
        tc.variables["USE_LOG4CPLUS"] = "OFF"
        tc.variables["USE_LOG4CXX"] = "OFF"
        tc.variables["USE_LOG4CPP"] = "OFF"
        tc.variables["DISABLE_LOGGING"] = "ON"
        if self.settings.os == "Windows":
            tc.preprocessor_definitions["_WIN32_WINNT"] = "0x0601"
            tc.preprocessor_definitions["WINVER"]       = "0x0601"
        #tc.cache_variables["CMAKE_POLICY_VERSION_MINIMUM"] = "3.5"
        tc.generate()

    def build(self):
        cmake = tools.cmake.CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = tools.cmake.CMake(self)
        cmake.install()
        tools.files.copy(self, "**/pion*.pdb", dst=os.path.join(self.package_folder, "bin"), src=self.build_folder, keep_path=False)

    def package_id(self):
        self.info.requires["boost"].full_package_mode()

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", "Pion")
        self.cpp_info.set_property("cmake_target_name", "")
        #
        self.cpp_info.components["pion"].libs = ["pion"]
        self.cpp_info.components["pion"].set_property("cmake_target_name", "Pion::pion")
        self.cpp_info.components["pion"].requires = ["openssl::openssl", "zlib-ng::zlib-ng", "boost::filesystem", "boost::regex", "boost::thread", "boost::system"]
        if self.settings.os == "Linux":
            self.cpp_info.components["pion"].system_libs.append("dl")
        self.cpp_info.components["pion"].defines = ["PION_STATIC_LINKING"]
        #
        self.cpp_info.components["services"].libs = ["pion_services"]
        self.cpp_info.components["services"].set_property("cmake_target_name", "Pion::services")
        self.cpp_info.components["services"].requires = ["pion"]
