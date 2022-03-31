add_subdirectory(libgit2xx)

add_executable(goot)

target_sources(goot
PRIVATE
	goot.cpp
)

target_link_libraries(goot
PRIVATE
	compile_options
	CONAN_PKG::qt
	git2xx
)

if(WIN32)
	target_link_libraries(goot
	PRIVATE
		dwrite
	)
endif()
