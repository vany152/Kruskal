#include <random>
#include <ctime>
#include <list>
#include <stack>
#include <array>

#include "cyclesdemo.h"
#include "ui_cyclesdemo.h"
#include "../common/Error.h"

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

CyclesDemo::CyclesDemo(QWidget * parent) noexcept:
		QWidget(parent),
		ui(new Ui::CyclesDemo),
		scene(new QGraphicsScene(parent)),
		internalScene(new QGraphicsScene(nullptr))
{
	ui->setupUi(this);
	
	/*
	 * отключение некоторых кнопок
	 */
	ui->nextStepButton->setEnabled(false);
	ui->prevStepButton->setEnabled(false);
	ui->toBeginOfDemoButton->setEnabled(false);
	ui->toEndOfDemoButton->setEnabled(false);
	
	/*
	 * установка графических сцен
	 */
	internalScene->setSceneRect(-300, -175, 550, 350);
	scene->setSceneRect(0, 0, 550, 350);
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
	graph = generateGraph();
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
	
	delete internalScene;
	for (auto& demoStep : demoSteps) { delete demoStep; } // удаляем шаги демонстрации
	delete scene;

    if (tmp.absolutePath().contains("_cycles")) { tmp.removeRecursively(); } // удаляем временную папку
}

/** *************************************************** PRIVATE **************************************************** **/

shared_ptr<Graph> CyclesDemo::generateGraph() noexcept
{
	mt19937 gen(time(nullptr));
	
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

	shared_ptr<Graph> newGraph = std::make_shared<Graph>(
			Graph(
					numberOfVertexes,
					numberOfEdges,
					minEdgeCost,
					maxEdgeCost
			)
	);
	newGraph->Generate();

	return newGraph;
}

/// загрузка снапшотов в память программы
void CyclesDemo::loadDemo()
{
	QFileInfoList content = tmp.entryInfoList(QDir::Files, QDir::Name);
	for (auto& file : content)
	{
		if (!file.exists()) { throw FileError("не удалось прочитать файл " + file.absolutePath().toStdString()); }
		demoSteps.push_back(new QGraphicsSvgItem(file.absoluteFilePath()));
	}
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
	ui->nextStepButton->setEnabled(true);
	ui->prevStepButton->setEnabled(true);
	ui->toBeginOfDemoButton->setEnabled(true);
	ui->toEndOfDemoButton->setEnabled(true);
	
	ui->startButton->setEnabled(false);
	ui->chooseEdgeComboBox->setEnabled(false);
	
	/*
	 * временная директория
	 */
	tmp = QDir::temp();
    if (!tmp.mkpath("_cycles"))
	{
        tmp.cd("_cycles");
		tmp.removeRecursively();
        tmp.mkpath("_cycles");
	}
    tmp.cd("_cycles");

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
	try { saveSvg(sceneToSave, tmp.absoluteFilePath(p)); } // сохраняеv сцену в svg
	catch (FileError & err) { err.Show(); }
}

/// вывод стека на графическую сцену
void CyclesDemo::displayStack(const stack<array<Vertex *, 2>> & st) noexcept
{
	stack<array<Vertex *, 2>> edges, _st = st;
    while (!_st.empty()) { edges.push(_st.top()); _st.pop(); }
	
	QString stackStr("Стек ребер:\n");
	while (!edges.empty())
	{
		stackStr += edges.top().at(0)->GetLabel();
		stackStr += edges.top().at(1)->GetLabel();
		stackStr += '\n';
		edges.pop();
	}
	
	stackView->setPlainText(stackStr);
	stackView->setPos(-300, -150);
}


