#include "AboutDialog.hpp"

#include <QDesktopServices>
#include <QDialog>
#include <QUrl>
#include <QWidget>

namespace Gui
{
AboutDialog::AboutDialog(QWidget* parent):
    QDialog(parent)
{
	ui.setupUi(this);
}

void AboutDialog::on_pushButtonQtLicense_clicked()
{
	QDesktopServices::openUrl(QUrl { "https://doc.qt.io/qt-6/lgpl.html" });
}

void AboutDialog::on_toolButtonGitHub_clicked()
{
	QDesktopServices::openUrl(QUrl { "https://github.com/DrFrankenstein/goot" });
}

}
