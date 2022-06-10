#include <cstdint>

#include <QtSql>

#include "sessionstat.h"

/** **************************************************** PUBLIC **************************************************** **/

/**
 * @brief сохранение количества полученных баллов за вопрос
 * @param question вопрос
 * @param rate полученный балл
 */
void SessionStat::AddQuestionRate(const std::shared_ptr<Question> & question, int rate) noexcept
{
	questionsRate.emplace_back(question, rate);
}

/**
 * @brief сохранение статистики в таблицу "statistics" базы данных SQLite 3
 * @details база данных создается, если ее не существует.\n
 * таблица @statistics@ создается, если ее не существует.\n
 * формат таблицы statistics:\n
 * 1. Пользователь VARCHAR(255) NOT NULL\n
 * 2. Начало DATETIME NOT NULL\n
 * 3. Окончание DATETIME NOT NULL\n
 * 4. Продолжительность TIME NOT NULL\n
 * 5. Набранный балл INTEGER NOT NULL\n
 * 6. Максимальный балл INTEGER NOT NULL
 * 7. Процент выполнения INTEGER
 * @param dbname имя файла базы данных
 */
void SessionStat::Save(const QString & dbname)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "dbsave");
	db.setDatabaseName(dbname);
	db.open();
	
	QSqlQuery query(db);
	query.exec("CREATE TABLE IF NOT EXISTS statistics ("
	           "'Пользователь' VARCHAR(255) NOT NULL,"
	           "'Начало' DATETIME NOT NULL,"
	           "'Окончание' DATETIME NOT NULL,"
	           "'Продолжительность' TIME,"
	           "'Набранный балл' INTEGER,"
	           "'Максимальный балл' INTEGER,"
			   "'Процент выполнения' INTEGER"
	           ");"
	);
	
	query.prepare("INSERT INTO statistics ("
	              "'Пользователь', "
	              "'Начало', "
	              "'Окончание', "
	              "'Продолжительность', "
	              "'Набранный балл', "
	              "'Максимальный балл',"
				  "'Процент выполнения'"
	              ") VALUES ("
	              ":username, "
	              ":testStart, "
	              ":testFinish, "
	              ":testDuration, "
	              ":userRate, "
	              ":maxRate, "
				  ":percentage);"
	);
	
    int32_t userRate = calculateUserRate();
    int32_t maxRate = calculateMaxRate();
	query.bindValue(":username", username);
	query.bindValue(":testStart", startTime.toString("yyyy-MM-dd  HH:mm:ss"));
	query.bindValue(":testFinish", finishTime.toString("yyyy-MM-dd  HH:mm:ss"));
	query.bindValue(":testDuration", duration(startTime, finishTime).toString("HH:mm:ss"));
	query.bindValue(":userRate", userRate);
	query.bindValue(":maxRate", maxRate);
	query.bindValue(":percentage", userRate * 100 / maxRate);
	query.exec();
	
	QSqlDatabase::removeDatabase(db.connectionName());
}

/// вывод данных класса в строку
QString SessionStat::ToHtmlString() const
{
	if (!startTime.isValid()) { throw StatError("невалидное время начала тестирования"); }
	if (!finishTime.isValid()) { throw StatError("невалидное время окончания тестирования"); }
	if (startTime > finishTime) { throw StatError("тестирование не может начинаться позже, чем заканчиваться"); }
	
    QString datetimeFormat = "hh:mm:ss";
	QString result(
			"Имя пользователя: <b>" + username + "</b><br>" +
			"Начало тестирования: <b>" + startTime.toString(datetimeFormat) + "</b><br>" +
			"Окончание тестирования: <b>" + finishTime.toString(datetimeFormat) + "</b><br>" +
			"Продолжительность тестирования: <b>" + duration(startTime, finishTime).toString(datetimeFormat) + "</b><br><br>"
	);
	
	/*
	 * хранит количество полученных и максимальный балл по каждому вопросу
	 */
	QString tmp;
	
	for (int i = 0; i < questionsRate.size(); ++i)
	{
		int max = questionsRate[i].first->GetComplexity(); // максимальный балл за вопрос
		int obtained = questionsRate[i].second; // полученный балл за вопрос
		
		tmp += "Вопрос " + QString::number(i + 1) + ": <b>" +
		       QString::number(obtained) + " / " + QString::number(max) + "</b><br>";
	}
	
	result += "Набрано баллов: " + QString::number(calculateUserRate()) +
	          " из " + QString::number(calculateMaxRate()) + ":<br>" +
	          tmp;
	
	return result;
}

/// установка времени начала теста
void SessionStat::SetStartTime(const QDateTime & _startTime)
{
	if (!_startTime.isValid()) { throw StatError("невалидное время начала тестирования"); }
    SessionStat::startTime = _startTime;
}

/// установка времени окончания теста
void SessionStat::SetFinishTime(const QDateTime & _finishTime)
{
	if (!_finishTime.isValid()) { throw StatError("невалидное время окончания тестирования"); }
    SessionStat::finishTime = _finishTime;
}

/** *************************************************** PRIVATE **************************************************** **/

/// вычисление продолжительности между двумя временными точками
QTime SessionStat::duration(const QDateTime & from, const QDateTime & to) noexcept
{
	uint64_t secs = from.secsTo(to);
    int h = secs / 3600;
    int m = secs / 60;
    int s = secs % 60;
	QTime res(h, m, s);
	
	return res;
}

/**
 * @brief подсчет набранного пользователем балла
 * @return набранный балл
 */
int SessionStat::calculateUserRate() const noexcept
{
    int userRate = 0;
    for (const auto & question : questionsRate) { userRate += question.second; }
    
    return userRate;
}

/**
 * @brief подсчет максимального балла за тест
 * @return максимальный балл
 */
int SessionStat::calculateMaxRate() const noexcept
{
    int maxRate = 0;
    // сложность вопроса = количество баллов за верный ответ
    for (const auto & question : questionsRate) { maxRate += question.first->GetComplexity(); }
    
    return maxRate;
}
