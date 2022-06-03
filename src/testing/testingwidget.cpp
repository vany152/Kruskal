#include <random>
#include <numeric>
#include <ctime>

#include "testingwidget.h"
#include "ui_testingwidget.h"
#include "../common/constants.h"

using namespace std;

TestingWidget::TestingWidget(const std::shared_ptr<SessionStat> & stat, QWidget * parent) :
        QWidget(parent), ui(new Ui::TestingWidget), currentQuestion(0), currentSessionStat(stat)
{
	ui->setupUi(this);
	
	readQuestions();
	displayCurrentQuestion();
}

TestingWidget::~TestingWidget()
{
	delete ui;
}

/**
 * @brief добавление вопросов из файла в список вопросов
 * @details вопросы добавляются в случайном порядке
 */
void TestingWidget::readQuestions()
{
	/*
	 * читаем вопросы из файла с вопросами
	 */
	QJsonObject testingConfig;
	try { testingConfig = readJson(configPath).find("testing")->toObject(); }
	catch (FileError & err) { err.Show(); throw err; }
	QString questionsFile = testingConfig.find("questionsSource")->toString();
	
	QJsonObject questionsJson;
	try { questionsJson = readJson(questionsFile); }
	catch (FileError & err) { err.Show(); throw err; }
	
	/*
	 * формируем порядок вопросов.
	 * сформированный массив содержит порядок вопросов
	 */
	// количество вопросов = минимум(количество имеющихся вопросов, записанное в файле конфигов количество)
	size_t numberOfQuestions = min(static_cast<int>(questionsJson.size()), testingConfig.find("amountOfQuestions")->toInt());
	vector<int> questionsOrder(numberOfQuestions);
	iota(questionsOrder.begin(), questionsOrder.end(), 1); // заполняем порядок числами от 1
	// случайным образом перемешиваем массив с номерами вариантов ответа
	shuffle(questionsOrder.begin(), questionsOrder.end(), mt19937(time(nullptr)));
	
	QJsonObject question;
	for (auto & number : questionsOrder)
	{
		question = questionsJson.find(QString::number(number))->toObject();
		
		auto questionWidget = make_shared<QuestionWidget>(
				make_shared<Question>(question),
				this,
				ui->questionTextBrowser
		);
		questions.push_back(questionWidget);
	}

	amountOfQuestions = questions.size();
}

void TestingWidget::displayCurrentQuestion()
{
	ui->formLayout->insertRow(1, questions[currentQuestion].get());
	questions[currentQuestion]->Display();
}

void TestingWidget::closeEvent(QCloseEvent * e)
{
	emit closed();
	e->accept();
}

void TestingWidget::on_answerButton_clicked()
{
	questions[currentQuestion]->hide();
	ui->formLayout->takeRow(1);

	currentSessionStat->AddQuestionRate(
			questions[currentQuestion]->GetQuestion(),
			questions[currentQuestion]->RateAnswer()
	);
	
	if (++currentQuestion < amountOfQuestions) { displayCurrentQuestion(); }
	else { close(); }
}

