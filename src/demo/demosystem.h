#ifndef KRUSKAL_DEMOSYSTEM_H
#define KRUSKAL_DEMOSYSTEM_H

#include <QMainWindow>

#include "kruskaldemo.h"
#include "cyclesdemo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DemoSystem; }
QT_END_NAMESPACE

class DemoSystem : public QMainWindow
{
Q_OBJECT

public:
	/// конструктор класса
	explicit DemoSystem(QWidget * parent = nullptr);

	/// деструктор класса
	~DemoSystem() override;

private:
	Ui::DemoSystem * ui;
	KruskalDemo * kruskalDemo; ///< демонстрация алгоритма Краскала
	CyclesDemo * cyclesDemo; ///< демонстрация алгоритма поиска цикла на графе

	void closeEvent(QCloseEvent * e) override;
	
public slots:
	/// переход к демонстрации алгоритма Краскала
	void ShowKruskal();
	/// переход к демонстрации алгоритма проверки включения ребра в цикл
	void ShowCycles();
};

#endif // KRUSKAL_DEMOSYSTEM_H
