#include "Gui/MainWindow.hpp"

#include <QApplication>

using Gui::MainWindow;

auto main(int argc, char* argv[]) -> int
{
	QApplication app { argc, argv };

	MainWindow mainWindow;
	mainWindow.show();

	return QApplication::exec();
}
