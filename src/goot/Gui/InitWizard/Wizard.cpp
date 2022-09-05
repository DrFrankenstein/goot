#include "Wizard.hpp"

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
Wizard::Wizard(Git::Git& git, QWidget* parent):
    QWizard { parent }, m_git { git }, m_page1 { git }
{
	setPixmap(QWizard::LogoPixmap, { ":/icons/icons/fluency/icons8-new-repository-48.png" });
	setPage(Page::Location, &m_page1);
	setPage(Page::Description, &m_page2);
	setPage(Page::Ref, &m_page3);
	setPage(Page::Summary, &m_page4);
}

auto Wizard::getRepository() -> Git::Repository&
{
	return m_repo;
}

auto Wizard::accept() -> void
{
	if (!validateCurrentPage())
		return;

	auto options = makeOptions();

	const auto path = field(Fields::path.toString()).toString().toStdString();
	m_repo = m_git.initRepository(path, options);

	QWizard::accept();
}

auto Wizard::makeOptions() const -> Git::RepositoryInitOptions
{
	Git::RepositoryInitOptions options;

	if (!m_reinit)
		options.flags |= GIT_REPOSITORY_INIT_NO_REINIT;

	if (m_mkdir)
		options.flags |= GIT_REPOSITORY_INIT_MKDIR;

	const auto bare = field(Fields::bare.toString()).toBool();
	if (bare)
		options.flags |= GIT_REPOSITORY_INIT_BARE;

	// TODO add perms
	options.mode = GIT_REPOSITORY_INIT_SHARED_UMASK;

	const auto description = field(Fields::description.toString()).toString();
	if (!description.isEmpty())
		options.setDescription(description.toStdString());

	const auto initialHead = field(Fields::ref.toString()).toString();
	if (!initialHead.isEmpty())
		options.setInitialHead(initialHead.toStdString());

	const auto hasCustomWorkDir = field(Fields::customWorkdir.toString()).toBool();
	if (hasCustomWorkDir)
	{
		const auto workdir = field(Fields::workdir.toString()).toString();
		options.setWorkdirPath(workdir.toStdString());
	}

	return options;
}

}
