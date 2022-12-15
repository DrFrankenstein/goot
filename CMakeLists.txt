cmake_minimum_required(VERSION 3.19)

project(
	goot
	VERSION 0.0.1
	LANGUAGES CXX
)

include(compile_options.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
include(3rd-party/cmake-conan/conan.cmake)

conan_cmake_configure(
	REQUIRES
		openssl/1.1.1q
		libgit2/1.3.0
		qt/6.3.1
		zlib/1.2.12
	OPTIONS
		qt:qttools=True
		qt:shared=True
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
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

find_package(Git)
if(NOT ${GIT_FOUND})
	set(PROJECT_BUILD unknown)
else()
	execute_process(
		COMMAND ${GIT_EXECUTABLE} describe --always --dirty
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE PROJECT_BUILD
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

string(TIMESTAMP BUILD_YEAR "%Y")

qt_standard_project_setup()

add_subdirectory(src)
