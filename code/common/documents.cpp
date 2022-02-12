#include "documents.h"
#include "Error.h"

#include <QFile>

/// чтение данных из json файла
QJsonObject readJson(const QString & filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		throw FileError("не удалось прочитать файл " + filename.toStdString());
	}
	
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	file.close();
	
	return doc.object();
}

/// запись данных в json файл
void writeJson(const QJsonObject & obj, const QString & filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		throw FileError("не удалось записать в файл " + filename.toStdString());
	}
	
	file.write(QJsonDocument(obj).toJson());
	file.close();
}

/// сохранение графической сцены в svg файл
void saveSvg(QGraphicsScene * scene, const QString & path)
{
	if (path.isEmpty()) { throw std::invalid_argument("путь к файлу не может быть пустым"); }

	QSize sceneSize = scene->sceneRect().size().toSize();

	QSvgGenerator generator;
	generator.setFileName(path);
	generator.setSize(sceneSize);
	generator.setResolution(90);

	QPainter painter;
	painter.begin(&generator);
	scene->render(&painter);
	painter.end();
}