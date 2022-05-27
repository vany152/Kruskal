#include "statviewwidget.h"
#include "ui_statviewwidget.h"

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

/// конструктор класса
StatViewWidget::StatViewWidget(const QString & dbname, QWidget * parent) :
		QWidget(parent), ui(new Ui::StatViewWidget)
{
	ui->setupUi(this);
	
	// отображаем строку ввода имени пользователя и скрываем все остальные виджеты параметров
	changeParametersFields(ui->showOptionComboBox->currentText());
	
	/*
	 * устанавливаем на виджеты отображения даты и времени текущий день
	 */
	ui->testingStartDateTimeEdit->setDate(QDateTime::currentDateTime().date());
    ui->testingFinishDateTimeEdit->setDate(QDateTime::currentDateTime().date());
	
	/*
	 * связываемся с базой данных
	 */
	db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "dbview"));
	db->setDatabaseName(dbname);
	db->open();
	
	/*
	 * выводим базу данных на экран
	 */
	model = new QSqlTableModel(this, *db);
	model->setTable("statistics");
	model->select();
	ui->tableView->setModel(model);
	ui->tableView->resizeColumnsToContents();
	
	connect(ui->showOptionComboBox, &QComboBox::currentTextChanged, this, &StatViewWidget::changeParametersFields);
}

/// деструктор класса
StatViewWidget::~StatViewWidget()
{
	db->removeDatabase(db->connectionName());
	delete db;
	delete ui;
}

/** *************************************************** PRIVATE **************************************************** **/

/**
 * @brief установление видимости виджета ввода имени пользователя
 * @param visible true - виджет отображается, false - виджет скрыт
 */
void StatViewWidget::setVisibleUsernameFields(bool visible)
{
	ui->usernameLineEdit->setVisible(visible);
}

/**
 * @brief установление видимости виджетов ввода интервала тестирования
 * @param visible true - виджет отображается, false - виджет скрыт
 */
void StatViewWidget::setVisibleTestingPeriodFields(bool visible)
{
	ui->testingStartLabel->setVisible(visible);
	ui->testingStartDateTimeEdit->setVisible(visible);
	ui->testingFinishLabel->setVisible(visible);
	ui->testingFinishDateTimeEdit->setVisible(visible);
}

/**
 * @brief установление видимости виджетов ввода интервала продолжительности тестирования
 * @param visible true - виджет отображается, false - виджет скрыт
 */
void StatViewWidget::setVisibleTestingDurationFields(bool visible)
{
	ui->durationFromLabel->setVisible(visible);
	ui->durationFromTimeEdit->setVisible(visible);
	ui->durationToLabel->setVisible(visible);
	ui->durationToTimeEdit->setVisible(visible);
}

/**
 * @brief установление видимости виджетов ввода интервала процентов выполнения
 * @param visible true - виджет отображается, false - виджет скрыт
 */
void StatViewWidget::setVisiblePercentFields(bool visible)
{
	ui->percentsFromLaleb->setVisible(visible);
	ui->percentsFromSpinBox->setVisible(visible);
	ui->percentsToLabel->setVisible(visible);
	ui->percentsToSpinBox->setVisible(visible);
}

/** **************************************************** SLOTS ***************************************************** **/

/// применение фильтра к табличной модели БД
void StatViewWidget::on_showButton_clicked()
{
	QString option = ui->showOptionComboBox->currentText();
	if (option == "Имя пользователя")
	{
		// SELECT * FROM statistics WHERE Пользователь LIKE ' {username}% '
		model->setFilter(QString("Пользователь LIKE '%1%'").arg(ui->usernameLineEdit->text()));
	}
	else if (option == "Период прохождения тестирования")
	{
		// SELECT * FROM statistics WHERE ' {start} ' <= Начало AND Конец <= ' {finish} ';
		QDateTime start = ui->testingStartDateTimeEdit->dateTime();
		QDateTime finish = ui->testingFinishDateTimeEdit->dateTime();
		model->setFilter(
				QString("'%1' <= Начало AND Окончание <= '%2'").arg(
						start.toString("yyyy-MM-dd  HH:mm:ss"),
						finish.toString("yyyy-MM-dd  HH:mm:ss")
				)
		);
	}
	else if (option == "Продолжительность тестирования")
	{
		// SELECT * FROM statistics WHERE ' {from} ' <= Продолжительность AND Продолжительность <= ' {to} ';
		QTime from = ui->durationFromTimeEdit->time();
		QTime to = ui->durationToTimeEdit->time();
		model->setFilter(
				QString("Продолжительность BETWEEN '%1' AND '%2'").arg(
						from.toString("HH:mm:ss"),
						to.toString("HH:mm:ss")
				)
		);
	}
	else if (option == "Процент выполнения")
	{
		// SELECT * FROM statistics WHERE ' {from} ' <= 'Процент выполнения' AND 'Процент выполнения' <= ' {to} ';
		int from = ui->percentsFromSpinBox->value();
		int to = ui->percentsToSpinBox->value();
		model->setFilter(
                QString("\"Процент выполнения\" BETWEEN %1 AND %2").arg(
						QString::number(from),
						QString::number(to)
				)
		);
	}
	model->select();
}

/// очистка фильтра табличной модели БД
void StatViewWidget::on_clearFilterButton_clicked()
{
	model->setFilter("");
	model->select();
}

/// отображение \ скрытие полей ввода фильтра в зависимости от текущего текста QComboBox
void StatViewWidget::changeParametersFields(const QString & option)
{
	if (option == "Имя пользователя")
	{
		setVisibleUsernameFields(true);
		setVisibleTestingPeriodFields(false);
		setVisibleTestingDurationFields(false);
		setVisiblePercentFields(false);
	}
	else if (option == "Период прохождения тестирования")
	{
		setVisibleTestingPeriodFields(true);
		setVisibleUsernameFields(false);
		setVisibleTestingDurationFields(false);
		setVisiblePercentFields(false);
	}
	else if (option == "Продолжительность тестирования")
	{
		setVisibleTestingDurationFields(true);
		setVisibleUsernameFields(false);
		setVisibleTestingPeriodFields(false);
		setVisiblePercentFields(false);
	}
	else if (option == "Процент выполнения")
	{
		setVisiblePercentFields(true);
		setVisibleUsernameFields(false);
		setVisibleTestingPeriodFields(false);
		setVisibleTestingDurationFields(false);
	}
}





