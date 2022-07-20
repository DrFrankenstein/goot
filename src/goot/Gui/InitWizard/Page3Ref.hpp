
#pragma once

#include "ui_Page3Ref.h"

#include <QWidget>
#include <QWizardPage>

namespace Gui::InitWizard
{
class Page3Ref : public QWizardPage
{
	Q_OBJECT
	
	public:
	explicit Page3Ref(QWidget* parent = nullptr);

	virtual auto initializePage() -> void override;

	private:
	Ui::Page3Ref ui;
};
}
