
#include "Page3Ref.hpp"

#include <QWidget>
#include <QWizardPage>
#include <memory>

using std::make_unique;

namespace Gui::InitWizard
{
Page3Ref::Page3Ref(QWidget* parent):
    QWizardPage { parent }
{
	ui.setupUi(this);

	m_workdirBrowse = make_unique<Utils::BrowseInput>(
	    this, *ui.lineEditWorkdir, *ui.pushButtonBrowseWorkdir, tr("Select working copy location")
	);
	m_workdirBrowse->dialog().setFileMode(QFileDialog::Directory);
}

auto Page3Ref::initializePage() -> void
{
	// TODO: read default branch from config (init.defaultbranch)
}

}
