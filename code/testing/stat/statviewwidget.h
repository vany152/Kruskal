#ifndef ASD_STATVIEWWIDGET_H
#define ASD_STATVIEWWIDGET_H

#include <QWidget>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class StatViewWidget; }
QT_END_NAMESPACE

class StatViewWidget : public QWidget
{
Q_OBJECT

public:
	/// конструктор класса
	explicit StatViewWidget(const QString & dbname, QWidget * parent = nullptr);
	/// деструктор класса
	~StatViewWidget() override;

private:
	Ui::StatViewWidget * ui;
	/*
	 * если размещать базу данных не в динамической памяти, ее деструктор срабатывает после удаления ui,
	 * из-за чего программа аварийно завершается с сигналом SIGSEGV
	 */
	QSqlDatabase * db; ///< база данных со статистикой
	QSqlTableModel * model; ///< табличная модель базы данных
	
	/**
	 * @brief установление видимости виджета ввода имени пользователя
	 * @param visible true - виджет отображается, false - виджет скрыт
	 */
	void setVisibleUsernameFields(bool visible);
	
	/**
	 * @brief установление видимости виджетов ввода интервала тестирования
	 * @param visible true - виджет отображается, false - виджет скрыт
	 */
	void setVisibleTestingPeriodFields(bool visible);
	
	/**
	 * @brief установление видимости виджетов ввода интервала продолжительности тестирования
	 * @param visible true - виджет отображается, false - виджет скрыт
	 */
	void setVisibleTestingDurationFields(bool visible);
	
	/**
	 * @brief установление видимости виджетов ввода интервала процентов выполнения
	 * @param visible true - виджет отображается, false - виджет скрыт
	 */
	void setVisiblePercentFields(bool visible);
	
private slots:
	/// применение фильтра к табличной модели БД
	void on_showButton_clicked();
	/// очистка фильтра табличной модели БД
	void on_clearFilterButton_clicked();
	/// отображение \ скрытие полей ввода фильтра в зависимости от текущего текста QComboBox
	void changeParametersFields(const QString & option);
};

#endif //ASD_STATVIEWWIDGET_H
