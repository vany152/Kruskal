#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "theory/theorysystem.h"
#include "testing/testingsystem.h"
#include "demo/demosystem.h"

/** **************************************************** PUBLIC **************************************************** **/

MainWindow::MainWindow(QWidget * parent) :
		QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle("Алгоритм Краскала");
	displayHelloText();
}

MainWindow::~MainWindow()
{
	delete ui;
}

/** *************************************************** PRIVATE **************************************************** **/

/// вывод приветствия в текстовое поле
void MainWindow::displayHelloText()
{
	ui->welcomeTextBrowser->setHtml(
			"Астраханский государственный университет <br/>\n"
			"Институт информационных технологий и коммуникации\n"
			"<br/>\n"
			"\n"
			"<p align=\"right\"> \n"
			"Кафедра <br/>\n"
			"Автоматические системы <br/>\n"
			"обработки информации и управления\n"
			"</p>\n"
			"<br/>\n"
			"\n"
			"<h4 align=\"center\">\n"
			"Курсовой проект по дисциплине <br/> \n"
			"\"Алгоритмы и структуры данных\" <br/> \n"
			"<font color=\"green\"> Учебно-демонстрационная программа <br/> \n"
			"\"Алгоритм Краскала поиска минимального остовного дерева\" </font>\n"
			"</h4>\n"
			"<br/>\n"
			"\n"
			"<p align=\"right\"> \n"
			"Выполнил студент группы ДИПРб-21 <br/> \n"
			"Ермолаев Иван Дмитриевич\n"
			"</p>\n"
			"\n"
			"<p align='center'> г. Астрахань, 2021 </p>\n"
	);
}

/** ***************************************************** SLOTS **************************************************** **/

/// запуск окна с теорией
void MainWindow::on_theoryButton_clicked()
{
//    TheorySystem * theorySystemWindow = new TheorySystem(this);
//    theorySystemWindow->show();
}

/// запуск окна с демонстрацией
void MainWindow::on_demoButton_clicked()
{
	DemoSystem * demoSystem = new DemoSystem(this);
	demoSystem->show();
}

/// запуск окна с тестированием
void MainWindow::on_testingButton_clicked()
{
	TestingSystem * testingSystemWindow = new TestingSystem(this);
	testingSystemWindow->show();
}


