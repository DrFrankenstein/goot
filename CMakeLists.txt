cmake_minimum_required(VERSION 3.16)

project(
	goot
	VERSION 0.0.1
	LANGUAGES CXX
)

include(compile_options.cmake)

# replace with qt_standard_project_setup() once we're on 6.4.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
include(3rd-party/cmake-conan/conan.cmake)

conan_cmake_configure(
	REQUIRES
		openssl/1.1.1m
		libgit2/1.3.0
		qt/6.3.1
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
find_package(Qt6 REQUIRED COMPONENTS Widgets)

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

add_subdirectory(src)
