#ifndef ASD_CONSTANTS_H
#define ASD_CONSTANTS_H

namespace {
#include <QString>
}

/// отвечает за включение отладочного кода
#ifndef NDEBUG
  #define DEBUG
#endif

#define PI 3.14159265358979323846 // чтобы не включать <cmath>

#ifdef DEBUG
const QString configPath = "D:/dev/Kruskal/resx/config.json"; ///< путь к файлу с конфигами
const QString databasePath = "D:/dev/Kruskal/resx/stat.sqlite"; ///< путь к файлу с базой данных
#else
const QString configPath = "resx/config.json"; ///< путь к файлу с конфигами
const QString databasePath = "resx/stat.sqlite"; ///< путь к файлу с базой данных
#endif

#endif //ASD_CONSTANTS_H
