#include <QFileDialog>

#include "settingswindow.h"
#include "ui_settingswindow.h"

#include "constants.h"
#include "documents.h"
#include "Error.h"

/** **************************************************** PUBLIC **************************************************** **/

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

/// возвращение состояния объекта
SettingsWindow & SettingsWindow::Instance()
{
    static SettingsWindow self(nullptr);
    return self;
}

/** **************************************************** PRIVATE *************************************************** **/

// приватный конструктор для синглтона
SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    
    /*
     * устанавливаем свойства окна
     */
    setFixedSize(700, 200);
    setWindowTitle("Настройки");
    
    /*
     * загружаем значения из конфигов
     */
    update();
}

/// обновление полей значениями из файла конфигурации
void SettingsWindow::update()
{
    /*
     * читаем конфиги
     */
    QJsonObject config;
    try { config = readJson(configPath); }
    catch (FileError & err) { err.Show(); close(); return; }
    
    /*
     * устанавливаем значения из конфигов на элементы вкладки "настройки теории"
     */
    QJsonObject theoryConfig = config.find("theory")->toObject();
    ui->theorySourceLabel->setText(theoryConfig.find("mainFileSource")->toString());
    
    /*
     * устанавливаем значения из конфигов на элементы вкладки "настройки тестирования"
     */
    QJsonObject testingConfig = config.find("testing")->toObject();
    ui->questionsSourceLabel->setText(testingConfig.find("questionsSource")->toString());
    ui->amountOfQuestionsSpinBox->setValue(testingConfig.find("amountOfQuestions")->toInt());
}

/** ************************************************* PRIVATE SLOTS ************************************************ **/

/** *************************** НАСТРОЙКИ ТЕОРИИ **************************** **/

/**
 * @brief выбор нового пути к главному файлу теории
 * @details используется QFileDialog
 */
void SettingsWindow::on_changeTheorySourceButton_clicked()
{
    QString newSource = QFileDialog::getOpenFileName(
            this,
            "Choose theory file",
            "/src",
            "HTML файлы (*.html);;Все файлы (*.*)"
    );
    
    QJsonObject config = readJson(configPath);
    ui->theorySourceLabel->setText(newSource);
}

/// сохранение конфигурации теории в файл
void SettingsWindow::on_saveTheorySettingsButton_clicked()
{
    QJsonObject config = readJson(configPath);
    QJsonValueRef theoryConfig = config["theory"];
    QJsonObject newTheoryConfig = theoryConfig.toObject();
    
    newTheoryConfig["mainFileSource"] = ui->theorySourceLabel->text();
    
    theoryConfig = newTheoryConfig;
    writeJson(config, configPath);
    
    update();
}

/** ************************ НАСТРОЙКИ ТЕСТИРОВАНИЯ ************************* **/

/**
 * @brief выбор нового пути к главному файлу теории
 * @details используется QFileDialog
 */
void SettingsWindow::on_changeQuestionsSourceButton_clicked()
{
    QString newSource = QFileDialog::getOpenFileName(
            this,
            "Choose theory file",
            "/resx",
            "JSON файлы (*.json);;Все файлы (*.*)"
    );
    
    QJsonObject config;
    try { config = readJson(configPath); }
    catch (FileError & err) { err.Show(); close(); return; }
    ui->questionsSourceLabel->setText(newSource);
}

/// сохранение конфигурации тестирования в файл
void SettingsWindow::on_saveTestingsWettingsButton_clicked()
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
    
    update();
}