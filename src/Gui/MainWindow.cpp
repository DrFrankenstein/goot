#include "MainWindow.hpp"

#include <QMainWindow>
#include <QWidget>

namespace Gui
{
MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent)
{
	ui.setupUi(this);
}

}
