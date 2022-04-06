#pragma once

#include "ui_MainWindow.h"

#include <QMainWindow>
#include <QWidget>

namespace Gui
{
class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
	explicit MainWindow(QWidget* parent = nullptr);

	public slots:
	void on_actionAbout_triggered(bool checked = false);

	private:
	Ui::MainWindow ui;
};

}
