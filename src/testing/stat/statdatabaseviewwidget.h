#ifndef KRUSKAL_STATDATABASEVIEWWIDGET_H
#define KRUSKAL_STATDATABASEVIEWWIDGET_H

#include <QWidget>
#include <QtSql>

#include "StatDatabaseViewOption.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StatDatabaseViewWidget; }
QT_END_NAMESPACE

class StatDatabaseViewWidget : public QWidget
{
Q_OBJECT

public:
	/// конструктор класса
	explicit StatDatabaseViewWidget(const QString & dbname, QWidget * parent = nullptr);
	/// деструктор класса
	~StatDatabaseViewWidget() override;

private:
	Ui::StatDatabaseViewWidget * ui;
	/*
	 * если размещать базу данных не в динамической памяти, ее деструктор срабатывает после удаления ui,
	 * из-за чего программа аварийно завершается с сигналом SIGSEGV
	 */
	QSqlDatabase * db; ///< база данных со статистикой
	QSqlTableModel * model; ///< табличная модель базы данных
	
    std::list<std::unique_ptr<StatDatabaseViewOption>> filterOptions; ///< опции фильтрации записей в базе данных
    
    void addFilterOptions();
	
private slots:
	/// применение фильтра к табличной модели БД
	void on_showButton_clicked();
	/// очистка фильтра табличной модели БД
	void on_clearFilterButton_clicked();
	/// отображение \ скрытие полей ввода фильтра в зависимости от текущего фильтра
	void changeParametersFields(const QString & optionName);
};

#endif // KRUSKAL_STATDATABASEVIEWWIDGET_H
