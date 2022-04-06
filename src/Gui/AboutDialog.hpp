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
	void on_pushButtonQtLicense_clicked();
	void on_toolButtonGitHub_clicked();

	private:
	Ui::AboutDialog ui;
};
}
