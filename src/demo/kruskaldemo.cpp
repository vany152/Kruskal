#include <list>

#include "kruskaldemo.h"
#include "ui_kruskaldemo.h"
#include "../common/Error.h"

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

KruskalDemo::KruskalDemo(QWidget * parent) :
		QWidget(parent),
        ui(new Ui::KruskalDemo),
        GraphDemo(parent, "_kruskal", QRect(0, 0, 800, 400), QRect(-200, -200, 800, 400))
{
	ui->setupUi(reinterpret_cast<QWidget *>(this));
    
    /*
     * устанавливаем основную графическую сцену
     */
	ui->graphicsView->setScene(scene);
	
	/*
	 * основной граф
	 */
	graph->PrepareToPaint(QPoint(0, 0), 150, internalScene);
	graph->Paint(Qt::black, 0xD68910);

	connect(graph.get(), &Graph::takeSnapshot, this, &KruskalDemo::takeSnapshot);
	
	/*
	 * минимальное остовное дерево
	 */
	mst = make_unique<Graph>(Graph(*(graph)));
	mst->PrepareToPaint(QPoint(380, 0), 150, internalScene);
	mst->Paint(Qt::black, 0xD68910);
	mst->HideEdges();
	
	graph->Kruskal(*mst); // включаем на mst ребра, входящие в МОД графа graph
	
	try { loadDemo(); } // загружаем в память этапы демонстрации
	catch (FileError & err) { err.Show(); close(); return; }
	currentStep = demoSteps.begin(); // выставляем указатель текущего шага на
	scene->addItem(*currentStep); // выводим на сцену первый шаг демонстрации
}

KruskalDemo::~KruskalDemo()
{
	delete ui;
}

/** *************************************************** PRIVATE **************************************************** **/

/// нажатие на кнопку предыдущего шага
void KruskalDemo::on_prevStepButton_clicked()
{
	if (currentStep != demoSteps.begin())
	{
		scene->removeItem(*currentStep);
		--currentStep;
		scene->addItem(*currentStep);
	}
}

/// нажатие на кнопку следующего шага
void KruskalDemo::on_nextStepButton_clicked()
{
	if (++currentStep != demoSteps.end())
	{
		--currentStep;
		scene->removeItem(*currentStep);
		++currentStep;
		scene->addItem(*currentStep);
	}
	else
	{
		--currentStep;
	}
}

/// нажатие на кнопку перехода в начало демонстрации
void KruskalDemo::on_toBeginOfDemoButton_clicked()
{
	scene->removeItem(*currentStep);
	currentStep = demoSteps.begin();

	scene->addItem(static_cast<QGraphicsItem*>(*currentStep));
}

/// нажатие на кнопку перехода в конец демонстрации
void KruskalDemo::on_toEndOfDemoButton_clicked()
{
	scene->removeItem(*currentStep);
	currentStep = --demoSteps.end();

	scene->addItem(static_cast<QGraphicsItem *>(*currentStep));
}

/// сохранение снапшота графической сцены
void KruskalDemo::takeSnapshot(QGraphicsScene * sceneToSave)
{
	QString p(QString::number(snapshotNumber++) + ".svg");
	try { saveSvg(sceneToSave, tmp.absoluteFilePath(p)); } // сохраняеv сцену в svg
	catch (FileError & err) { err.Show(); }
}

