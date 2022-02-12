#ifndef ASD_TESTING_H
#define ASD_TESTING_H

#include <QWidget>
#include <QCloseEvent>
#include <vector>

#include "questions/questionwidget.h"
#include "stat/Stat.h"

using std::vector;
using std::shared_ptr;

QT_BEGIN_NAMESPACE
namespace Ui { class Testing; }
QT_END_NAMESPACE

class Testing : public QWidget
{
Q_OBJECT

public:
	explicit Testing(const std::shared_ptr<Stat> & stat, QWidget * parent = nullptr);
	
	~Testing() override;

private:
	Ui::Testing * ui;
    size_t currentQuestion; ///< индекс текущего вопроса текущий вопрос
    size_t amountOfQuestions; ///< количество вопросов
	vector<shared_ptr<QuestionWidget>> questions; ///< вопросы
	shared_ptr<Stat> stat; ///< статистика по текущему тестированию
	
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

#endif //ASD_TESTING_H
