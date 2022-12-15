qt_add_executable(goot)

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
		
		Gui/InitWizard/Wizard.hpp
		Gui/InitWizard/Wizard.cpp
		Gui/InitWizard/Page1Location.ui
		Gui/InitWizard/Page1Location.hpp
		Gui/InitWizard/Page1Location.cpp
		Gui/InitWizard/Page2Description.ui
		Gui/InitWizard/Page2Description.hpp
		Gui/InitWizard/Page2Description.cpp
		Gui/InitWizard/Page3Ref.ui
		Gui/InitWizard/Page3Ref.hpp
		Gui/InitWizard/Page3Ref.cpp
		Gui/InitWizard/Page4Summary.ui
		Gui/InitWizard/Page4Summary.hpp
		Gui/InitWizard/Page4Summary.cpp

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

target_precompile_headers(
	goot
	PRIVATE
		<algorithm>
		<cstddef>
		<numeric>
		<ranges>
		<span>
		<string>
		<string_view>
		<utility>
		<git2xx.hpp>
		<QList>
		<QObject>
		<QString>
		<QWidget>
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
endif()

install(
	TARGETS goot
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
	TARGET goot
	FILENAME_VARIABLE qtdeploy
	NO_UNSUPPORTED_PLATFORM_ERROR
)
message(NOTICE ${qtdeploy})
install(SCRIPT ${qtdeploy})