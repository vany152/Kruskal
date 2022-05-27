#include "statwidget.h"
#include "ui_statwidget.h"

StatWidget::StatWidget(const shared_ptr<Stat> & stat, QWidget * parent) :
		stat(stat), QWidget(parent), ui(new Ui::StatWidget)
{
	ui->setupUi(this);
	
	try { ui->statTextBrowser->setText(stat->ToString()); }
	catch (Stat::StatError & err) { err.Show(); throw err; }
}

StatWidget::~StatWidget()
{
	delete ui;
}

void StatWidget::closeEvent(QCloseEvent * e)
{
	emit closed();
	e->accept();
}


