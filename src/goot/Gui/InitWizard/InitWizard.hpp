#pragma once

#include "ui_InitWizard.h"
#include "../utils/BrowseInput.hpp"

#include <QByteArray>
#include <QWizard>
#include <git2xx/Git.hpp>
#include <git2xx/Repository.hpp>
#include <utility>

namespace Gui::InitWizard
{
class InitWizard : public QWizard
{
	Q_OBJECT

	public:
	explicit InitWizard(Git::Git& git, QWidget* parent = nullptr);

	auto getRepository() -> Git::Repository&;

	private:
	auto accept() -> void override;

	auto makeOptions() -> Git::RepositoryInitOptions;

	Git::Git& m_git;
	Git::Repository m_repo;

	std::unique_ptr<Utils::BrowseInput> m_pathBrowse;
	std::unique_ptr<Utils::BrowseInput> m_workdirBrowse;

	bool m_mkdir  = false;
	bool m_reinit = false;
};
}
