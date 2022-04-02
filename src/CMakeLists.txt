add_subdirectory(libgit2xx)

add_executable(goot)

target_sources(goot
PRIVATE
	goot.cpp
)

target_link_libraries(goot
PRIVATE
	compile_options
	Qt6::Widgets
	git2xx
)

if(WIN32)
	target_link_libraries(goot
	PRIVATE
		dwrite	# QtGui (dep of QtWidgets) seems to need that
	)
endif()
