#include <QFileInfo>
#include <QWebEngineHistory>

#include "theorysystem.h"
#include "ui_theorysystem.h"
#include "theorysettingswindow.h"
#include "../common/documents.h"
#include "../common/constants.h"
#include "../common/Error.h"

/** **************************************************** PUBLIC **************************************************** **/

/// конструктор класса
TheorySystem::TheorySystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TheorySystem)
{
    ui->setupUi(this);
	setWindowTitle("Теория");

    displaySource();

    ui->prevPageButton->setEnabled(false);
    ui->nextPageButton->setEnabled(false);

    connect(ui->webView, &QWebEngineView::urlChanged, this, &TheorySystem::urlChanged);
}

/// деструктор класса
TheorySystem::~TheorySystem()
{
    delete ui;
}

/** **************************************************** PRIVATE *************************************************** **/

/// вывод на экран главного файла с теорией
void TheorySystem::displaySource()
{
    QJsonObject config;
    try { config = readJson(configPath); }
    catch (FileError & err) { err.Show(); close(); return; }

    QJsonObject theoryConfig = config.find("theory")->toObject();
    QFileInfo mainFileInfo = QFileInfo(theoryConfig.find("mainFileSource")->toString());

    ui->webView->load(QUrl(mainFileInfo.absoluteFilePath()));
}

/** ***************************************************** SLOTS **************************************************** **/

/// переход к предыдущей странице истории
void TheorySystem::on_prevPageButton_clicked()
{
    ui->webView->back();
}

/// переход к следующей странице истории
void TheorySystem::on_nextPageButton_clicked()
{
    ui->webView->forward();
}

/**
 * @brief изменена URL
 * @details включение и отключение кнопок перемщения по истории
 */
void TheorySystem::urlChanged()
{
    /*
     * если мы НЕ можем идти вперед, отключаем кнопку перехода вперед
     * иначе - включаем нопку перехода на следующую страницу
     */
    if (!ui->webView->history()->canGoForward()) { ui->nextPageButton->setEnabled(false); }
    else { if (!ui->nextPageButton->isEnabled()) { ui->nextPageButton->setEnabled(true); } }

    /*
     * если мы НЕ можем идти назад, отключаем кнопку перехода назад
     * иначе - включаем нопку перехода на предыдущую страницу
     */
    if (!ui->webView->history()->canGoBack()) { ui->prevPageButton->setEnabled(false); }
    else { if (!ui->prevPageButton->isEnabled()) { ui->prevPageButton->setEnabled(true); } }
}

/// открыто окно настроек
void TheorySystem::on_settingsMenuBar_triggered()
{
    TheorySettingsWindow * theorySettingsWindow = new TheorySettingsWindow();
    theorySettingsWindow->show();
}



