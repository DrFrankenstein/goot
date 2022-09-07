#pragma once

#include "../utils/BrowseInput.hpp"
#include "ui_Page1Location.h"

#include <QWidget>
#include <QWizardPage>
#include <memory>

class QString;

namespace Git
{
class Git;
}

namespace Gui::InitWizard
{
class Page1Location : public QWizardPage
{
	Q_OBJECT

	public:
	explicit Page1Location(Git::Git& git, QWidget* parent = nullptr);

	private:
	auto initializePage() -> void override;
	auto validatePage() -> bool override;

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
