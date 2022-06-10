#include "statdatabaseviewwidget.h"
#include "ui_statdatabaseviewwidget.h"

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

/// конструктор класса
StatDatabaseViewWidget::StatDatabaseViewWidget(const QString & dbname, QWidget * parent) :
		QWidget(parent), ui(new Ui::StatDatabaseViewWidget)
{
	ui->setupUi(this);
    setWindowTitle("Статистика");
    
    /*
     * добавляем опции фильтра записей базы данных
     */
    addFilterOptions();
    for (const auto & option : filterOptions) { ui->showOptionComboBox->addItem(option->GetName()); }
	
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
	
	connect(ui->showOptionComboBox, &QComboBox::currentTextChanged, this, &StatDatabaseViewWidget::changeParametersFields);
}

/// деструктор класса
StatDatabaseViewWidget::~StatDatabaseViewWidget()
{
	db->removeDatabase(db->connectionName());
    delete db;
	delete ui;
}

/** *************************************************** PRIVATE **************************************************** **/

void StatDatabaseViewWidget::addFilterOptions()
{
    /*
     * фильтр "имя пользователя"
     */
    std::vector<QWidget *> fields{ui->usernameLineEdit};
    filterOptions.emplace_back(std::make_unique<UsernameOption>(fields));
    
    /*
     * фильтр "период тестирования
     */
    fields = {ui->testingStartLabel, ui->testingStartDateTimeEdit, ui->testingFinishLabel, ui->testingFinishDateTimeEdit};
    filterOptions.emplace_back(std::make_unique<TestingPeriodOption>(fields));
    
    /*
     * фильтр "продолжительность тестирования"
     */
    fields = {ui->durationFromLabel, ui->durationFromTimeEdit, ui->durationToLabel, ui->durationToTimeEdit};
    filterOptions.emplace_back(std::make_unique<TestingDurationOption>(fields));
    
    /*
     * фильтр "процент выполнения"
     */
    fields = {ui->percentsFromLaleb, ui->percentsFromSpinBox, ui->percentsToLabel, ui->percentsToSpinBox};
    filterOptions.emplace_back(std::make_unique<CompletionPercentage>(fields));
}

/** **************************************************** SLOTS ***************************************************** **/

/// применение фильтра к табличной модели БД
void StatDatabaseViewWidget::on_showButton_clicked()
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
void StatDatabaseViewWidget::on_clearFilterButton_clicked()
{
	model->setFilter("");
	model->select();
}

/// отображение \ скрытие полей ввода фильтра в зависимости от текущего текста QComboBox
void StatDatabaseViewWidget::changeParametersFields(const QString & optionName)
{
    for (auto & option : filterOptions) { option->SetFieldsVisibility(option->GetName() == optionName); }
}
