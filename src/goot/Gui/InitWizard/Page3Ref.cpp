
#include "Page3Ref.hpp"

#include <QWidget>
#include <QWizardPage>

namespace Gui::InitWizard
{
Page3Ref::Page3Ref(QWidget* parent):
    QWizardPage { parent } 
{ }

auto Page3Ref::initializePage() -> void
{
	// TODO: read default branch from config (init.defaultbranch)
}

}
