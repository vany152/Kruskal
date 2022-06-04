#ifndef KRUSKAL_TESTINGSYSTEM_H
#define KRUSKAL_TESTINGSYSTEM_H

#include <QMainWindow>
#include <QDialog>

#include "questions/question.h"
#include "welcometotesting.h"
#include "testingwidget.h"
#include "stat/sessionstatwidget.h"

#ifndef NOWEBENGINE
  #include "../common/htmlviewer.h"
#endif

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
	TestingWidget * testingWidget; ///< виджет тестирования
	SessionStatWidget * statWidget; ///< виджет со статистикой
	std::shared_ptr<SessionStat> currentSessionStat; ///< статистика по текущему тестированию
#ifndef NOWEBENGINE
    HtmlViewer * viewer; ///< просмотрщие HTML файлов
#endif
    
    void showRef(const QString & refSectionName);
	
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

#endif // KRUSKAL_TESTINGSYSTEM_H
