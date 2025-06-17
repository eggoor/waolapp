#include "EClipboard.h"

EClipboard::EClipboard(QObject* parent)
	: QObject(parent)
	, clipboard(QGuiApplication::clipboard())
{
	connect(clipboard, &QClipboard::dataChanged, this, &EClipboard::textChanged);
}

QString EClipboard::text() const
{
	return clipboard->text();
}

void EClipboard::setText(const QString& text)
{
	if (clipboard->text() != text) {
		clipboard->setText(text);
		emit textChanged();
	}
}
