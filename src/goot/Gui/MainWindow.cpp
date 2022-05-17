#include "MainWindow.hpp"

#include "AboutDialog.hpp"
#include "InitWizard.hpp"
#include "RepositoryWindow.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QWhatsThis>
#include <git2xx/Error.hpp>
#include <git2xx/Git.hpp>
#include <git2xx/Repository.hpp>
#include <utility>

class QWidget;

namespace Gui
{
MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent)
{
	ui.setupUi(this);
}

auto MainWindow::on_actionNew_triggered() -> void
{
	InitWizard wizard { git, this };
	if (wizard.exec() == QDialog::Accepted)
		openRepo(wizard.getRepository());
}

auto MainWindow::on_actionOpen_triggered() -> void
{
	const auto path = QFileDialog::getExistingDirectory(this, tr("Select repository location"));

	if (path.isEmpty())
		return;

	try
	{
		auto repo = git.openRepository(path.toStdString());
		openRepo(repo);
	}
	catch (const Git::Error& error)
	{
		QMessageBox::warning(this, tr("Could not open repository"), error.what());
	}
}

auto MainWindow::on_actionQuit_triggered() -> void
{
	QApplication::exit();
}

auto MainWindow::on_actionWhatsThis_triggered() -> void
{
	QWhatsThis::enterWhatsThisMode();
}

auto MainWindow::on_actionAbout_triggered() -> void
{
	AboutDialog about;
	about.exec();
}

auto MainWindow::openRepo(Git::Repository& repo) -> void
{
	auto& subwindow = *ui.mdiArea->addSubWindow(new RepositoryWindow(std::move(repo)));
	subwindow.show();
}

}
