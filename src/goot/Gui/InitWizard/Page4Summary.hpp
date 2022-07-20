
#pragma once

#include "ui_Page4Summary.h"

#include <QWidget>
#include <QWizardPage>

namespace Gui::InitWizard
{
class Page4Summary : public QWizardPage
{
	Q_OBJECT
	
	public:
	explicit Page4Summary(QWidget* parent = nullptr);

	private:
	Ui::Page4Summary ui;
};
}
