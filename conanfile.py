from conans import ConanFile, CMake, tools


class PionConan(ConanFile):
    name = "pion"
    version = "5.0.7"
    license = "Boost License - https://raw.githubusercontent.com/splunk/pion/develop/COPYING"
    description = "C++ framework for building lightweight HTTP interfaces"
    url = "https://github.com/odant/conan-pion"
    settings = {
        "os": ["Windows", "Linux"],
        "compiler": ["Visual Studio", "gcc"],
        "build_type": ["Debug", "Release"],
        "arch": ["x86_64", "x86"]
    }
    generators = "cmake"
    exports_sources = "src/*", "CMakeLists.txt", "FindPion.cmake"
    no_copy_source = True
    build_policy = "missing"

    def configure(self):
        # Only C++11
        if "libcxx" in self.settings.compiler.fields:
            if self.settings.compiler.libcxx == "libstdc++":
                raise Exception("This package is only compatible with libstdc++11")

    def requirements(self):
        self.requires("zlib/[~=1.2.11]@%s/stable" % self.user)
        #self.requires("openssl/[~=1.1.0g]@%s/testing" % self.user)
        self.requires("boost/1.66.0@%s/testing" % self.user)

    def build(self):
        cmake = CMake(self)
        #
        cmake.definitions["CMAKE_INSTALL_PREFIX:STRING"] = self.package_folder.replace("\\", "/")
        cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE:BOOL"] = "ON"
        cmake.definitions["BUILD_SHARED_LIBS:BOOL"] = "OFF"
        #
        cmake.definitions["BUILD_SPDY:BOOL"] = "OFF"
        cmake.definitions["BUILD_UT:BOOL"] = "OFF"
        cmake.definitions["BUILD_PIOND:BOOL"] = "OFF"
        cmake.definitions["BUILD_HELLOSERVER:BOOL"] = "OFF"
        cmake.definitions["USE_LOG4CPLUS:BOOL"] = "OFF"
        cmake.definitions["USE_LOG4CXX:BOOL"] = "OFF"
        cmake.definitions["USE_LOG4CPP:BOOL"] = "OFF"
        cmake.definitions["DISABLE_LOGGING:BOOL"] = "ON"
        #
        cmake.configure()
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("FindPion.cmake", src=".", dst=".")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.defines = ["PION_STATIC_LINKING"]
