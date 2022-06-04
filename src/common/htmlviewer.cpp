#include <QFileInfo>
#include <QWebEngineHistory>

#include "htmlviewer.h"
#include "ui_htmlviewer.h"
#include "settingswindow.h"
#include "documents.h"
#include "constants.h"
#include "Error.h"

/** **************************************************** PUBLIC **************************************************** **/

/// конструктор класса
HtmlViewer::HtmlViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HtmlViewer)
{
    ui->setupUi(this);

    ui->prevPageButton->setEnabled(false);
    ui->nextPageButton->setEnabled(false);

    connect(ui->webView, &QWebEngineView::urlChanged, this, &HtmlViewer::urlChanged);
}

/// деструктор класса
HtmlViewer::~HtmlViewer()
{
    delete ui;
}

void HtmlViewer::Open(const QString & source)
{
    ui->webView->load(source);
}

/** ***************************************************** SLOTS **************************************************** **/

/// переход к предыдущей странице истории
void HtmlViewer::on_prevPageButton_clicked()
{
    ui->webView->back();
}

/// переход к следующей странице истории
void HtmlViewer::on_nextPageButton_clicked()
{
    ui->webView->forward();
}

/**
 * @brief изменена URL
 * @details включение и отключение кнопок перемщения по истории
 */
void HtmlViewer::urlChanged()
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
void HtmlViewer::on_settingsMenuBar_triggered()
{
    SettingsWindow::Instance().show();
}



