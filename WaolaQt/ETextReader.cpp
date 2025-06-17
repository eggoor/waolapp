#include <QDirIterator>

#include "ETextReader.h"

ETextReader::ETextReader(QObject* parent)
	: QObject{parent}
{}

QString ETextReader::readFile(const QString& path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return {};

	QTextStream in(&file);
	return in.readAll();
}
