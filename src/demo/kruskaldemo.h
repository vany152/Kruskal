#ifndef KRUSKAL_KRUSKALDEMO_H
#define KRUSKAL_KRUSKALDEMO_H

#include <QDir>
#include <QWidget>
#include <QGraphicsScene>
#include <QtSvgWidgets/QGraphicsSvgItem>

#include "graph/Graph.h"
#include "../common/constants.h"
#include "../common/documents.h"

QT_BEGIN_NAMESPACE
namespace Ui { class KruskalDemo; }
QT_END_NAMESPACE

class KruskalDemo : public QWidget
{
Q_OBJECT

using demoStepsIterator = std::list<QGraphicsSvgItem*>::iterator;

public:
	/// конструктор класса
	explicit KruskalDemo(QWidget * parent = nullptr);

	/// деструктор класса
	~KruskalDemo() override;

private:
	Ui::KruskalDemo * ui;
	
	QGraphicsScene * scene; ///< основная сцена, на которую выводится пошаговая демонстрация
	QGraphicsScene * internalScene; ///< графическая сцена, на которой располагается граф
	std::shared_ptr<Graph> graph; ///< граф
	std::shared_ptr<Graph> mst; ///< минимальное остовное дерево графа
	
	std::list<QGraphicsSvgItem *> demoSteps; ///< список шагов демонстрации
	demoStepsIterator currentStep; ///< текущий шаг демонстрации
	
	QDir tmp; ///< временная директория, в которую сохраняются снапшоты графической сцены для пошаговй демонстрации
	/*
	 * если именовать файлы с 0 при большом количестве шагов (больше 10) возникают проблемы сортировки файлов со
	 * снапшотами по имени: 1 < 11 < 2
	 */
	int snapshotNumber = 100; ///< номер текущего снапшота
	
	/// генерация случайных параметров для создания графа
	static std::shared_ptr<Graph> generateGraph() noexcept;
	
	/// загрузка снапшотов в память программы
	void loadDemo();
	
public slots:
	/// нажатие на кнопку предыдущего шага
	void on_prevStepButton_clicked();
	/// нажатие на кнопку следующего шага
	void on_nextStepButton_clicked();
	/// нажатие на кнопку перехода в начало демонстрации
	void on_toBeginOfDemoButton_clicked() ;
	/// нажатие на кнопку перехода в конец демонстрации
	void on_toEndOfDemoButton_clicked();
	
private slots:
	/// сохранение снапшота графической сцены
	void takeSnapshot(QGraphicsScene * sceneToSave);
	/// нажатие на кнопку перехода к демонстрации алгоритма проверки включения ребра в цикл
	void on_toCyclesDemoButton_clicked() { emit showCycles(); }

signals:
	/// сигнал перехода к демонстрации алгоритма проверки включения ребра в цикл
	void showCycles();
};

#endif // KRUSKAL_KRUSKALDEMO_H
