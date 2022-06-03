#include <QGridLayout>
#include <QTextBrowser>

#include "testingsystem.h"
#include "ui_testingsystem.h"

#include "testingsettingswindow.h"
#include "stat/statdatabaseviewwidget.h"
#include "../common/documents.h"
#include "../common/constants.h"

/** **************************************************** PUBLIC **************************************************** **/

TestingSystem::TestingSystem(QWidget * parent) :
        QMainWindow(parent),
        ui(new Ui::TestingSystem),
		welcomeToTestingWidget(nullptr),
		testingWidget(nullptr),
		statWidget(nullptr),
        testingRulesDialog(nullptr)
{
	ui->setupUi(this);
	setWindowTitle("Тестирование");
	welcomeToTestingWidget = new WelcomeToTesting();
	ui->gridLayout->addWidget(welcomeToTestingWidget, 0, 0);
	
	testingRulesDialog = new QDialog();
	formTestingRulesDialog();
    editQuestionRulesDialog = new QDialog();
    formEditQuestionsRulesDialog();
	
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
	if (testingRulesDialog) { delete testingRulesDialog; }
    if (testingRulesDialog) { delete editQuestionRulesDialog; }
	delete ui;
}

/** **************************************************** PRIVATE *************************************************** **/

/// формирование окна с информацией о правилах тестирования
void TestingSystem::formTestingRulesDialog()
{
	testingRulesDialog->setFixedSize(700, 400);
	
	QGridLayout * gridLayout = new QGridLayout(testingRulesDialog);
	testingRulesDialog->setLayout(gridLayout);
	
	QTextBrowser * rulesTextBrowser = new QTextBrowser(testingRulesDialog);
	rulesTextBrowser->setStyleSheet("QTextBrowser { background-color : rgb(240, 240, 240) } ");
	rulesTextBrowser->setFrameShape(QFrame::NoFrame);
	rulesTextBrowser->setMarkdown(
			"# Правила тестирования\n"
			"Вам будет предложено несколько вопросов закрытого типа. На каждый вопрос имеется несколько "
			"вариантов ответа.\\\n"
			"Количество баллов, полученных за правильный ответ, варьируется и зависит от сложности вопроса:\n\n"
			"- простой - один балл\n"
			"- средний - два балла\n"
			"- сложный - три балла\n\n"
			"За верный ответ выставляется максимальное количество баллов по данному вопросу, за неверный - ноль\\\n"
			"Ответ на вопрос с невыбранными вариантами ответа считается неверным\n\n"
			"Для перехода к следующему вопросу необходимо нажать на кнопку \"ответить\"\\\n"
			"По окончании тестирования будет выведена статистика по текущему сеансу тестирования\\\n"
			"Статистика сохраняется в базу данных\n"
	);
	gridLayout->addWidget(rulesTextBrowser);
}

/// формирование окна с информацией о правилах редактирования вопрсов в файле
void TestingSystem::formEditQuestionsRulesDialog()
{
    editQuestionRulesDialog->setFixedSize(1000, 425);

    QGridLayout * gridLayout = new QGridLayout(testingRulesDialog);
    editQuestionRulesDialog->setLayout(gridLayout);

    QTextBrowser * rulesTextBrowser = new QTextBrowser(editQuestionRulesDialog);
    rulesTextBrowser->setStyleSheet("QTextBrowser { background-color : rgb(240, 240, 240) } ");
    rulesTextBrowser->setFrameShape(QFrame::NoFrame);
    rulesTextBrowser->setMarkdown(
            "# Редактирование базы вопросов\n"
            "База вопросов представлена набором элементов ключ-значение, где ключ - номер вопроса, обязательно заключенный в кавычки, "
            "а значение хранит элементы:\n"
            "* тело вопроса\n"
            "* варианты ответа\n"
            "* правильные варианты ответа\n"
            "* сложность вопроса, так же является количеством баллов за правильный ответ\n\n"
            "Пример:"
            "<pre><br/>"
            "\"3\" : {                                                    --- Номер вопроса. Записывается в кавычках<br/>"
            "      \"question\" : \"Кем был описан алгоритм Краскала?\",    --- Тело вопроса, записывается в кавычках<br/>"
            "      \"answers\" : [\"Джозефом Краскалом\",                   --- Варианты ответов. Записываются в кавычках<br/>"
            "                   \"Отакаром Борувкой\",<br/>"
            "                   \"Робертом Примом\",<br/>"
            "                   \"Войцехом Ярником\",<br/>"
            "                   \"Эдсгером Дейкстрой\"],<br/>"
            "      \"correctAnswers\" : [1, 4, 5],        --- Номера правильных вариантов ответа. Записываются без кавычек<br/>"
            "      \"complexity\" : 3                     --- Сложность вопроса<br/>"
            "  }"
            "</pre>"
    );
    gridLayout->addWidget(rulesTextBrowser);
}

/** ***************************************************** SLOTS **************************************************** **/

/// выбран пункт меню "настройки"
void TestingSystem::on_openSettingsMenu_triggered()
{
	TestingSettingsWindow * settings = new TestingSettingsWindow();
	settings->show();
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
	testingRulesDialog->exec();
}

void TestingSystem::showEditQuestionsRules()
{
    editQuestionRulesDialog->exec();
}
