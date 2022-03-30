add_executable(goot)

target_sources(goot
PRIVATE
	goot.cpp
)

target_link_libraries(goot
PRIVATE
	compile_options
	CONAN_PKG::qt
	CONAN_PKG::libgit2
)
