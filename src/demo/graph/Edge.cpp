#include <QGraphicsScene>
#include <stdexcept>

#include "Edge.h"
#include "Vertex.h"

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

/**
 * @brief подготовка объекта к отрисовке
 * @details 1. сохранение графической сцены, на которой будет отрисован объект\n
 * 2. создание линии ребра, которая будет добавлена на графическую сцену
 * @param graphCentre центра графа, которому принадлежит точка
 * @param graphicsScene графическая сцена, на которую будет добавлен объект
 */
void Edge::PrepareToPaint(const QPoint & graphCentre, QGraphicsScene * graphicsScene)
{
	if (!graphicsScene) { throw invalid_argument("internalScene не может быть нулевым указателем"); }
	scene = graphicsScene;
	edge = make_shared<QLine>(QLine(from->Coordinates(), to->Coordinates()));
	costPos = make_shared<QPoint>(FindCostPos(graphCentre));
}

/**
 * @brief поиск координат метки стоимости ребра графа
 * @param graphCentre центр окружности графа, которому принадлежит текущее ребро
 * @return найденная позиция метки
 */
QPoint Edge::FindCostPos(const QPoint & graphCentre) const noexcept
{
	/*
	 * 1. формируется вектор с началом в центре графа и концом в середине текущего ребра
	 * 2. длина полученного вектора увеличивается на некоторое значение
	 * 3. определяются смещения по x и по y для новой длины
	 * 4. смещения координат прибавляются к конечной вершине вектора:
	 *        если координата конечной вершины меньше координаты начальной вершины,
	 *        то смещение вычитается из координаты, иначе - прибавляется
	 */
	
	QLineF vector(graphCentre, edge->center()); // геометрический вектор из центра графа до текущей точки
	double length = vector.length(); // текущая длина вектора
	double newLength = vector.length() + 20; // новая длина вектора
	
	double x = abs(vector.x2() - vector.x1()); // смещение вектора по х
	double y = abs(vector.y2() - vector.y1()); // смещение вектора по y
	if (x == 0) { ++x; } // если возникнет треугольник с нулевым смещением по x, то программа не выдаст неверный ответ
	double dx = x * newLength / length - x; // считаем с помощью косинуса
	double dy = y * (x + dx) / x - y; // считаем с помощью тангенса
	
	QPointF pos = vector.p2();
	
	if (vector.x2() < vector.x1()) { pos.setX(pos.x() - dx - 10); }
	else { pos.setX(pos.x() + dx - 15); }
	
	if (vector.y2() < vector.y1()) { pos.setY(pos.y() - dy - 5); }
	else { pos.setY(pos.y() + dy - 15); }
	
	return pos.toPoint();
}

/**
 * @brief добавление объекта на графическую сцену
 * @param pen ручка, которой будет отрисован объект
 */
void Edge::Paint(const QPen & pen)
{
	graphicsEdge = scene->addLine(*edge, pen);
	graphicsCost = scene->addText(QString::number(cost));
	graphicsCost->setPos(*costPos);
	graphicsCost->setFont(QFont("Helvetica", 9, 10));
}

/// сокрытые ребра с графической сцены
void Edge::Hide() noexcept
{
	if (graphicsEdge) { graphicsEdge->hide(); }
	if (graphicsCost) { graphicsCost->hide(); }
}

/// отображение ребра на графической сцене
void Edge::Show() noexcept
{
	if (graphicsEdge) { graphicsEdge->show(); }
	if (graphicsCost) { graphicsCost->show(); }
}

/// возвращает метку ребра
QString Edge::GetName() const
{
	return QString(from->GetLabel()) + QString(to->GetLabel());
}
