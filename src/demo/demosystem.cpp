#include <QtSvgWidgets/QGraphicsSvgItem>
#include <QCloseEvent>

#include "demosystem.h"
#include "ui_demosystem.h"

DemoSystem::DemoSystem(QWidget * parent) :
		QMainWindow(parent), ui(new Ui::DemoSystem)
{
	ui->setupUi(this);
	setWindowTitle("Алгоритм Краскала");
	
	kruskalDemo = new KruskalDemo(nullptr);
	cyclesDemo = new CyclesDemo(nullptr);
	
	ui->gridLayout->addWidget(reinterpret_cast<QWidget *>(kruskalDemo), 0, 2, 1, 1);
	
	connect(kruskalDemo, &KruskalDemo::switchDemo, this, &DemoSystem::ShowCycles);
	connect(cyclesDemo, &CyclesDemo::switchDemo, this, &DemoSystem::ShowKruskal);
}

DemoSystem::~DemoSystem()
{
	delete ui;
}

/** *************************************************** PRIVATE **************************************************** **/

void DemoSystem::closeEvent(QCloseEvent * e)
{
	delete kruskalDemo;
	delete cyclesDemo;
	e->accept();
}

/** **************************************************** SLOTS *+*************************************************** **/

/// переход к демонстрации алгоритма Краскала
void DemoSystem::ShowKruskal()
{
	setWindowTitle("Алгоритм Краскала");
	ui->gridLayout->replaceWidget(cyclesDemo, kruskalDemo);
	cyclesDemo->setParent(nullptr);
}

/// переход к демонстрации алгоритма проверки включения ребра в цикл
void DemoSystem::ShowCycles()
{
	setWindowTitle("Проверка включения ребра в цикл");
	ui->gridLayout->replaceWidget(kruskalDemo, cyclesDemo);
	kruskalDemo->setParent(nullptr);
}
