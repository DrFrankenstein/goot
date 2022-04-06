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

	private:
	Ui::AboutDialog ui;
};
}
