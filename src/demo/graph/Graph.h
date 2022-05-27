#ifndef ASD_GRAPH_H
#define ASD_GRAPH_H

#include <cstdint>
#include <stack>

#include <QGraphicsScene>
#include <QObject>

#include "Edge.h"
#include "Vertex.h"

/// ненаправленный связанный взвешенный граф без кратных ребер и петель
class Graph : public QObject
{
Q_OBJECT

	friend class KruskalDemo;
	friend class CyclesDemo;

public:
	/**
	 * @brief конструктор класса
	 * @param numberOfVertexes количество вершин графа
	 * @param numberOfEdges количество ребер графа
	 * @param minEdgeCost минимальная стоимость ребра графа
	 * @param maxEdgeCost максимальная стоимость ребра графа
	 */
	explicit Graph(size_t numberOfVertexes, size_t numberOfEdges, int minEdgeCost, int maxEdgeCost) noexcept;
	
	/**
	 * @brief конструктор копирования
	 * @details ребра и вершины копируются без графических элементов
	 */
	Graph(const Graph & graph) noexcept;
	
	/**
	* @brief генерирует граф
	* @details генерирует ненаправленный связанный взвешенный граф без кратных ребер и петель
	*/
	void Generate() noexcept;
	
	/**
	 * @brief алгоритм Краскала поиска минимального остовного дерева
	 * @details по окончании выполнения алгоритма на сцене будет находиться граф с включенными ребрами, входящими в МОД
	 * @param mst отрисованный на сцене граф, его ребра скрыты (не удалены!)
	 */
	void Kruskal(Graph & mst) noexcept;
	
	/**
	 * @brief рекурсивная функция проверки наличия пути между двумя вершинами
	 * @param path путь (from, to)
	 * @param prev вершина, из которой произведен заход в from. при внешнем вызове может быть равна nullptr
	 * @param from вершина, из которой ищется путь
	 * @param to вершина, в которую ищется путь
	 * @return существует ли путь
	 */
	static bool DoesPathExist(std::list<Edge *> & path, const Vertex * prev, const Vertex * from, const Vertex * to) noexcept;

	/**
	 * @brief пошаговая демонстрация проверки включения ребра в цикл
	 * @param edge ребро, которое необходимо проверить
	 */
	void CheckEdgeInCycleDemo(Edge * edge);
	
	/**
	 * @brief подготовка графа к отрисовке на графической сцене
	 * @details 1. перед вызовом метода предполагается, что ребра графа в списке находятся в том же порядке, в котором
	 * они туда были добавлены.\n
	 * 2. выдача координат вершинам и их буквенным меткам\n
	 * 3. генерация линий ребер и выдача координат меткам их стоимости
	 */
	void PrepareToPaint(const QPoint & _graphCentre, int64_t _graphDiameter, QGraphicsScene * _graphicsScene);
	
	/**
	 * @brief отрисовка графа ина графической сцене
	 * @param _graphCentre центр окружности, на которой располагаются вершины графа
	 * @param _graphDiameter диаметр графа
	 * @param graphicsScene графическая сцена, на которой будет отрисован граф
	 */
	void Paint(uint32_t edgeColor, uint32_t vertexColor);
	
	/// сокрытие ребер графа с графической сцены
	void HideEdges();
	
	/**
	 * @drief поиск ребра графа по меткам его вершин
	 * @details выполняется поиск ребра (labelFrom, labelTo) или (labelTo, labelFrom)
	 * @return указатель на найденное ребро или nullptr, если ребро не найдено в графе
	 */
	Edge * FindEdge(char labelFrom, char labelTo) noexcept;
	
	/// поиск ребра в графе по буквенным меткам ребра edge
	Edge * FindEdge(const Edge & edge) noexcept;

private:
	const size_t NUMBER_OF_VERTEXES; ///< количество вершин графа
	const size_t NUMBER_OF_EDGES; ///< количество ребер графа
	const int MIN_EDGE_COST; ///< минимальная стоимость ребра графа
	const int MAX_EDGE_COST; ///< максимальная стоимость ребра графа
	
	/*
	 * почему list вместо vector.
	 * так как члены класса в памяти располагаются последовательно и размер векторов в памяти может
	 * увеличиваться, в конце концов один вектор перекроет второй, из-за чего ими нельзя будет
	 * нормально пользоваться.
	 * для избежания проблемы используются списки.
	 */
	std::list<Vertex> vertexes; ///< множество вершин графа
	std::list<Edge> edges; ///< множество ребер графа
	
	QGraphicsScene * scene; ///< графическая сцена, на которую добавляется граф
	QPoint graphCentre; ///< координаты центра окружности графа
	uint64_t graphDiameter; ///< диаметр окружности графа

	/**
	 * @brief поиск вершины по ее буквенной метке
	 * @param label буквенная метка
	 * @return указатель на найденную вершину или nullptr, если вершина не найдена в графе
	 */
	Vertex * findVertex(char label) noexcept;
	
signals:
	/// сигнал сохранение состояния сцены
	void takeSnapshot(QGraphicsScene * scene) const;
	/// стек изменился
	void stackChanged(const std::stack<std::array<Vertex *, 2>> & st) const;
};

#endif //ASD_GRAPH_H
