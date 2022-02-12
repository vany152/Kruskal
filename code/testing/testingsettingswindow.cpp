#include <QFileDialog>

#include "testingsettingswindow.h"
#include "ui_TestingSettingsWindow.h"

#include "../common/constants.h"
#include "../common/documents.h"
#include "../common/Error.h"

/**
 * todo в меню "справка" добавить "редактирование файла" с информацией о правилах редактирования файла с вопросами
 * todo в меню "справка" добавить "просмотреть статистику"
 */

/** **************************************************** PUBLIC **************************************************** **/

/// конструктор класса
TestingSettingsWindow::TestingSettingsWindow(QWidget * parent) :
		QWidget(parent), ui(new Ui::TestingSettingsWindow)
{
	ui->setupUi(this);
	setFixedSize(700, 140);
	
	QJsonObject config;
	try { config = readJson(configPath); }
	catch (FileError & err) { err.Show(); close(); return; }
	
	QJsonObject testingConfig = config.find("testing")->toObject();
	ui->questionsSourceLabel->setText(testingConfig.find("questionsSource")->toString());
	ui->amountOfQuestionsSpinBox->setValue(testingConfig.find("amountOfQuestions")->toInt());
	
	setWindowTitle("Настройки");
}

/// деструктор класса
TestingSettingsWindow::~TestingSettingsWindow()
{
	delete ui;
}

/** ***************************************************** SLOTS **************************************************** **/

/**
 * @brief выбор нового пути к главному файлу теории
 * @details используется QFileDialog
 */
void TestingSettingsWindow::on_changeQuestionsSourceButton_clicked()
{
    QString newSource = QFileDialog::getOpenFileName(
                this,
                "Choose theory file",
                "/src",
                "JSON файлы (*.json);;Все файлы (*.*)"
    );
	
	QJsonObject config;
	try { config = readJson(configPath); }
	catch (FileError & err) { err.Show(); close(); return; }
	ui->questionsSourceLabel->setText(newSource);
}

/// сохранение конфигурации в файл
void TestingSettingsWindow::on_saveButton_clicked()
{
	QJsonObject config;
	try { config = readJson(configPath); }
	catch (FileError & err) { err.Show(); close(); return; }
	
	QJsonValueRef testingConfig = config["testing"];
	QJsonObject newTestingConfig = testingConfig.toObject();
	
	newTestingConfig["amountOfQuestions"] = ui->amountOfQuestionsSpinBox->value();
	newTestingConfig["questionsSource"] = ui->questionsSourceLabel->text();
	
	testingConfig = newTestingConfig;
	writeJson(config, configPath);
}



