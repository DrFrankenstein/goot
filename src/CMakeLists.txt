add_subdirectory(libgit2xx)

add_executable(goot)

target_sources(goot
PRIVATE
	goot.cpp

	res/main.qrc

	Gui/MainWindow.ui
	Gui/MainWindow.hpp
	Gui/MainWindow.cpp
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

	if(CMAKE_BUILD_TYPE STREQUAL Debug)
		set(DEPLOYQT_BUILD_TYPE --debug)
	elseif(CMAKE_BUILD_TYPE STREQUAL Release)
		set(DEPLOYQT_BUILD_TYPE --release)
	elseif(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
		set(DEPLOYQT_BUILD_TYPE --release-with-debug-info)
	endif()

	find_program(DEPLOYQT windeployqt REQUIRED)

	add_custom_target(windeployqt
		ALL
		${DEPLOYQT} ${DEPLOYQT_BUILD_TYPE} $<TARGET_FILE:goot>
		DEPENDS goot
		COMMENT "Copying Qt libraries to output dir"
	)
endif()
