#include <QFileDialog>

#include "theorysettingswindow.h"
#include "ui_TheorySettingsWindow.h"

#include "../common/constants.h"
#include "../common/documents.h"

/// конструктор класса
TheorySettingsWindow::TheorySettingsWindow(QWidget * parent) :
		QWidget(parent), ui(new Ui::TheorySettingsWindow)
{
	ui->setupUi(this);
	setFixedSize(700, 140);
	
	QJsonObject config = readJson(configPath);
	QJsonObject theoryConfig = config.find("theory")->toObject();
	ui->theorySourceLabel->setText(theoryConfig.find("mainFileSource")->toString());
	
	setWindowTitle("Настройки");
}

/// деструктор класса
TheorySettingsWindow::~TheorySettingsWindow()
{
	delete ui;
}

/** ***************************************************** SLOTS **************************************************** **/

/**
 * @brief выбор нового пути к главному файлу теории
 * @details используется QFileDialog
 */
void TheorySettingsWindow::on_changeTheorySourceButton_clicked()
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

/// сохранение конфигурации в файл
void TheorySettingsWindow::on_saveButton_clicked()
{
	QJsonObject config = readJson(configPath);
	QJsonValueRef theoryConfig = config["theory"];
	QJsonObject newTheoryConfig = theoryConfig.toObject();
	
	newTheoryConfig["mainFileSource"] = ui->theorySourceLabel->text();
	
	theoryConfig = newTheoryConfig;
	writeJson(config, configPath);
}

