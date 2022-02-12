#ifndef ASD_QUESTION_H
#define ASD_QUESTION_H

#include <vector>
#include <set>

#include <QString>

#include "../../common/documents.h"

using std::vector;
using std::set;

/// класс закрытого вопроса
class Question final
{
public:
	/// описывает сложность вопроса, так же является количеством баллов за правильный ответ
	enum Complexity { simple = 1, medium, hard };
	
	Question() : question(""), complexity(simple), answers() {}
	
	/**
	 * @brief конструктор с параметрами
	 * @param topic тема вопроса
	 * @param question тело вопроса
	 * @param answers варианты ответов
	 * @param correctAnswers номера правильный ответов
	 * @param complexity сложность вопроса
	 */
	explicit Question(
			const QString & question,
			const std::vector<QString> & answers,
			const std::set<int> & correctAnswers,
			Complexity complexity
	) : question(question),
	    answers(answers),
	    correctAnswers(correctAnswers),
	    complexity(complexity) {};
	
	Question(const QJsonObject & questionJson);
	
	/**
	 * @brief оценка ответа на вопрос
	 * @param userAnswers пользовательский ответ
	 * @return количество баллов
	 */
	int RateAnswer(const std::set<int> & userAnswers) const noexcept;
	
	QString GetQuestion() const noexcept { return question; }
	vector<QString> GetAnswers() const noexcept { return answers; }
	Complexity GetComplexity() const noexcept { return complexity; }

private:
	QString question; ///< тело вопроса
	Complexity complexity; ///< сложность вопроса
	vector<QString> answers; ///< варианты ответов на вопросы
	/*
	 * set чтобы не сортировать содержимое и не удалять повторяющиеся элементы
	 */
	set<int> correctAnswers; ///< номера правильных ответов на вопрос
};

#endif //ASD_QUESTION_H
