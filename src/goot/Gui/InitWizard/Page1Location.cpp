#include "Page1Location.hpp"

#include "../utils/BrowseInput.hpp"
#include "Wizard.hpp"

#include <QChar>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <git2xx/Repository.hpp>
#include <memory>

using std::make_unique;

namespace Gui::InitWizard
{
Page1Location::Page1Location(Git::Git& git, QWidget* parent):
    QWizardPage { parent }, m_git { git }
{
	ui.setupUi(this);

	m_pathBrowse = make_unique<Utils::BrowseInput>(
	    this, *ui.lineEditPath, *ui.pushButtonBrowsePath, tr("Select repository location")
	);
	m_pathBrowse->dialog().setFileMode(QFileDialog::Directory);

	registerField(Fields::path.toString(), ui.lineEditPath);
}

auto Page1Location::initializePage() -> void
{
	m_reinit = false;
	m_mkdir = false;
}

auto Page1Location::validatePage() -> bool
{
	const auto path = ui.lineEditPath->text();

	if (path.isEmpty())
		return false;

	const auto dir = QDir { path };

	// ???: check whether path looks valid at all?

	if (!dir.exists())
	{
		if (!askCreate(path))
			return false;

		m_mkdir = true;
	}

	auto parent = dir;
	parent.cdUp();
	const auto parentPath = parent.path().toStdString();
	const auto existingRepoPath = Git::Repository::discover(m_git, path.toStdString(), false, parentPath);
	const auto isRepo = !existingRepoPath.view().empty();
	if (isRepo)
	{
		if (!askReinit())
			return false;

		m_reinit = true;
	}

	const auto parentRepo = Git::Repository::discover(m_git, parentPath);
	const auto isBelowRepo = !parentRepo.view().empty();
	if (isBelowRepo)
	{
		const auto parentRepoPath = parentRepo.data();
		if (!askSubrepo(QString::fromUtf8(parentRepoPath.data(), parentRepoPath.size())))
			return false;
	}

	// ???: what happens if we attempt to create a bare repo in a non-empty directory?

	return true;
}

auto Page1Location::askCreate(const QString& path) -> bool
{  // FIXME: There's room for a lot of UX improvement here. I want to use a
	// TaskDialog on Windows, but I'd either have to fall back on a message box
	// on other platforms, or shim that in myself in pure Qt. Another option
	// would be navigating to a special wizard page with the appropriate
	// message and options.

	QMessageBox mbox { this };
	mbox.setIcon(QMessageBox::Warning);
	mbox.setText(tr("The path specified does not exist. Do you want to create it?"));
	mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	mbox.setDetailedText(path);
	return mbox.exec() == QMessageBox::Yes;
}

auto Page1Location::askReinit() -> bool
{
	QMessageBox mbox { this };
	mbox.setIcon(QMessageBox::Warning);
	mbox.setText(tr("This folder is already a git repository. Do you want to re-initialize it?"));
	// TODO: clarify what reinit means; it looks like it might just be recreating the config: https://github.com/libgit2/libgit2/blob/63970244fb2d49794e7b5d268a2defe4299fd3ad/src/libgit2/repository.c#L2390-L2403
	// mbox.setInformativeText(tr("Re-initializing"))
	mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	return mbox.exec() == QMessageBox::Yes;
}

auto Page1Location::askSubrepo(const QString& path) -> bool
{
	QMessageBox mbox { this };
	mbox.setIcon(QMessageBox::Warning);
	mbox.setText(tr("This folder is inside an existing git repository. Create a repository here anyway?"));
	mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	return mbox.exec() == QMessageBox::Yes;
}

}
