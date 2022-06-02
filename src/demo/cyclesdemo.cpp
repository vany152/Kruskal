#include <random>
#include <stack>
#include <array>

#include "cyclesdemo.h"
#include "ui_cyclesdemo.h"
#include "../common/Error.h"

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

CyclesDemo::CyclesDemo(QWidget * parent):
		QWidget(parent),
		ui(new Ui::CyclesDemo),
        GraphDemo(parent, "_cycles", QRect(0, 0, 550, 350), QRect(-300, -175, 550, 350))
{
	ui->setupUi(this);
	
	/*
	 * отключение некоторых кнопок
	 */
	ui->nextStepButton->setEnabled(false);
	ui->prevStepButton->setEnabled(false);
	ui->toBeginOfDemoButton->setEnabled(false);
	ui->toEndOfDemoButton->setEnabled(false);
 
	ui->graphicsView->setScene(scene);
	
	/*
	 * установка просмотра стека ребер
	 */
	auto mainSceneStackView = scene->addText("Стек ребер:\n");
	mainSceneStackView->setPos(0, 25);
	stackView = internalScene->addText("Стек ребер:\n");
	stackView->setPos(-300, -150);
	
	/*
	 * отрисовка графа на основной сцене
	 */
	graph->PrepareToPaint(QPoint(300, 150), 135, scene);
	graph->Paint(Qt::black, 0xD68910);
 
	/*
	 * отрисовка графа на внутренней сцене
	 */
	graph->PrepareToPaint(QPoint(0, -25), 135, internalScene);
	graph->Paint(Qt::black, 0xD68910);
	
	/*
	 * добавление в comboBox ребер графа
	 */
	for (auto & edge : graph->edges) { ui->chooseEdgeComboBox->addItem(edge.GetName()); }
	
	connect(graph.get(), &Graph::takeSnapshot, this, &CyclesDemo::takeSnapshot);
	connect(graph.get(), &Graph::stackChanged, this, &CyclesDemo::displayStack);
}

CyclesDemo::~CyclesDemo()
{
	delete ui;
}

/** **************************************************** SLOTS ***************************************************** **/

/// нажатие на кнопку предыдущего шага
void CyclesDemo::on_prevStepButton_clicked()
{
	if (currentStep != demoSteps.begin())
	{
		scene->removeItem(*currentStep);
		--currentStep;
		scene->addItem(*currentStep);
	}
}

/// нажатие на кнопку следующего шага
void CyclesDemo::on_nextStepButton_clicked()
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

/// нажатие на кнопку перемещения в начало демонстрации
void CyclesDemo::on_toBeginOfDemoButton_clicked()
{
	scene->removeItem(*currentStep);
	currentStep = demoSteps.begin();

	scene->addItem(static_cast<QGraphicsItem*>(*currentStep));
}

/// нажатие на кнопку перемещения в конец демонстрации
void CyclesDemo::on_toEndOfDemoButton_clicked()
{
	scene->removeItem(*currentStep);
	currentStep = --demoSteps.end();

	scene->addItem(static_cast<QGraphicsItem*>(*currentStep));
}

/// нажатие на кнопку начала демонстрации
void CyclesDemo::on_startButton_clicked()
{
    /*
     * включаем и отключаем некоторые кнопки
     */
	ui->nextStepButton->setEnabled(true);
	ui->prevStepButton->setEnabled(true);
	ui->toBeginOfDemoButton->setEnabled(true);
	ui->toEndOfDemoButton->setEnabled(true);
	
	ui->startButton->setEnabled(false);
	ui->chooseEdgeComboBox->setEnabled(false);
    
    /*
     * формируем демонстрацию проверки включения ребра в цикд
     */
	std::string edgeName = ui->chooseEdgeComboBox->currentText().toStdString();
	graph->CheckEdgeInCycleDemo(graph->FindEdge(edgeName.at(0), edgeName.at(1)));
	
	try { loadDemo(); } // загружаем в память этапы демонстрации
	catch (FileError & err) { err.Show(); close(); return; }
	currentStep = demoSteps.begin(); // выставляем указатель текущего шага на
	scene->clear();
	scene->addItem(*currentStep); // выводим на сцену первый шаг демонстрации
}

/// сохранение снапшота графической сцены
void CyclesDemo::takeSnapshot(QGraphicsScene* sceneToSave)
{
	QString p(QString::number(snapshotNumber++) + ".svg");
	try { saveSvg(sceneToSave, tmp.absoluteFilePath(p)); } // сохраняем сцену в svg
	catch (FileError & err) { err.Show(); }
}

/// вывод стека на графическую сцену
void CyclesDemo::displayStack(const stack<pair<Vertex *, Vertex *>> & st) noexcept
{
	stack<pair<Vertex *, Vertex *>> edges, _st = st;
    while (!_st.empty()) { edges.push(_st.top()); _st.pop(); }
	
	QString stackStr("Стек ребер:\n");
	while (!edges.empty())
	{
		stackStr += edges.top().first->GetLabel();
		stackStr += edges.top().second->GetLabel();
		stackStr += '\n';
		edges.pop();
	}
	
	stackView->setPlainText(stackStr);
	stackView->setPos(-300, -150);
}


