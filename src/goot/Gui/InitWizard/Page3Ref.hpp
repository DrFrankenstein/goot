
#pragma once

#include "ui_Page3Ref.h"

#include <memory>
#include <QWidget>
#include <QWizardPage>

#include "../utils/BrowseInput.hpp"

namespace Gui::InitWizard
{
class Page3Ref : public QWizardPage
{
	Q_OBJECT
	
	public:
	explicit Page3Ref(QWidget* parent = nullptr);

	private:
	auto initializePage() -> void override;

	Ui::Page3Ref ui;

	std::unique_ptr<Utils::BrowseInput> m_workdirBrowse;
};
}
