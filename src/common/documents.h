#ifndef ASD_DOCUMENTS_H
#define ASD_DOCUMENTS_H

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

#endif //ASD_DOCUMENTS_H
