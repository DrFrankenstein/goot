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
	auto on_actionAbout_triggered(bool checked = false) -> void;

	private:
	Ui::MainWindow ui;
};

}