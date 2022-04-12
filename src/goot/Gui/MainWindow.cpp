#include "MainWindow.hpp"

#include "AboutDialog.hpp"
#include "RepositoryWindow.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <git2xx/Error.hpp>
#include <git2xx/Git.hpp>
#include <utility>

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
		auto& subwindow = *ui.mdiArea->addSubWindow(new RepositoryWindow(std::move(repo)));
		subwindow.show();
	}
	catch (const Git::Error& error)
	{
		QMessageBox::warning(this, tr("Could not open repository"), error.what());
	}
}

auto MainWindow::on_actionAbout_triggered() -> void
{
	AboutDialog about;
	about.exec();
}

}
