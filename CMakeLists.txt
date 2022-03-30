cmake_minimum_required(VERSION 3.11)

project(
	goot
	VERSION 0.0.1
	LANGUAGES CXX
)

add_library(compile_options INTERFACE)
target_compile_features(compile_options INTERFACE cxx_std_20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(src)
