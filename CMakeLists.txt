cmake_minimum_required(VERSION 3.11)

project(
	goot
	VERSION 0.0.1
	LANGUAGES CXX
)

add_library(compile_options INTERFACE)
target_compile_features(compile_options INTERFACE cxx_std_20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
include(3rd-party/cmake-conan/conan.cmake)

conan_cmake_configure(
REQUIRES
	openssl/1.1.1m
	libgit2/1.3.0
	qt/6.2.4
GENERATORS
	cmake_find_package
)

conan_cmake_autodetect(conan_settings)

conan_cmake_install(
	PATH_OR_REFERENCE .
	BUILD missing
	REMOTE conancenter
	SETTINGS ${conan_settings}
)

find_package(libgit2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_subdirectory(src)
