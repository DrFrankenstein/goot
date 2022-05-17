#pragma once

#include <QFileDialog>
#include <QObject>
#include <QString>

class QLineEdit;
class QAbstractButton;

namespace Gui::Utils
{
class BrowseInput : public QObject
{
	Q_OBJECT
	Q_PROPERTY(PathMode pathMode READ pathMode WRITE setPathMode)

	public:
	enum class PathMode { File, Url };

	BrowseInput(QObject* parent, QLineEdit& edit, QAbstractButton& browseButton, 
		const QString& caption = {});

	auto dialog() -> QFileDialog&;

	auto pathMode() -> PathMode;

	private slots:
	auto onBrowseButtonClicked() -> void;
	auto setPathMode(PathMode pathMode) -> void;

	private:
	PathMode m_pathMode = PathMode::File;

	QLineEdit& m_edit;
	QAbstractButton& m_browseButton;

	QFileDialog m_dlg;
};
}
