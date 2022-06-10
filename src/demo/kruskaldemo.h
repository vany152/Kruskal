#ifndef KRUSKAL_KRUSKALDEMO_H
#define KRUSKAL_KRUSKALDEMO_H

#include <QDir>
#include <QWidget>
#include <QGraphicsScene>
#include <QtSvgWidgets/QGraphicsSvgItem>

#include "GraphDemo.h"
#include "graph/Graph.h"
#include "../common/constants.h"
#include "../common/documents.h"

QT_BEGIN_NAMESPACE
namespace Ui { class KruskalDemo; }
QT_END_NAMESPACE

class KruskalDemo : public QWidget, public GraphDemo
{
Q_OBJECT

public:
	/// конструктор класса
	explicit KruskalDemo(QWidget * parent = nullptr);

	/// деструктор класса
	~KruskalDemo() override;

private:
	Ui::KruskalDemo * ui;
	std::unique_ptr<Graph> mst; ///< минимальное остовное дерево графа

public slots:
    /// нажатие на кнопку предыдущего шага
    void on_prevStepButton_clicked();
    /// нажатие на кнопку следующего шага
    void on_nextStepButton_clicked();
    /// нажатие на кнопку перехода в начало демонстрации
    void on_toBeginOfDemoButton_clicked() ;
    /// нажатие на кнопку перехода в конец демонстрации
    void on_toEndOfDemoButton_clicked();
    /// нажатие на кнопку смены демонстрации
    void on_switchDemoButton_clicked() { emit switchDemo(); }
    /// сохранение снапшота графической сцены
    void takeSnapshot(QGraphicsScene * sceneToSave);
    
signals:
    /// сигнал перехода к демонстрации алгоритма проверки включения ребра в цикл
    void switchDemo();
};

#endif // KRUSKAL_KRUSKALDEMO_H
