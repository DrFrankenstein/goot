#include "InitWizard.hpp"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <QWizard>
#include <cstdint>
#include <git2xx/Git.hpp>
#include <git2xx/Repository.hpp>
#include <qglobal.h>
#include <utility>

using std::make_unique, std::uint32_t;

namespace Gui
{
InitWizard::InitWizard(Git::Git& git, QWidget* parent):
    QWizard { parent }, m_git { git }
{
	ui.setupUi(this);

	setPixmap(QWizard::LogoPixmap, { ":/icons/icons/fluency/icons8-new-repository-48.png" });
	
	m_pathBrowse = make_unique<Utils::BrowseInput>(
		this, *ui.lineEditPath, *ui.pushButtonBrowsePath, tr("Select repository location")
	);
	m_pathBrowse->dialog().setFileMode(QFileDialog::Directory);

	m_workdirBrowse = make_unique<Utils::BrowseInput>(
		this, *ui.lineEditWorkdir, *ui.pushButtonBrowseWorkdir, tr("Select working copy location")
	);
	m_workdirBrowse->dialog().setFileMode(QFileDialog::Directory);
}

auto InitWizard::getRepository() -> Git::Repository&
{
	return m_repo;
}

auto InitWizard::initializePage(int id) -> void
{
	switch (id)
	{
	case 1: page1Location_initialize(); break;
	case 2: page2Description_initialize(); break;
	case 3: page3Ref_initialize(); break;
	case 4: page4Summary_initialize(); break;
	default: qt_noop();
	}

	QWizard::initializePage(id);
}

auto InitWizard::validateCurrentPage() -> bool
{
	switch (currentId())
	{
	case 1: return page1Location_validate();
	case 2: return page2Description_validate();
	case 3: return page3Ref_validate();
	case 4: return page4Summary_validate();
	default: return false;
	}
}

auto InitWizard::page1Location_initialize() -> void
{
	m_reinit = false;
}

auto InitWizard::page1Location_validate() -> bool
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
	const auto existingRepoPath = m_git.discoverRepository(path.toStdString(), false, parentPath);
	const auto isRepo   = !existingRepoPath.view().empty();
	if (isRepo)
	{
		if (!askReinit())
			return false;

		m_reinit = true;
	}

	const auto parentRepo = m_git.discoverRepository(parentPath);
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

auto InitWizard::page2Description_initialize() -> void { }
auto InitWizard::page2Description_validate() -> bool { return true; }

auto InitWizard::page3Ref_initialize() -> void 
{
	// TODO: read default branch from config (init.defaultbranch)
}
auto InitWizard::page3Ref_validate() -> bool { return true; }

auto InitWizard::page4Summary_initialize() -> void { }
auto InitWizard::page4Summary_validate() -> bool { return true; }

auto InitWizard::askCreate(const QString& path) -> bool
{	// FIXME: There's room for a lot of UX improvement here. I want to use a
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

auto InitWizard::askReinit() -> bool
{	
	QMessageBox mbox { this };
	mbox.setIcon(QMessageBox::Warning);
	mbox.setText(tr("This folder is already a git repository. Do you want to re-initialize it?"));
	// TODO: clarify what reinit means; it looks like it might just be recreating the config: https://github.com/libgit2/libgit2/blob/63970244fb2d49794e7b5d268a2defe4299fd3ad/src/libgit2/repository.c#L2390-L2403
	// mbox.setInformativeText(tr("Re-initializing"))
	mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	return mbox.exec() == QMessageBox::Yes;
}

auto InitWizard::askSubrepo(const QString& path) -> bool
{
	QMessageBox mbox { this };
	mbox.setIcon(QMessageBox::Warning);
	mbox.setText(tr("This folder is inside an existing git repository. Create a repository here anyway?"));
	mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	return mbox.exec() == QMessageBox::Yes;
}

auto InitWizard::accept() -> void
{
	if (!validateCurrentPage())
		return;

	auto options = makeOptions();

	m_repo = m_git.initRepository(ui.lineEditPath->text().toStdString(), options);

	QWizard::accept();
}

auto InitWizard::makeOptions() -> Git::RepositoryInitOptions
{
	Git::RepositoryInitOptions options;

	if (!m_reinit)
		options.flags |= GIT_REPOSITORY_INIT_NO_REINIT;

	if (m_mkdir)
		options.flags |= GIT_REPOSITORY_INIT_MKDIR;

	if (ui.checkBoxBare->isChecked())
		options.flags |= GIT_REPOSITORY_INIT_BARE;
	
	// TODO add perms
	options.mode = GIT_REPOSITORY_INIT_SHARED_UMASK;

	const auto description = ui.lineEditDescription->text();
	if (!description.isEmpty())
		options.setDescription(description.toStdString());

	const auto initialHead = ui.lineEditRef->text();
	if (!initialHead.isEmpty())
		options.setInitialHead(initialHead.toStdString());

	if (ui.checkBoxCustomWorkdir->isChecked())
		options.setWorkdirPath(ui.lineEditWorkdir->text().toStdString());

	return options;
}

}
