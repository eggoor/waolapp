#ifndef ECLIPBOARD_H
#define ECLIPBOARD_H

#include <QQmlEngine>

class EClipboard : public QObject {
	Q_OBJECT
	QML_ELEMENT
	Q_DISABLE_COPY(EClipboard)

	Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
	explicit EClipboard(QObject* parent = nullptr);

	QString text() const;
	void setText(const QString& text);

signals:
	void textChanged();

private:
	class QClipboard* clipboard;
};

#endif // ECLIPBOARD_H
