#include "MainWindow.hpp"

#include <QFileDialog>
#include <QMainWindow>

#include "AboutDialog.hpp"

class QWidget;

namespace Gui
{
MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent)
{
	ui.setupUi(this);
}

auto MainWindow::on_actionOpen_triggered() -> void
{
	const auto path = QFileDialog::getExistingDirectory(this, tr("Select repository location"));

	if (path.isEmpty())
		return;
}

auto MainWindow::on_actionAbout_triggered() -> void
{
	AboutDialog about;
	about.exec();
}

}
