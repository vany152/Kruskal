#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testing/testingsystem.h"
#include "demo/demosystem.h"

#ifndef NOWEBENGINE
  #include "common/htmlviewer.h"
#else
  #include <QMessageBox>
#endif

/** **************************************************** PUBLIC **************************************************** **/

MainWindow::MainWindow(QWidget * parent) :
		QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle("Алгоритм Краскала");
 
	displayHelloText();
}

MainWindow::~MainWindow()
{
	delete ui;
}

/** *************************************************** PRIVATE **************************************************** **/

/// вывод приветствия в текстовое поле
void MainWindow::displayHelloText()
{
    /*
    * читаем конфиги
    */
    QJsonObject config;
    try { config = readJson(configPath); }
    catch (FileError & err) { err.Show(); close(); return; }
    
    QString titleText;  // содержимое файла с текстом титульного листа
    try { titleText = readFile(config.find("titleText")->toString()); }
    catch (FileError & err) { err.Show(); close(); return; }
    
    /*
     * выводим прочитанный текст на виджет
     */
    ui->welcomeTextBrowser->setHtml(titleText);
}

/** ***************************************************** SLOTS **************************************************** **/

/// запуск окна с теорией
void MainWindow::on_theoryButton_clicked()
{
#ifndef NOWEBENGINE
    HtmlViewer * theoryViewer = new HtmlViewer(this);
    theoryViewer->SetWindowTitle("Теория");
    theoryViewer->show();

    /*
     * в конфигах ищем путь файла с теорией
     */
    QJsonObject config;
    try { config = readJson(configPath); }
    catch (FileError & err) { err.Show(); close(); return; }
    QJsonObject theoryConfig = config.find("theory")->toObject();
    QFileInfo mainFileInfo = QFileInfo(theoryConfig.find("mainFileSource")->toString());

    /*
     * загружаем файл с теорией
     */
    theoryViewer->Open(mainFileInfo.absoluteFilePath());
#else
    QMessageBox * messageBox = new QMessageBox(this);
    messageBox->setText("Компонент не скомпилирован");
    messageBox->exec();
#endif
}

/// запуск окна с демонстрацией
void MainWindow::on_demoButton_clicked()
{
	DemoSystem * demoSystem = new DemoSystem(this);
	demoSystem->show();
}

/// запуск окна с тестированием
void MainWindow::on_testingButton_clicked()
{
	TestingSystem * testingSystemWindow = new TestingSystem(this);
	testingSystemWindow->show();
}


