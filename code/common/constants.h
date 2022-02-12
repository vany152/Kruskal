#ifndef ASD_CONSTANTS_H
#define ASD_CONSTANTS_H

#include <QString>

/// отвечает за включение отладочного кода
//#define DEBUG // todo изменить при компиляции в релизе!

#define PI 3.14159265358979323846 // чтобы не включать <cmath>

#ifdef DEBUG
const QString configPath = "D:/study/asd/asd/coursework/code/config.json"; ///< путь к файлу с конфигами
const QString databasePath = "D:/study/asd/asd/coursework/src/stat.sqlite"; ///< путь к файлу с базой данных
#else
const QString configPath = "src/config.json"; ///< путь к файлу с конфигами
const QString databasePath = "src/stat.sqlite"; ///< путь к файлу с базой данных
#endif

#endif //ASD_CONSTANTS_H
