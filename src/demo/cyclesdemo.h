#ifndef KRUSKAL_CYCLESDEMO_H
#define KRUSKAL_CYCLESDEMO_H

#include <QDir>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtSvgWidgets/QGraphicsSvgItem>

#include "GraphDemo.h"
#include "graph/Graph.h"
#include "../common/constants.h"
#include "../common/documents.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CyclesDemo; }
QT_END_NAMESPACE

class CyclesDemo : public QWidget, public GraphDemo
{
Q_OBJECT

public:
	/// конструктор класса
	explicit CyclesDemo(QWidget * parent = nullptr);
	
	/// деструктор класса
	~CyclesDemo() override;
	
private:
	Ui::CyclesDemo * ui;
	QGraphicsTextItem * stackView; ///< просмотр стека на графической сцене

public slots:
	/// нажатие на кнопку предыдущего шага
	void on_prevStepButton_clicked();
	/// нажатие на кнопку следующего шага
	void on_nextStepButton_clicked();
	/// нажатие на кнопку перемещения в начало демонстрации
	void on_toBeginOfDemoButton_clicked();
	/// нажатие на кнопку перемещения в конец демонстрации
	void on_toEndOfDemoButton_clicked();
	/// нажатие на кнопку начала демонстрации
	void on_startButton_clicked();
    /// нажатие на кнопку смены демонстрации
    void on_switchDemoButton_clicked() { emit switchDemo(); }
	
private slots:
	/// сохранение снапшота графической сцены
	void takeSnapshot(QGraphicsScene* sceneToSave);
	/// вывод стека на графическую сцену
	void displayStack(const std::stack<std::pair<Vertex *, Vertex *>> & st) noexcept;
	/// нажатие на кнопку перехода к демонстрации алгоритма Краскала
    
signals:
	/// сигнал перехода к демонстрации алгоритма Краскала
	void switchDemo();
};

#endif // KRUSKAL_CYCLESDEMO_H
