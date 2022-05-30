#ifndef KRUSKAL_VERTEX_H
#define KRUSKAL_VERTEX_H

class Edge;

/// вершина графа
class Vertex
{
	friend class Graph;

public:
	enum State { Open, Locked };
	State state; ///< состояние вершины, необходимо для алгоритма поиска цикла на графе
	
	/**
	 * @brief конструктор класса
	 * @param label буквенная метка вершины
	 */
	explicit Vertex(char label) :
			label(label),
			state(Open),
			scene(nullptr),
			labelPos(nullptr),
			ellipse(nullptr),
			graphicsEllipse(nullptr),
			graphicsLabel(nullptr) {}
	
	/**
	 * @brief подготовка объекта к отрисовке
	 * @details 1. сохранение графической сцены, на которой будет отрисован объект\n
	 * 2. создание линии ребра, которая будет добавлена на графическую сцену
	 * @param graphCentre центра графа, которому принадлежит точка
	 * @param pointCoordinates координаты левого верхнего угла эллипса на графической сцене
	 * @param pointSize размер точки на графической сцене
	 * @param graphicsScene графическая сцена, на которую будет добавлен объект
	 */
	void PrepareToPaint(
			const QPoint & graphCentre,
			const QPoint & pointCoordinates,
			const QSize & pointSize,
			QGraphicsScene * graphicsScene
	);
	
	/**
	 * @brief поиск координат метки вершины графа
	 * @param graphCentre центр окружности графа, которому принадлежит текущая вершина
	 * @return найденная позиция метки
	 */
	QPoint FindLabelPos(const QPoint & graphCentre);
	
	/**
	 * @brief добавление объекта на графическую сцену
	 * @param pen ручка, которой будет отрисован объект
	 * @param brush кисть, которой будет заполнен объект
	 */
	void Paint(const QPen & pen, const QBrush & brush);
	
	/// возвращает координаты центра точки
	QPoint Coordinates() { return ellipse->center(); }
	
	/// отображение вершины на графической сцене
	void Show();
	
	void SetColor(int color);
	
	char GetLabel() { return label; }

private:
	char label; ///< буквенная метка вершины
	std::vector<Edge *> edges; ///< массив указателей на ребра, исходяще (или входящие) из вершины
	
	// графическая часть
	QGraphicsScene * scene; ///< графическая сцена, на которую выводится объект
	std::shared_ptr<QPoint> labelPos; ///< позиция буквенной метки вершины на графической сцене
	std::shared_ptr<QRect> ellipse; ///< прямоугольник, описывающий эллипс вершины на графической сцене
	QGraphicsEllipseItem * graphicsEllipse; ///< указатель на добавленный на графическую сцену эллипс
	QGraphicsTextItem * graphicsLabel; ///< указатель на добавленную на графическую сцену подпись вершины
};

#endif // KRUSKAL_VERTEX_H
