#pragma once

#include "../utils/BrowseInput.hpp"

#include <QByteArray>
#include <QWizard>
#include <git2xx/Git.hpp>
#include <git2xx/Repository.hpp>
#include <utility>

#include "Page1Location.hpp"
#include "Page2Description.hpp"
#include "Page3Ref.hpp"
#include "Page4Summary.hpp"

namespace Gui::InitWizard
{
class Wizard : public QWizard
{
	Q_OBJECT

	enum class Page
	{
		Location,
		Description,
		Ref,
		Summary
	};

	public:
	explicit Wizard(Git::Git& git, QWidget* parent = nullptr);

	auto getRepository() -> Git::Repository&;

	private:
	inline auto setPage(Page id, QWizardPage* page)
	{
		return QWizard::setPage(static_cast<int>(id), page);
	}

	auto accept() -> void override;

	auto makeOptions() -> Git::RepositoryInitOptions;

	Git::Git& m_git;
	Git::Repository m_repo;

	Page1Location m_page1;
	Page2Description m_page2;
	Page3Ref m_page3;
	Page4Summary m_page4;

	bool m_mkdir  = false;
	bool m_reinit = false;
};
}
