#ifndef ASD_EDGE_H
#define ASD_EDGE_H

#include <QGraphicsLineItem>

class Vertex;

/// ребро графа
class Edge
{
	friend class Graph;
	
public:
	/**
	 * конструктор класса
	 * @param from начальная вершина ребра
	 * @param to конечная вершина ребра
	 * @param cost стоимости ребра
	 */
	Edge(Vertex * from, Vertex * to, int cost) noexcept : from(from), to(to), cost(cost) { } ;
	
	/**
	 * @brief подготовка объекта к отрисовке
	 * @details 1. сохранение графической сцены, на которой будет отрисован объект\n
	 * 2. создание линии ребра, которая будет добавлена на графическую сцену
	 * @param graphCentre центра графа, которому принадлежит точка
	 * @param graphicsScene графическая сцена, на которую будет добавлен объект
	 */
	void PrepareToPaint(const QPoint & graphCentre, QGraphicsScene * graphicsScene);
	
	/**
	 * @brief поиск координат метки стоимости ребра графа
	 * @param graphCentre центр окружности графа, которому принадлежит текущее ребро
	 * @return найденная позиция метки
	 */
	QPoint FindCostPos(const QPoint & graphCentre) const noexcept;
	
	/**
	 * @brief добавление объекта на графическую сцену
	 * @param pen ручка, которой будет отрисован объект
	 */
	void Paint(const QPen & pen);
	
	/// изменение ручки, которой нарисован объект
	void SetPen(const QPen & pen) { graphicsEdge->setPen(pen); }
	
	/// сокрытые ребра с графической сцены
	void Hide() noexcept;
	
	/// отображение ребра на графической сцене
	void Show() noexcept;
	
	/// возвращает метку ребра
	QString GetName() const;
	
private:
	Vertex * from; ///< начальная вершина ребра
	Vertex * to; ///< конечная вершина ребра
	int cost; ///< стоимость ребра
	
	QGraphicsScene * scene; ///< графическая сцена, на которую выводится объект
	std::shared_ptr<QPoint> costPos; ///< позиция стоимости ребра на графической сцене
	std::shared_ptr<QLine> edge; ///< линия, описывающая ребро на графической сцене
	QGraphicsLineItem * graphicsEdge; ///< указатель на добавленное на графическую сцену ребро
	QGraphicsTextItem * graphicsCost; ///< указатель на добавленную на графическую сцену стоимость ребра
};

#endif //ASD_EDGE_H
