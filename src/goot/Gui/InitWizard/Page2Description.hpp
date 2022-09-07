
#pragma once

#include "ui_Page2Description.h"

#include <QWizardPage>

class QWidget;

namespace Gui::InitWizard
{
class Page2Description : public QWizardPage
{
	Q_OBJECT

	public:
	explicit Page2Description(QWidget* parent = nullptr);

	private:
	Ui::Page2Description ui;
};
}
