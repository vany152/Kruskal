#include <QGridLayout>
#include <QTextBrowser>

#include "testingsystem.h"
#include "ui_testingsystem.h"

#include "../common/constants.h"
#include "../common/settingswindow.h"
#include "stat/statdatabaseviewwidget.h"

/** **************************************************** PUBLIC **************************************************** **/

TestingSystem::TestingSystem(QWidget * parent) :
        QMainWindow(parent),
        ui(new Ui::TestingSystem),
		welcomeToTestingWidget(nullptr),
		testingWidget(nullptr),
		statWidget(nullptr)
{
	ui->setupUi(this);
	setWindowTitle("Тестирование");
	welcomeToTestingWidget = new WelcomeToTesting();
	ui->gridLayout->addWidget(welcomeToTestingWidget, 0, 0);

#ifndef NOWEBENGINE
    viewer = new HtmlViewer(this);
    viewer->SetWindowTitle("Справка");
#endif
	
	connect(welcomeToTestingWidget, &WelcomeToTesting::testingRulesButtonClicked, this, &TestingSystem::showTestingRules);
	connect(ui->testingRulesMenu, &QAction::triggered, this, &TestingSystem::showTestingRules);
	connect(welcomeToTestingWidget, &WelcomeToTesting::closed, this, &TestingSystem::startTest);
    connect(ui->editQuestionsRulesMenu, &QAction::triggered, this, &TestingSystem::showEditQuestionsRules);
}

TestingSystem::~TestingSystem()
{
	if (welcomeToTestingWidget) { delete welcomeToTestingWidget; }
	if (testingWidget) { delete testingWidget; }
	if (statWidget) { delete statWidget; }
	delete ui;
}

/** **************************************************** PRIVATE *************************************************** **/

void TestingSystem::showRef(const QString & refSectionName)
{
#ifndef NOWEBENGINE
    QJsonObject config;
    try { config = readJson(configPath); }
    catch (FileError & err) { err.Show(); close(); return; }
    
    viewer->Open(config.find(refSectionName)->toString());
    viewer->show();
#endif
}

/** ***************************************************** SLOTS **************************************************** **/

/// выбран пункт меню "настройки"
void TestingSystem::on_openSettingsMenu_triggered()
{
    SettingsWindow::Instance().show();
}

/// вывод виджета с базой данных статистики
void TestingSystem::on_statMenu_triggered()
{
	StatDatabaseViewWidget * statView = new StatDatabaseViewWidget(databasePath, nullptr);
	statView->show();
}

/// запуск теста
void TestingSystem::startTest(const QString & username)
{
	/*
	 * отключаем пункт запуска настроек в меню
	 */
	ui->openSettingsMenu->setEnabled(false);
	
	/*
	 * создаем класс, который будет хранить статистику
	 */
	currentSessionStat.reset(new SessionStat(username));
	currentSessionStat->SetStartTime(QDateTime::currentDateTime());
	
	/*
	 * создаем виджет, на который будут выводиться вопросы
	 */
	try { testingWidget = new TestingWidget(currentSessionStat); }
    catch (FileError) { close(); return; }
	
	ui->gridLayout->addWidget(testingWidget, 0, 0);
	testingWidget->show();
	
	connect(testingWidget, &TestingWidget::closed, this, &TestingSystem::showStat);
	connect(testingWidget, &TestingWidget::closed, this, &TestingSystem::saveStat);
}

/// сохранение статистики в базу данных
void TestingSystem::saveStat()
{
	currentSessionStat->Save(databasePath);
}

/// завершение теста и вывод статистики
void TestingSystem::showStat()
{
	/*
	 * включаем пункт запуска настроек в меню
	 */
	ui->openSettingsMenu->setEnabled(true);
	
	/*
	 * сохраняем время завершения теста
	 */
	currentSessionStat->SetFinishTime(QDateTime::currentDateTime());
	
	/*
	 * создаем виджет, на который будет выведена статистика
	 */
	try { statWidget = new SessionStatWidget(currentSessionStat, ui->centralwidget); }
    catch (SessionStat::StatError) { close(); return; }
	ui->gridLayout->addWidget(statWidget, 0, 0);
	statWidget->show();
}

/// вывод диалогового окна с правилами тестирования
void TestingSystem::showTestingRules()
{
    showRef("testingRules");
}

void TestingSystem::showEditQuestionsRules()
{
    showRef("questionsDatabaseEditRef");
}
