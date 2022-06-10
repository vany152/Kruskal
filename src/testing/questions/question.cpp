#include "question.h"

Question::Question(const QJsonObject & questionJson)
{
	question = questionJson.find("question")->toString();
	complexity = static_cast<Complexity>(questionJson.find("complexity")->toInt());
	
	QJsonArray answersArray = questionJson.find("answers")->toArray();
	for (auto answer : answersArray) { answers.push_back(answer.toString()); }
	
	QJsonArray correctAnswersArray = questionJson.find("correctAnswers")->toArray();
	for (auto correctAnswer : correctAnswersArray) { correctAnswers.insert(correctAnswer.toInt()); }
}

/**
 * @brief оценка ответа на вопрос
 * @param answer пользовательский ответ
 * @return количество баллов
 */
int Question::RateAnswer(const std::set<int> & userAnswers) const noexcept
{
	return (userAnswers == correctAnswers) ? complexity : 0;
}
