#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <stdexcept>

#include "Vertex.h"
#include "Edge.h"

#include "../../common/constants.h"
#ifndef NDEBUG
  #include <QDebug>
#endif

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

/**
 * @brief подготовка объекта к отрисовке
 * @details 1. сохранение графической сцены, на которой будет отрисован объект\n
 * 2. создание линии ребра, которая будет добавлена на графическую сцену
 * @param graphCentre центра графа, которому принадлежит точка
 * @param pointCoordinates координаты левого верхнего угла эллипса на графической сцене
 * @param pointSize размер точки на графической сцене
 * @param graphicsScene графическая сцена, на которую будет добавлен объект
 */
void Vertex::PrepareToPaint(
		const QPoint & graphCentre,
		const QPoint & pointCoordinates,
		const QSize & pointSize,
		QGraphicsScene * graphicsScene
)
{
	if (!graphicsScene) { throw invalid_argument("internalScene не может быть нулевым указателем"); }
	scene = graphicsScene;
	ellipse = make_shared<QRect>(QRect(pointCoordinates, pointSize));
	labelPos = make_shared<QPoint>(FindLabelPos(graphCentre));
}

/**
 * @brief поиск координат метки вершины графа
 * @param graphCentre центр окружности графа, которому принадлежит текущая вершина
 * @return найденная позиция метки
 */
QPoint Vertex::FindLabelPos(const QPoint & graphCentre)
{
	/*
	 * 1. формируется вектор с началом в центре графа и концом в текущей вершине
	 * 2. длина полученного вектора увеличивается на некоторое значение
	 * 3. определяются смещения по x и по y для новой длины
	 * 4. смещения координат прибавляются к конечной вершине вектора:
	 *        если координата конечной вершины меньше координаты начальной вершины,
	 *        то смещение вычитается из координаты, иначе - прибавляется
	 */
	
	QLineF vector(graphCentre, ellipse->topLeft()); // геометрический вектор из центра графа до текущей точки
	double length = vector.length(); // текущая длина вектора
	double newLength = vector.length() + 25; // новая длина вектора
	
	double x = abs(vector.x2() - vector.x1()); // смещение вектора по х
	double y = abs(vector.y2() - vector.y1()); // смещение вектора по y
	if (x == 0) { ++x; } // если возникнет треугольник с нулевым смещением по x, то программа не выдаст неверный ответ
	double dx = x * newLength / length - x; // считаем с помощью косинуса
	double dy = y * (x + dx) / x - y; // считаем с помощью тангенса
	
	QPointF pos = vector.p2();
	if (vector.x2() < vector.x1()) { pos.setX(pos.x() - dx); }
	else { pos.setX(pos.x() + dx); }
	
	if (vector.y2() < vector.y1()) { pos.setY(pos.y() - dy); }
	else { pos.setY(pos.y() + dy - 15); }
	
	return pos.toPoint();
}

/**
 * @brief добавление объекта на графическую сцену
 * @param pen ручка, которой будет отрисован объект
 * @param brush кисть, которой будет заполнен объект
 */
void Vertex::Paint(const QPen & pen, const QBrush & brush)
{
	graphicsEllipse = scene->addEllipse(*ellipse, pen, brush);
    graphicsLabel = scene->addText("");
    graphicsLabel->setHtml("<font color=#D68910>" + QString(label) + "</font>"); // темно-оранжевый цвет
	graphicsLabel->setPos(*labelPos);
}

/// отображение вершины на графической сцене
void Vertex::Show()
{
	if (graphicsEllipse) { graphicsEllipse->show(); }
	if (graphicsLabel) { graphicsLabel->show(); }
}

void Vertex::SetColor(int color)
{
    QString hexColor = QString( "%1" ).arg(color, 0, 16);
    graphicsLabel->setHtml("<font color=#" + hexColor + ">" + QString(label) + "</font>");
}





