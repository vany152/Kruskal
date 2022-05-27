#ifndef ASD_STATWIDGET_H
#define ASD_STATWIDGET_H

#include <QWidget>
#include <QCloseEvent>

#include "stat.h"

using std::shared_ptr;

QT_BEGIN_NAMESPACE
namespace Ui { class StatWidget; }
QT_END_NAMESPACE

class StatWidget : public QWidget
{
Q_OBJECT

public:
	explicit StatWidget(const shared_ptr<Stat> & stat, QWidget * parent = nullptr);
	
	~StatWidget() override;
	
	void closeEvent(QCloseEvent * e) override;

private:
	Ui::StatWidget * ui;
	shared_ptr<Stat> stat; ///< указатель на статистику тестирования
	
signals:
	void closed();
};

#endif //ASD_STATWIDGET_H
