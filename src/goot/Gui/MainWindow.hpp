#pragma once

#include "ui_MainWindow.h"

#include <QMainWindow>
#include <QObject>
#include <git2xx/Git.hpp>

class QWidget;

namespace Gui
{
class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
	explicit MainWindow(QWidget* parent = nullptr);

	public slots:
	auto on_actionOpen_triggered() -> void;
	auto on_actionQuit_triggered() -> void;
	auto on_actionWhatsThis_triggered() -> void;
	auto on_actionAbout_triggered() -> void;

	private:
	Ui::MainWindow ui;
	Git::Git git;
};

}
