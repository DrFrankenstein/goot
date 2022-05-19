add_executable(goot)

target_sources(
	goot
	PRIVATE
		goot.cpp

		res/main.qrc

		Gui/utils.hpp

		Gui/MainWindow.ui
		Gui/MainWindow.hpp
		Gui/MainWindow.cpp

		Gui/RepositoryWindow.ui
		Gui/RepositoryWindow.hpp
		Gui/RepositoryWindow.cpp
		
		Gui/InitWizard/InitWizard.ui
		Gui/InitWizard/InitWizard.hpp
		Gui/InitWizard/InitWizard.cpp

		Gui/AboutDialog.ui
		Gui/AboutDialog.hpp
		Gui/AboutDialog.cpp

		Gui/utils/BrowseInput.hpp
		Gui/utils/BrowseInput.cpp
)

configure_file(config.hpp.in config.hpp @ONLY)
target_include_directories(
	goot
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
	goot
	PRIVATE
		compile_options
		Qt6::Widgets
		git2xx
)

if(WIN32)
	target_link_libraries(
		goot
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

	add_custom_target(
		windeployqt
		ALL
		${DEPLOYQT} ${DEPLOYQT_BUILD_TYPE} $<TARGET_FILE:goot>
		DEPENDS goot
		COMMENT "Copying Qt libraries to output dir"
	)
endif()
