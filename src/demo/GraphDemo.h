#ifndef KRUSKAL_GRAPHDEMO_H
#define KRUSKAL_GRAPHDEMO_H

#include <QDir>
#include <QWidget>
#include <QGraphicsScene>
#include <QtSvgWidgets/QGraphicsSvgItem>

#include "graph/Graph.h"
#include "../common/constants.h"
#include "../common/documents.h"

class GraphDemo
{
    using DemoStepsIterator = std::list<QGraphicsSvgItem*>::iterator;

public:
    GraphDemo(QWidget * parent, const QString & tempDirName, const QRect & sceneRect, const QRect & internalSceneRect);
    virtual ~GraphDemo() = 0;
    
protected:
    QGraphicsScene * scene; ///< основная сцена, на которую выводится пошаговая демонстрация
    QGraphicsScene * internalScene; ///< графическая сцена, на которой располагается граф
    std::unique_ptr<Graph> graph; ///< граф
    
    std::list<QGraphicsSvgItem *> demoSteps; ///< список шагов демонстрации
    DemoStepsIterator currentStep; ///< текущий шаг демонстрации
    
    QDir tmp; ///< временная директория, в которую сохраняются снапшоты графической сцены для пошаговй демонстрации
    /*
     * если именовать файлы с 0 при большом количестве шагов (больше 10) возникают проблемы сортировки файлов со
     * снапшотами по имени: 1 < 11 < 2
     */
    int snapshotNumber = 1000; ///< номер текущего снапшота
    
protected:
    /// генерация случайных параметров для создания графа
    static std::unique_ptr<Graph> generateGraph() noexcept;
    
    /// загрузка снапшотов в память программы
    void loadDemo();
};

#endif //KRUSKAL_GRAPHDEMO_H
