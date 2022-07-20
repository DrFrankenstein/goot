#pragma once

#include "ui_Page1Location.h"

#include <QWidget>
#include <QWizardPage>

class QString;

namespace Gui::InitWizard
{
class Page1Location : public QWizardPage
{
	Q_OBJECT
	
	public:
	explicit Page1Location(QWidget* parent = nullptr);

	virtual auto initializePage() -> void override;
	virtual auto validatePage() -> bool override;

	private:
	auto askCreate(const QString& path) -> bool;
	auto askReinit() -> bool;
	auto askSubrepo(const QString& path) -> bool;

	Ui::Page1Location ui;

	bool m_reinit;
	bool m_mkdir;
};
}
