#ifndef KRUSKAL_DOCUMENTS_H
#define KRUSKAL_DOCUMENTS_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsScene>
#include <QtSvg>

/// чтение данных из json файла
QJsonObject readJson(const QString & filename);

/// запись данных в json файл
void writeJson(const QJsonObject & obj, const QString & filename);

/// сохранение графической сцены в svg файл
void saveSvg(QGraphicsScene * scene, const QString & path);

#endif // KRUSKAL_DOCUMENTS_H
