#include "BrowseInput.hpp"

#include <QAbstractButton>
#include <QDir>
#include <QFileDialog>
#include <QLineEdit>
#include <QString>

class QWidget;

namespace Gui::Utils
{
BrowseInput::BrowseInput(QObject* parent, QLineEdit& edit, QAbstractButton& browseButton, const QString& caption):
    QObject { parent }, 
	m_edit { edit }, 
	m_browseButton { browseButton },
    m_dlg { qobject_cast<QWidget*>(parent), caption }
{
	connect(&m_browseButton, &QAbstractButton::clicked, this, &BrowseInput::onBrowseButtonClicked);
}

auto BrowseInput::dialog() -> QFileDialog&
{
	return m_dlg;
}

auto BrowseInput::pathMode() -> PathMode
{
	return m_pathMode;
}

auto BrowseInput::setPathMode(PathMode pathMode) -> void
{
	m_pathMode = pathMode;
}

auto BrowseInput::onBrowseButtonClicked() -> void
{
	if (!m_edit.text().isEmpty())
	{
		switch (m_pathMode)
		{
		case PathMode::File:
			m_dlg.selectFile(QDir::fromNativeSeparators(m_edit.text()));
			break;
		case PathMode::Url:
			m_dlg.selectUrl(m_edit.text());
			break;
		}
	}

	if (m_pathMode == PathMode::File)
		m_dlg.setSupportedSchemes({ "file" });
	else
		m_dlg.setSupportedSchemes({});

	const auto result = m_dlg.exec();
	if (result != QDialog::Accepted)
		return;

	switch (m_pathMode)
	{
	case PathMode::File:
		m_edit.setText(QDir::toNativeSeparators(m_dlg.selectedFiles().value(0)));
		break;

	case PathMode::Url:
		m_edit.setText(m_dlg.selectedUrls().value(0).toString());
		break;
	}
}
}
