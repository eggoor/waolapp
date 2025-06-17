#ifndef ETEXTREADER_H
#define ETEXTREADER_H

#include <QQmlEngine>

class ETextReader : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	Q_DISABLE_COPY(ETextReader)
public:
	explicit ETextReader(QObject* parent = nullptr);
	Q_INVOKABLE QString readFile(const QString& path);
};

#endif // ETEXTREADER_H
