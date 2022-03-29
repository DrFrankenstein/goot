add_executable(goot)

target_sources(goot
PRIVATE
	goot.cpp
)

target_link_libraries(goot
PRIVATE
	Qt5::Widgets
	CONAN_PKG::libgit2
)
