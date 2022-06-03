#ifndef KRUSKAL_SESSIONSTAT_H
#define KRUSKAL_SESSIONSTAT_H

#include <vector>
#include <string>
#include <unordered_map>

#include <QDateTime>
#include <QSqlDatabase>

#include "../questions/question.h"
#include "../../common/Error.h"

class SessionStat
{
public:
    class StatError final : public Error { public: StatError(const std::string & what) : Error(what) {} };
	
	/**
	 * @brief конструктор класса
	 * @param username имя пользователя, к которому относится статистика
	 */
	SessionStat(const QString & username) : username(username){}
	
	/**
	 * @brief сохранение количества полученных баллов за вопрос
	 * @param question вопрос
	 * @param rate полученный балл
	 */
	void AddQuestionRate(const std::shared_ptr<Question> & question, int rate) noexcept;

	/**
	 * @brief сохранение статистики в таблицу "statistics" базы данных SQLite 3
	 * @details база данных создается, если ее не существует.\n
	 * таблица @statistics@ создается, если ее не существует.\n
	 * формат таблицы statistics:\n
	 * 1. username VARCHAR(255) NOT NULL\n
	 * 2. testStart DATETIME NOT NULL\n
	 * 3. testFinish DATETIME NOT NULL\n
	 * 4. testDuration TIME NOT NULL\n
	 * 5. userRate INTEGER NOT NULL\n
	 * 6. maxRate INTEGER NOT NULL
	 * @param dbname имя файла базы данных
	 */
	void Save(const QString & dbname);
	
	/// вывод данных класса в строку
	QString ToHtmlString() const;
	
	/// установка времени начала теста
	void SetStartTime(const QDateTime & _startTime);
	/// установка времени окончания теста
	void SetFinishTime(const QDateTime & _finishTime);
	
private:
	QString username; ///< имя пользователя
	QDateTime startTime; ///< время начало теста
	QDateTime finishTime; ///< время окончания теста
	
	/*
	 * из-за сортировки map'ом элементов при применении метода ToHtmlString() вопросы и соответствующие
	 * им баллы будут располагаться в неверном порядке.
	 */
	std::vector<std::pair<std::shared_ptr<Question>, int>> questionsRate; ///< вопросы и полученные за них баллы
	
	/// вычисление продолжительности между двумя временными точками
	static QTime duration(const QDateTime & from, const QDateTime & to) noexcept;
    
    /**
     * @brief подсчет набранного пользователем балла
     * @return набранный балл
     */
    int calculateUserRate() const noexcept;
    
    /**
     * @brief подсчет максимального балла за тест
     * @return максимальный балл
     */
    int calculateMaxRate() const noexcept;
};

#endif // KRUSKAL_SESSIONSTAT_H
