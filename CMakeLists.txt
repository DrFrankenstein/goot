cmake_minimum_required(VERSION 3.11)

project(
	goot
	VERSION 0.0.1
	LANGUAGES CXX
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(src)
