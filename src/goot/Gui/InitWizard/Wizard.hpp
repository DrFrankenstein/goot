#pragma once

#include "../utils/BrowseInput.hpp"
#include "Page1Location.hpp"
#include "Page2Description.hpp"
#include "Page3Ref.hpp"
#include "Page4Summary.hpp"

#include <QByteArray>
#include <QString>
#include <QStringView>
#include <QWizard>
#include <git2xx/Git.hpp>
#include <git2xx/Repository.hpp>
#include <utility>

namespace Gui::InitWizard
{
namespace Fields
{
// Page 1
constexpr auto path = QStringView { u"path" };

// Page 2
constexpr auto description = QStringView { u"description" };

// Page 3
constexpr auto ref = QStringView { u"ref" };
constexpr auto bare = QStringView { u"bare" };
constexpr auto customWorkdir = QStringView { u"customWorkdir" };
constexpr auto workdir = QStringView { u"workdir" };
}

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

	auto makeOptions() const -> Git::RepositoryInitOptions;

	Git::Git& m_git;
	Git::Repository m_repo;

	Page1Location m_page1;
	Page2Description m_page2;
	Page3Ref m_page3;
	Page4Summary m_page4;

	bool m_mkdir = false;
	bool m_reinit = false;
};
}
