#include <random>
#include <ctime>
#include <list>

#include "kruskaldemo.h"
#include "ui_kruskaldemo.h"
#include "../common/Error.h"

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

KruskalDemo::KruskalDemo(QWidget * parent) :
		QWidget(parent),
		ui(new Ui::KruskalDemo),
		scene(new QGraphicsScene(parent)),
		internalScene(new QGraphicsScene(nullptr))
{
	ui->setupUi(this);
	
	/*
	 * временная директория
	 */
	tmp = QDir::temp();
	if (!tmp.mkpath("_kruskal"))
	{
		tmp.cd("_kruskal");
		tmp.removeRecursively();
		tmp.mkpath("_kruskal");
	}
	tmp.cd("_kruskal");
	
	/*
	 * установки графических сцен
	 */
	internalScene->setSceneRect(-200, -200, 800, 400);
	scene->setSceneRect(0, 0, 800, 400);
	ui->graphicsView->setScene(scene);
	
	/*
	 * основной граф
	 */
	graph = generateGraph();
	graph->PrepareToPaint(QPoint(0, 0), 150, internalScene);
	graph->Paint(Qt::black, 0xD68910);

	connect(graph.get(), &Graph::takeSnapshot, this, &KruskalDemo::takeSnapshot);
	
	/*
	 * минимальное остовное дерево
	 */
	mst = make_shared<Graph>(Graph(*graph));
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
	
	delete internalScene;
	for (auto & demoStep: demoSteps) { delete demoStep; } // удаляем шаги демонстрации
	delete scene;
	
    if (tmp.absolutePath().contains("_kruskal")) { tmp.removeRecursively(); } // удаляем временную папку
}

/** *************************************************** PRIVATE **************************************************** **/

shared_ptr<Graph> KruskalDemo::generateGraph() noexcept
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
void KruskalDemo::loadDemo()
{
	QFileInfoList content = tmp.entryInfoList(QDir::Files, QDir::Name);
	for (auto& file : content)
	{
		if (!file.exists()) { throw FileError("не удалось прочитать файл " + file.absolutePath().toStdString()); }
		demoSteps.push_back(new QGraphicsSvgItem(file.absoluteFilePath()));
	}
}

/** **************************************************** SLOTS +**************************************************** **/

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

