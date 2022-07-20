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

namespace Gui::InitWizard
{
InitWizard::InitWizard(Git::Git& git, QWidget* parent):
    QWizard { parent }, m_git { git }
{
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
