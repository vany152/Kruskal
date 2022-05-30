#include "welcometotesting.h"
#include "ui_welcometotesting.h"
#include "../common/constants.h"

WelcomeToTesting::WelcomeToTesting(QWidget * parent) :
		QWidget(parent), ui(new Ui::WelcomeToTesting)
{
	ui->setupUi(this);
#ifndef NDEBUG
	ui->usernameLineEdit->setText("test");
#else
	ui->startTestingButton->setEnabled(false);
#endif

	setFixedSize(500, 400);
}

WelcomeToTesting::~WelcomeToTesting()
{
	delete ui;
}

void WelcomeToTesting::closeEvent(QCloseEvent * e)
{
	emit closed(ui->usernameLineEdit->text());
	e->accept();
}

/// изменение имени пользователя
void WelcomeToTesting::on_usernameLineEdit_textChanged()
{
	if (!ui->usernameLineEdit->text().trimmed().isEmpty()) { ui->startTestingButton->setEnabled(true); }
	else { ui->startTestingButton->setEnabled(false); }
}

void WelcomeToTesting::on_startTestingButton_clicked()
{
	close();
}

void WelcomeToTesting::on_showTestingRulesButton_clicked()
{
	emit testingRulesButtonClicked();
}



