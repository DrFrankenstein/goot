#include "Page2Description.hpp"

#include "Wizard.hpp"

#include <QWidget>

namespace Gui::InitWizard
{
Page2Description::Page2Description(QWidget* parent):
    QWizardPage { parent }
{
	ui.setupUi(this);

	registerField(Fields::description.toString(), ui.lineEditDescription);
}

}
