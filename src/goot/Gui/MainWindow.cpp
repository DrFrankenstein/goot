#include "MainWindow.hpp"

#include "AboutDialog.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <git2xx/Error.hpp>
#include <git2xx/Git.hpp>

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

	try
	{
		auto repo = git.openRepository(path.toStdString());
	}
	catch (Git::Error error)
	{
		// TODO
	}
}

auto MainWindow::on_actionAbout_triggered() -> void
{
	AboutDialog about;
	about.exec();
}

}
