#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace {
#include <QString>
}

const double PI = 3.14159265358979323846; // чтобы не включать <cmath>

#ifdef NDEBUG
const QString configPath = "resx/config.json"; ///< путь к файлу с конфигами
const QString databasePath = "resx/stat.sqlite"; ///< путь к файлу с базой данных
#else
const QString configPath = "D:/dev/Kruskal/resx/config.json"; ///< путь к файлу с конфигами
const QString databasePath = "D:/dev/Kruskal/resx/stat.sqlite"; ///< путь к файлу с базой данных
#endif

#endif //CONSTANTS_H
