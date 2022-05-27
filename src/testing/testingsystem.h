#ifndef ASD_TESTINGSYSTEM_H
#define ASD_TESTINGSYSTEM_H

#include <QMainWindow>
#include <QDialog>

#include "questions/question.h"
#include "welcometotesting.h"
#include "testing.h"
#include "stat/statwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TestingSystem; }
QT_END_NAMESPACE

class TestingSystem : public QMainWindow
{
Q_OBJECT

public:
	explicit TestingSystem(QWidget * parent = nullptr);
	~TestingSystem() override;

private:
	Ui::TestingSystem * ui;
	WelcomeToTesting * welcomeToTestingWidget; ///< приветственный виджет
	Testing * testingWidget; ///< виджет тестирования
	StatWidget * statWidget; ///< виджет со статистикой
	QDialog * testingRulesDialog; ///< диалоговое окно с правилами тестирования
    QDialog * editQuestionRulesDialog; ///< диалоговое окно с правилами редактирования вопросов в файле
	std::shared_ptr<Stat> stat; ///< статистика по текущему тестированию
	
	/// формирование окна с информацией о правилах тестирования
	void formTestingRulesDialog();
    /// формирование окна с информацией о правилах редактирования вопрсов в файле
    void formEditQuestionsRulesDialog();
	
private slots:
	/// выбран пункт меню "настройки"
	void on_openSettingsMenu_triggered();
	/// вывод виджета с базой данных статистики
	void on_statMenu_triggered();
	/// запуск теста
	void startTest(const QString & username);
	/// сохранение статистики в базу данных
	void saveStat();
	/// завершение теста и вывод статистики
	void showStat();
	/// вывод диалогового окна с правилами тестирования
	void showTestingRules();
    /// вывод диалогового окна с информацией о редактировании вопросов
    void showEditQuestionsRules();
};

#endif //ASD_TESTINGSYSTEM_H
