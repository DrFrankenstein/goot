#include "RepositoryWindow.hpp"

#include <QWidget>
#include <utility>

namespace Gui
{
RepositoryWindow::RepositoryWindow(Git::Repository repo, QWidget* parent):
    QWidget { parent }, m_repo { std::move(repo) }
{ 
	ui.setupUi(this);
}
}
