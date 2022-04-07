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

	Utils::formatLabel(*ui.labelVersion, Config::PROJECT_VERSION);
	Utils::formatLabel(*ui.labelCopyright, Config::BUILD_YEAR);
	
	const auto qtver = string_view { qVersion() };
	Utils::formatLabel(*ui.labelQt, qtver) ;
}

auto AboutDialog::on_pushButtonQtLicense_clicked() -> void
{
	QDesktopServices::openUrl(QUrl { "https://doc.qt.io/qt-6/lgpl.html" });
}

auto AboutDialog::on_toolButtonGitHub_clicked() -> void
{
	QDesktopServices::openUrl(QUrl { "https://github.com/DrFrankenstein/goot" });
}

}
