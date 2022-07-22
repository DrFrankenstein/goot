
#include "Page2Description.hpp"

#include <QWidget>

namespace Gui::InitWizard
{
Page2Description::Page2Description(QWidget* parent):
    QWizardPage { parent }
{
	ui.setupUi(this);
}

}
