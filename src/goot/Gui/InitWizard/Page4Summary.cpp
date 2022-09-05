#include "Page4Summary.hpp"

#include "Wizard.hpp"

#include <QWidget>

namespace Gui::InitWizard
{
Page4Summary::Page4Summary(QWidget* parent):
    QWizardPage { parent }
{
	ui.setupUi(this);
}

auto Page4Summary::initializePage() -> void
{
	const auto format = ui.labelSummary->text();
	const auto repo = field(InitWizard::Fields::path.toString()).toString();
	const auto ref = field(InitWizard::Fields::ref.toString()).toString();
	const auto customWorkdir = field(InitWizard::Fields::customWorkdir.toString()).toBool();

	const auto workdir = customWorkdir ?
	    field(InitWizard::Fields::workdir.toString()).toString() :
	    repo;

	ui.labelSummary->setText(format.arg(repo, ref, workdir));
}
}
