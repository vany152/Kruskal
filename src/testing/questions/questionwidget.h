#ifndef KRUSKAL_QUESTIONWIDGET_H
#define KRUSKAL_QUESTIONWIDGET_H

#include <vector>

#include <QWidget>
#include <QCheckBox>
#include <QTextBrowser>

#include "question.h"

using std::vector;
using std::shared_ptr;

QT_BEGIN_NAMESPACE
namespace Ui { class QuestionWidget; }
QT_END_NAMESPACE

class QuestionWidget : public QWidget
{
Q_OBJECT

public:
	QuestionWidget(const shared_ptr<Question> & question, QWidget * parent = nullptr, QTextBrowser * questionTextBrowser = nullptr);
	~QuestionWidget() override;
	
	/// вывод элементов вопроса на форму
	void Display();

	/**
	 * @brief оценка ответа на вопрос
	 * @return количество баллов
	 */
	int RateAnswer();
	
	shared_ptr<Question> GetQuestion();

private:
	Ui::QuestionWidget * ui;
	QTextBrowser * questionTextBrowser; ///< указатель на область записи вопроса
	shared_ptr<Question> question; ///< указатель на вопрос, который будет выведен на виджете
	vector<QCheckBox *> answersCheckBoxes; ///< массив указателей на CheckBox'ы, которые связаны с ответами
	
	/// формирование ответов: заполнение массива с вариантами ответа CheckBox'ами
	void formAnswers();
};

#endif // KRUSKAL_QUESTIONWIDGET_H
