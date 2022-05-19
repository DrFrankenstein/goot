#pragma once

#include "ui_InitWizard.h"
#include "../utils/BrowseInput.hpp"

#include <QByteArray>
#include <QWizard>
#include <git2xx/Git.hpp>
#include <git2xx/Repository.hpp>
#include <utility>

namespace Gui
{
class InitWizard : public QWizard
{
	Q_OBJECT

	public:
	explicit InitWizard(Git::Git& git, QWidget* parent = nullptr);

	auto getRepository() -> Git::Repository&;

	private:
	// This might benefit from being split into separate QWizardPage subclasses,
	// but that would also mean losing *some* Qt Designer ease-of-maintenance.
	// I'm still considering it, but I'll probably leave that for later.
	auto initializePage(int id) -> void override;
	auto validateCurrentPage() -> bool override;

	auto page1Location_initialize() -> void;
	auto page1Location_validate() -> bool;

	auto page2Description_initialize() -> void;
	auto page2Description_validate() -> bool;

	auto page3Ref_initialize() -> void;
	auto page3Ref_validate() -> bool;

	auto page4Summary_initialize() -> void;
	auto page4Summary_validate() -> bool;

	auto updateInitOptions() -> void;

	auto askCreate(const QString& path) -> bool;
	auto askReinit() -> bool;
	auto askSubrepo(const QString& path) -> bool;

	auto accept() -> void override;

	auto makeOptions() -> Git::RepositoryInitOptions;

	Git::Git& m_git;
	Git::Repository m_repo;
	Ui::InitWizard ui;

	std::unique_ptr<Utils::BrowseInput> m_pathBrowse;
	std::unique_ptr<Utils::BrowseInput> m_workdirBrowse;

	bool m_mkdir  = false;
	bool m_reinit = false;
};
}
