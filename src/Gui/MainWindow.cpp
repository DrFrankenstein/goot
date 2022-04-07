#include "MainWindow.hpp"

#include <QMainWindow>
#include <QWidget>

#include "AboutDialog.hpp"

namespace Gui
{
MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent)
{
	ui.setupUi(this);
}

auto MainWindow::on_actionAbout_triggered(bool checked) -> void
{
	AboutDialog about;
	about.exec();
}

}
