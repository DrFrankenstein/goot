#pragma once

#include "ui_RepositoryWindow.h"

#include <QWidget>
#include <git2xx/Repository.hpp>

namespace Gui
{
class RepositoryWindow : public QWidget
{
	Q_OBJECT

	public:
	explicit RepositoryWindow(Git::Repository repo, QWidget* parent = nullptr);

	private:
	Ui::RepositoryWindow ui;
	Git::Repository m_repo;
};
}
