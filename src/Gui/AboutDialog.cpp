#include "AboutDialog.hpp"

#include <QDialog>
#include <QWidget>

namespace Gui
{
AboutDialog::AboutDialog(QWidget* parent):
    QDialog(parent)
{
	ui.setupUi(this);
}
}
