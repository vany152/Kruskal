#ifndef ASD_CYCLESDEMO_H
#define ASD_CYCLESDEMO_H

#include <QDir>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtSvgWidgets/QGraphicsSvgItem>

#include "graph/Graph.h"
#include "../common/constants.h"
#include "../common/documents.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CyclesDemo; }
QT_END_NAMESPACE

class CyclesDemo : public QWidget
{
Q_OBJECT

using demoStepsIterator = std::list<QGraphicsSvgItem*>::iterator;

public:
	/// конструктор класса
	explicit CyclesDemo(QWidget * parent = nullptr) noexcept;
	
	/// деструктор класса
	~CyclesDemo() override;
	
private:
	Ui::CyclesDemo * ui;
	
	QGraphicsScene * scene; ///< основная сцена, на которую выводится пошаговая демонстрация
	QGraphicsScene * internalScene; ///< графическа¤ сцена, на которой располагаетс¤ граф
	std::shared_ptr<Graph> graph; ///< граф
	QGraphicsTextItem * stackView; ///< просмотр стека на графической сцене
	
	std::list<QGraphicsSvgItem *> demoSteps; ///< список шагов демонстрации
	demoStepsIterator currentStep; ///< текущий шаг демонстрации
	
	QDir tmp; ///< временна¤ директори¤, в которую сохран¤ютс¤ снапшоты графической сцены дл¤ пошаговй демонстрации
	/*
	 * если именовать файлы с 0 при большом количестве шагов (больше 10) возникают проблемы сортировки файлов со
	 * снапшотами по имени: 1 < 11 < 2
	 */
	int snapshotNumber = 100; ///< номер текущего снапшота
	
	/// генераци¤ случайных параметров дл¤ создани¤ графа
	static std::shared_ptr<Graph> generateGraph() noexcept;
	
	/// загрузка снапшотов в пам¤ть программы
	void loadDemo();

public slots:
	/// нажатие на кнопку предыдущего шага
	void on_prevStepButton_clicked();
	/// нажатие на кнопку следующего шага
	void on_nextStepButton_clicked();
	/// нажатие на кнопку перемещения в начало демонстрации
	void on_toBeginOfDemoButton_clicked();
	/// нажатие на кнопку перемещения в конец демонстрации
	void on_toEndOfDemoButton_clicked();
	/// нажатие на кнопку начала демонстрации
	void on_startButton_clicked();
	
private slots:
	/// сохранение снапшота графической сцены
	void takeSnapshot(QGraphicsScene* sceneToSave);
	/// вывод стека на графическую сцену
	void displayStack(const std::stack<std::array<Vertex *, 2>> & st) noexcept;
	/// нажатие на кнопку перехода к демонстрации алгоритма Краскала
	void on_toKruskalDemoButton_clicked() { emit showKruskal(); }

signals:
	/// сигнал перехода к демонстрации алгоритма Краскала
	void showKruskal();
};

#endif //ASD_CYCLESDEMO_H
