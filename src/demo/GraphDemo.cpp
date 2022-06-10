#include <random>
#include <ctime>
#include <list>

#include "GraphDemo.h"
#include "../common/Error.h"

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

GraphDemo::GraphDemo(
    QWidget * parent,
    const QString & tempDirName,
    const QRect & sceneRect,
    const QRect & internalSceneRect
) :
    scene(new QGraphicsScene(parent)),
    internalScene(new QGraphicsScene(nullptr))
{
    /*
     * временная директория
     */
    tmp = QDir::temp();
    if (!tmp.mkpath(tempDirName))
    {
        tmp.cd(tempDirName);
        tmp.removeRecursively();
        tmp.mkpath(tempDirName);
    }
    tmp.cd(tempDirName);
    
    /*
     * установки графических сцен
     */
    internalScene->setSceneRect(internalSceneRect);
    scene->setSceneRect(sceneRect);
    
    /*
     * основной граф
     */
    graph = generateGraph();
//    graph->PrepareToPaint(QPoint(0, 0), 150, internalScene);
//    graph->Paint(Qt::black, 0xD68910);
}

GraphDemo::~GraphDemo()
{
    delete internalScene;
    for (auto & demoStep: demoSteps) { delete demoStep; } // удаляем шаги демонстрации
    delete scene;
    
    tmp.removeRecursively(); // удаляем временную папку
}

/** *************************************************** PRIVATE **************************************************** **/

unique_ptr<Graph> GraphDemo::generateGraph() noexcept
{
    static mt19937 gen(time(nullptr));
    
    /*
     * минимальное и максимальное число вершин графа
     */
    size_t minNumberOfVertexes = 6;
    size_t maxNumberOfVertexes = 7;
    uniform_int_distribution<size_t> randomNumberOfVertexes(minNumberOfVertexes, maxNumberOfVertexes);
    size_t numberOfVertexes = randomNumberOfVertexes(gen);
    /*
     * минимальное и максимальное число ребер графа
     */
    size_t minNumberOfEdges = numberOfVertexes + 1;
    size_t maxNumberOfEdges = numberOfVertexes * 2 - 5;
    uniform_int_distribution<size_t> randomNumberOfEdges(minNumberOfEdges, maxNumberOfEdges);
    size_t numberOfEdges = randomNumberOfEdges(gen);
    /*
     * минимальная и максимальная стоимость ребра
     */
    int minEdgeCost = 5;
    int maxEdgeCost = 50;
    
    unique_ptr<Graph> newGraph = std::make_unique<Graph>(numberOfVertexes);
    newGraph->RandomlyLinkVertices(numberOfEdges, minEdgeCost, maxEdgeCost);
    
    return newGraph;
}

/// загрузка снапшотов в память программы
void GraphDemo::loadDemo()
{
    QFileInfoList content = tmp.entryInfoList(QDir::Files, QDir::Name);
    for (auto& file : content)
    {
        if (!file.exists()) { throw FileError("не удалось прочитать файл " + file.absolutePath().toStdString()); }
        demoSteps.push_back(new QGraphicsSvgItem(file.absoluteFilePath()));
    }
}
