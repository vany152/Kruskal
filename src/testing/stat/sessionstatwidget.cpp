#include "sessionstatwidget.h"
#include "ui_sessionstatwidget.h"

SessionStatWidget::SessionStatWidget(const std::shared_ptr<SessionStat> & stat, QWidget * parent) :
		QWidget(parent), ui(new Ui::SessionStatWidget)
{
	ui->setupUi(this);
	
	try { ui->statTextBrowser->setHtml(stat->ToHtmlString()); }
	catch (SessionStat::StatError & err) { err.Show(); throw err; }
}

SessionStatWidget::~SessionStatWidget()
{
	delete ui;
}

void SessionStatWidget::closeEvent(QCloseEvent * e)
{
	emit closed();
	e->accept();
}


