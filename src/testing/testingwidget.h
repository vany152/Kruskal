#ifndef KRUSKAL_TESTINGWIDGET_H
#define KRUSKAL_TESTINGWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <vector>

#include "questions/questionwidget.h"
#include "stat/sessionstat.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TestingWidget; }
QT_END_NAMESPACE

class TestingWidget : public QWidget
{
Q_OBJECT

public:
	explicit TestingWidget(const std::shared_ptr<SessionStat> & stat, QWidget * parent = nullptr);
	
	~TestingWidget() override;

private:
	Ui::TestingWidget * ui;
    size_t currentQuestion; ///< индекс текущего вопроса текущий вопрос
    size_t amountOfQuestions; ///< количество вопросов
	std::vector<std::shared_ptr<QuestionWidget>> questions; ///< вопросы
	std::shared_ptr<SessionStat> currentSessionStat; ///< статистика по текущему тестированию
	
	/**
	 * @brief добавление вопросов из файла в список вопросов
	 * @details вопросы добавляются в случайном порядке
	 */
	void readQuestions();
	/// вывод на экран текущего вопроса
	void displayCurrentQuestion();
	void closeEvent(QCloseEvent * e) override;

signals:
	void closed();

private slots:
	void on_answerButton_clicked();
};

#endif // KRUSKAL_TESTINGWIDGET_H
