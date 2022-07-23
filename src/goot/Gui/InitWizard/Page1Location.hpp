#pragma once

#include "../utils/BrowseInput.hpp"
#include "ui_Page1Location.h"

#include <QWidget>
#include <QWizardPage>
#include <git2xx/Git.hpp>
#include <memory>

class QString;

namespace Gui::InitWizard
{
class Page1Location : public QWizardPage
{
	Q_OBJECT

	public:
	explicit Page1Location(Git::Git& git, QWidget* parent = nullptr);

	auto initializePage() -> void override;
	auto validatePage() -> bool override;

	private:
	auto askCreate(const QString& path) -> bool;
	auto askReinit() -> bool;
	auto askSubrepo(const QString& path) -> bool;

	Ui::Page1Location ui;
	std::unique_ptr<Utils::BrowseInput> m_pathBrowse;

	Git::Git& m_git;

	bool m_reinit;
	bool m_mkdir;
};
}
