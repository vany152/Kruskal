#include <set>

#include "questionwidget.h"
#include "ui_questionwidget.h"

QuestionWidget::QuestionWidget(const shared_ptr<Question> & question, QWidget * parent, QTextBrowser * questionTextBrowser) :
		QWidget(parent), questionTextBrowser(questionTextBrowser), ui(new Ui::QuestionWidget),
		question(question)
{
	ui->setupUi(this);

	formAnswers();
}

QuestionWidget::~QuestionWidget()
{
	delete ui;
}

/// вывод элементов вопроса на форму
void QuestionWidget::Display()
{
	questionTextBrowser->setText(question->GetQuestion());
	show();
}

/**
 * @brief оценка ответа на вопрос
 * @return количество баллов
 */
int QuestionWidget::RateAnswer()
{
	std::set<int> userAnswer;
	for (int i = 0; i < answersCheckBoxes.size(); ++i)
	{
		if (answersCheckBoxes[i]->isChecked())
		{ userAnswer.insert(i + 1); }
	}
	
	return question->RateAnswer(userAnswer);
}

shared_ptr<Question> QuestionWidget::GetQuestion()
{
	return question;
}

/// формирование ответов: заполнение массива с вариантами ответа CheckBox'ами
void QuestionWidget::formAnswers()
{
	vector<QString> questionAnswers = question->GetAnswers();
	for (const auto & answer : questionAnswers)
	{
		QCheckBox * answerCheckBox = new QCheckBox(this);
		answerCheckBox->setText(answer);
		
		ui->verticalLayout->addWidget(answerCheckBox);
		answersCheckBoxes.push_back(answerCheckBox);
	}
	hide();
}

