#pragma once

#include "ui_AboutDialog.h"

#include <QDialog>
#include <QWidget>

namespace Gui
{
class AboutDialog : public QDialog
{
	Q_OBJECT

	public:
	explicit AboutDialog(QWidget* parent = nullptr);

	public slots:
	auto on_pushButtonQtLicense_clicked() -> void;
	auto on_toolButtonGitHub_clicked() -> void;

	private:
	Ui::AboutDialog ui;
};
}
