
#pragma once

#include "ui_Page4Summary.h"

#include <QWizardPage>

class QWidget;

namespace Gui::InitWizard
{
class Page4Summary : public QWizardPage
{
	Q_OBJECT

	public:
	explicit Page4Summary(QWidget* parent = nullptr);

	private:
	auto initializePage() -> void override;

	Ui::Page4Summary ui;
};
}
