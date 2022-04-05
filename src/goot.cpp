#include "Gui/MainWindow.hpp"

#include <QApplication>

using Gui::MainWindow;

int main(int argc, char* argv[])
{
	QApplication app { argc, argv };

	MainWindow mainWindow;
	mainWindow.show();

	return QApplication::exec();
}
