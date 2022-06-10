#ifndef KRUSKAL_SESSIONSTATWIDGET_H
#define KRUSKAL_SESSIONSTATWIDGET_H

#include <QWidget>
#include <QCloseEvent>

#include "sessionstat.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SessionStatWidget; }
QT_END_NAMESPACE

class SessionStatWidget : public QWidget
{
Q_OBJECT

public:
	explicit SessionStatWidget(const std::shared_ptr<SessionStat> & stat, QWidget * parent = nullptr);
	
	~SessionStatWidget() override;
	
	void closeEvent(QCloseEvent * e) override;

private:
	Ui::SessionStatWidget * ui;
	
signals:
	void closed();
};

#endif // KRUSKAL_SESSIONSTATWIDGET_H
