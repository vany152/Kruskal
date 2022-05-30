#include <random>
#include <vector>
#include <ctime>
#include <stack>
#include <array>
#include <stdexcept>

#include "Graph.h"
#include "../../common/constants.h"

#ifndef NDEBUG
  #include <QDebug>
#endif

using namespace std;

/** **************************************************** PUBLIC **************************************************** **/

/**
 * @brief конструктор класса
 * @details количество вершин не должно быть нечетным и меньше 6
 * @param numberOfVertexes количество вершин графа
 * @param numberOfEdges количество ребер графа
 * @param minEdgeCost минимальная стоимость ребра графа
 * @param maxEdgeCost максимальная стоимость ребра графа
 */
Graph::Graph(
		size_t numberOfVertexes,
		size_t numberOfEdges,
		int minEdgeCost,
		int maxEdgeCost
) noexcept:
		NUMBER_OF_VERTEXES(numberOfVertexes),
		NUMBER_OF_EDGES(numberOfEdges),
		MIN_EDGE_COST(minEdgeCost),
		MAX_EDGE_COST(maxEdgeCost)
{
	// создание вершин графа
	for (size_t i = 0; i < NUMBER_OF_VERTEXES; ++i) { vertexes.push_back(Vertex(65 + i)); }
}

Graph::Graph(const Graph & graph) noexcept:
		NUMBER_OF_VERTEXES(graph.NUMBER_OF_VERTEXES),
		NUMBER_OF_EDGES(graph.NUMBER_OF_EDGES),
		MIN_EDGE_COST(graph.MIN_EDGE_COST),
		MAX_EDGE_COST(graph.MAX_EDGE_COST)
{
	/*
	 * копирование вершин
	 */
	for (auto & vertex: graph.vertexes) { vertexes.emplace_back(vertex.label); }
	
	/*
	 * связывание вершин ребрами
	 */
	for (auto & edge: graph.edges)
	{
		Vertex * from = findVertex(edge.from->label);
		Vertex * to = findVertex(edge.to->label);
		
		edges.emplace_back(from, to, edge.cost);
		
		from->edges.push_back(&edges.back());
		to->edges.push_back(&edges.back());
	}
}

/**
 * @brief генерирует граф
 * @details генерирует ненаправленный связанный взвешенный граф без кратных ребер и петель
 */
void Graph::Generate() noexcept
{
    mt19937 gen(time(nullptr));
	
	/*
	 * алгоритм генерации графа
	 *
	 * генерация связного графа выполняется в два прохода:
	 * в первом проходе множество вершин разделяется на неиспользованные (исходное множество вершин)
	 * и использованные. при добавлении вершины в множество использованных, она удаляется из множества
	 * неиспользованных.
	 * список использованных вершин инициализируется случайной вершиной,
	 * далее на каждом цикле к последней использованной вершине присоединяется случайная неиспользованная вершина.
	 * в результате первого прохода формируется связная основа (являющаяся деревом).
	 *
	 * второй проход напоминает первый, за исключением нескольких различий:
	 * 1. множество неиспользованных вершин инициализируется вершинами связной основы,
	 * 2. последняя использованная вершина связывается со случайной вершиной, не подсоединенной к ней.
	 */
	
	uniform_int_distribution<size_t> randomVertex(0, NUMBER_OF_VERTEXES - 1); // генерирует рандомные индексы
	uniform_int_distribution<int> randomEdgeCost(MIN_EDGE_COST, MAX_EDGE_COST); // генерирует рандомные веса ребер
	
	vector<Vertex *> unusedVertexes;
	for (auto & i: vertexes) { unusedVertexes.push_back(&i); }
	
	/*
	 * в конце этапа массив edges содержит NUMBER_OF_VERTEXES ребер
	 */
	vector<Vertex *> usedVertexes;
	usedVertexes.push_back(unusedVertexes.front()); // инициализируем использованные вершины первой неиспользованной
	unusedVertexes.front() = nullptr;
	
	size_t index; // индекс новой вершины
	while (usedVertexes.size() != NUMBER_OF_VERTEXES)
	{
		index = randomVertex(gen); // генерируем индекс новой вершины
		
		if (unusedVertexes[index] != nullptr) // если она не использована
		{
			Edge newEdge(
					usedVertexes.back(), // из последней использованной вершины
					unusedVertexes[index], // в случайную неиспользуемую
					randomEdgeCost(gen)
			);
			edges.push_back(newEdge); // добавляем сгенерированное ребро
			usedVertexes.back()->edges.push_back(&edges.back());
			unusedVertexes[index]->edges.push_back(&edges.back()); // сохраняем новое ребро в текущей вершине
			
			usedVertexes.push_back(unusedVertexes[index]); // сохраняем использованную вершину
			unusedVertexes[index] = nullptr; // помечаем вершину как использованную
		}
	}
	
	/*
	 * в конце этапа массив edges содержит NUMBER_OF_EDGES ребер
	 */
	size_t oldIndex = 0;
	unusedVertexes = usedVertexes;
	
	usedVertexes.resize(0); // убираем лишние элементы из вектора
	index = randomVertex(gen);
	usedVertexes.push_back(unusedVertexes[index]); // инициализируем множество использованных вершин случайной вершиной
	unusedVertexes[index] = nullptr;
	oldIndex = index; // запоминаем использованный индекс
	
	while (edges.size() < NUMBER_OF_EDGES)
	{
		index = randomVertex(gen);
		if ((unusedVertexes[index] != nullptr) && (index != oldIndex - 1) && (index != oldIndex + 1))
		{
			Edge newEdge(
					usedVertexes.back(), // из последней использованной вершины
					unusedVertexes[index], // в случайную неиспользуемую
					randomEdgeCost(gen)
			);
			edges.push_back(newEdge);
			usedVertexes.back()->edges.push_back(&edges.back());
			unusedVertexes[index]->edges.push_back(&edges.back());
			
			usedVertexes.push_back(unusedVertexes[index]); // сохраняем использованную вершину
			unusedVertexes[index] = nullptr; // помечаем вершину как использованную
			oldIndex = index; // сохраняем использованный индекс
		}
	}
}

/**
 * @brief алгоритм Краскала поиска минимального остовного дерева
 * @details по окончании выполнения алгоритма на сцене будет находиться граф с включенными ребрами, входящими в МОД
 * @param mst отрисованный на сцене граф, его ребра скрыты (не удалены!)
 */
void Graph::Kruskal(Graph & mst) noexcept
{
	/*
	 * алгоритм Краскала:
	 * 1) ребра графа сортируются в порядке неубывания стоимости
	 * 2) в минимальное остовное дерево (МОД) добавляется число вершин, равное количеству вершин текущего графа
	 * 3) рассматриваем ребра текущего графа:
	 *        если текущее ребро не создает цикла в графе, добавляем его в МОД.
	 *        (вместо добавления ребра в МОД и проверки, создает ли оно цикл, достаточно
	 *         проверить, существует ли путь в МОД между конечными вершинами МОД. если
	 *         существует, добавление ребра создаст цикл, иначе - цикла не возникнет)
	 */
	
	// internalMst - minimum spanning tree - минимальное остовное дерево
	Graph internalMst(NUMBER_OF_VERTEXES, NUMBER_OF_EDGES, MIN_EDGE_COST, MAX_EDGE_COST);
	
	// сортируем ребра текущего графа по неубыванию стоимости
	auto edgeLess = [](const Edge & first, const Edge & second) -> bool { return first.cost < second.cost; };
	edges.sort(edgeLess);
	
	emit takeSnapshot(scene); // сохраняем сцену с первым шагом демонстрации
	for (auto & edge: edges)
	{
		/*
		 * так как вершины в списке вершин именуются в алфавитном порядке, без проблем можно получить
		 * индекс вершины по ее буквенному обозначению (обозначение - английская заглавная буква)
		 */
		auto it = internalMst.vertexes.begin();
		advance(it, edge.from->label - 65);
		Vertex * mstPathFrom = &(*it); // вершина МОД, соответствующая началу ребра в текущем графе
		
		it = internalMst.vertexes.begin();
		advance(it, edge.to->label - 65);
		Vertex * mstPathTo = &(*it);  // вершина МОД, соответствующая концу ребра в текущем графе
		
		Edge * graphicsMstEdge = mst.FindEdge(mstPathFrom->label, mstPathTo->label);
		list<Edge *> path; // путь между вершинами mstPathFrom и mstPathTo
		// существует ли в МОД путь между концами текущего ребра
		if (!internalMst.DoesPathExist(path, 0, mstPathFrom, mstPathTo))
		{
			/*
             * если путь не существует, добавляем ребро в internalMst и включаем его на отрисованном mst,
             * так же подсвечиваем текущее рассматриваемое ребро на основном графе
			 */
			edge.SetPen(QPen(QBrush(0x2eb8b8), 2));
			
			internalMst.edges.emplace_back(mstPathFrom, mstPathTo, edge.cost); // добавляем ребро в МОД
			/*
			 * указываем вершинам МОД, что они соединены ребром
			 */
			mstPathFrom->edges.push_back(&internalMst.edges.back());
			mstPathTo->edges.push_back(&internalMst.edges.back());
			
			graphicsMstEdge->Show(); // ребро отрисованного МОД, соответствующее добавляемому
		}
		else
		{
			/*
			 * если путь существует, рассматриваемое ребро создает цикл.
			 * включаем рассматриваемое ребро на отрисованном mst, выделяем цикл и выводим сообщение, удаляем ребро с mst
			 */
			
			// если ребро не подходит, окрашиваем его в другой цвет
			edge.SetPen(QPen(QBrush(0xff1a1a), 2));
			
			// показываем на отрисованном МОД цикл, затем выключаем добавленное ребро
			graphicsMstEdge->SetPen(QPen(QBrush(0xff1a1a), 2));
			graphicsMstEdge->Show();
			for (auto & _edge: path) { mst.FindEdge(*_edge)->SetPen(QPen(QBrush(0xff1a1a), 2)); }
			
			// выводим сообщение о том, что ребро создает цикл
			QGraphicsTextItem * message = scene->addText("");
			message->setHtml(
					"Ребро <font color=\"#239b56\">" +
					QString(mstPathFrom->GetLabel()) + QString(mstPathTo->GetLabel()) +
					"</font> создает цикл"
			);
			message->setPos(mst.graphCentre.x() - 75, mst.graphCentre.y() - 200);
			emit takeSnapshot(scene); // сохраняем сцену с циклом и сообщением
			delete message; // удаляем сообщение
			
			graphicsMstEdge->Hide();
			for (auto & _edge: path) { mst.FindEdge(*_edge)->SetPen(QPen(QBrush(Qt::black), 2)); }
		}
		
		emit takeSnapshot(scene); // сохраняем сцену
		edge.SetPen(QPen(Qt::black, 2)); // возвращаем черный цвет ребру
	}
	
	/*
	 * выводим сообщение о том, что МОД неайдено
	 */
	QGraphicsTextItem * message = scene->addText("Минимальное остовное дерево найдено");
	message->setPos(mst.graphCentre.x() - 75, mst.graphCentre.y() + 175);
	
	emit takeSnapshot(scene); // сохраняем сцену с последним шагом демонстрации
}

/**
 * @brief рекурсивная функция проверки наличия пути между двумя вершинами
 * @param path путь (from, to)
 * @param prev вершина, из которой произведен заход в from. при внешнем вызове может быть равна nullptr
 * @param from вершина, из которой ищется путь
 * @param to вершина, в которую ищется путь
 * @return существует ли путь
 */
bool Graph::DoesPathExist(list<Edge *> & path, const Vertex * prev, const Vertex * from, const Vertex * to) noexcept
{
	/*
	 * используется алгоритм обхода в глубину
	 */
	
	if (from == to) { return true; } // если нашли пришли в нужную вершину, значит, путь найден
	
	bool result = false; // найден ли путь from->to
	for (const auto & edge: from->edges)
	{
		path.push_back(edge);
		Vertex * nextFrom = (edge->from == from) ? edge->to : edge->from;
		
		/*
		 * если следующая вершина не та, из которой мы пришли, рассматриваются ее ребра.
		 * (так как граф ненаправленный, во избежание циклов в пути одно и то же ребро
		 * не рассматривается дважды)
		 */
		if (nextFrom != prev) { result = DoesPathExist(path, from, nextFrom, to); }
		/*
		 * как только находим путь, выходим из цикла и схлопываем рекурсию.
		 * если цикл не прервать, и через последующие ребра нельзя будет добраться до вершины to,
		 * рекурсия вернет значение false
		 */
		if (result) { break; }
		else { path.pop_back(); }
	}
	
	return result;
}

/**
 * @brief пошаговая демонстрация проверки включения ребра в цикл
 * @param edge ребро, которое необходимо проверить
 */
void Graph::CheckEdgeInCycleDemo(Edge * edge)
{
	if (!edge) { throw std::invalid_argument("указатель на ребро не может быть нулевым"); }
	
	/*
	 * используется итеративный алгоритм обхода в глубину
	 */
	
	QPen coloredPen(QBrush(0xff1a1a), 2), defaultPen(QBrush(Qt::black), 2);
	
    Vertex * end = (edge->from->edges.size() == 1) ? edge->from : edge->to; // вершина, к которой ищется путь
	Vertex * current = (end == edge->from) ? edge->to : edge->from; // текущая вершина
	Vertex * prev = end; // предыдущая вершина
	prev->state = Vertex::Locked;
	
	stack<array<Vertex *, 2>> path; // путь, представлен ребрами - парами вершин
	path.push({prev, current}); // добавляем в путь первую вершину
	
	bool stop = false;
	
	emit takeSnapshot(scene); // сохраняем сцену до начала алгоритма
	emit stackChanged(path); // отображаем изменения стека ребер
	while (!stop && !path.empty())
	{
		/*
		 * снимаем со стека последнее добавленное ребро
		 */
		prev = path.top().at(0);
		current = path.top().at(1);
		
		/*
		 * ищем это ребро в графе и окрашиваем его
		 */
		Edge * currentEdge = FindEdge(prev->label, current->label);
		currentEdge->SetPen(coloredPen);
		currentEdge->to->SetColor(0xff1a1a);
		
		/*
		 * если попали в открытую точку, перекрашиваем ее до снапшота
		 */
		if (current->state == Vertex::Open) { current->SetColor(0xff1a1a); }
		emit takeSnapshot(scene); // сохраняем состояние сцены
		
		if (current == end) { stop = true; } // если пришли в искомую вершину, алгоритм завершен
		
		if (!stop && current->state == Vertex::Open) // если пришли в открытую вершину
		{
			current->state = Vertex::Locked; // закрываем ее
			
			size_t stackSizeBefore = path.size();
			for (auto & _edge: current->edges) // добавляем ее ребра в стек
			{
				Vertex * next = (_edge->from == current) ? _edge->to : _edge->from;
				if (next != prev) { path.push({current, next}); }
			}
			if (path.size() != stackSizeBefore) { emit stackChanged(path); } // отображаем изменения стека ребер
		}
		else if (!stop && current->state == Vertex::Locked) // если пришли в закрытую вершину
		{
			currentEdge->SetPen(defaultPen); // красим ее в обычный цвет
			path.pop(); // удаляем из пути ребро с концом в текущей вершине
			
			emit stackChanged(path); // отображаем изменения стека ребер
			emit takeSnapshot(scene); // сохраняем состояние сцены
		}
	}
	
	/*
	 * выводим результат: содержится ли ребро в цикле
	 */
	QGraphicsTextItem * message = scene->addText("");
	if (!path.empty())
	{
		message->setHtml("Ребро <font color=\"#239b56\">" + edge->GetName() + "</font> содержится в цикле");
	}
	else
	{
		message->setHtml("Ребро <font color=\"#239b56\">" + edge->GetName() + "</font> не входит в цикл");
	}
	message->toHtml();
	message->setPos(graphCentre.x() - 75, graphCentre.y() + 175);
	
	emit takeSnapshot(scene); // сохраняем состояние сцены в конце алгоритма
}

/**
 * @brief подготовка графа к отрисовке на графической сцене
 * @details 1. перед вызовом метода предполагается, что ребра графа в списке находятся в том же порядке, в котором
 * они туда были добавлены.\n
 * 2. выдача координат вершинам и их буквенным меткам\n
 * 3. генерация линий ребер и выдача координат меткам их стоимости
 */
void Graph::PrepareToPaint(const QPoint & _graphCentre, int64_t _graphDiameter, QGraphicsScene * _graphicsScene)
{
	if (!_graphicsScene) { throw invalid_argument("graphicsScene не может быть нулевым указателем"); }
	scene = _graphicsScene;
	graphCentre = _graphCentre;
	graphDiameter = _graphDiameter;
	
	/*
	 * выдача координат вершинами
	 */
	// первую вершину обрабатываем отдельно
	Vertex * firstVertex = edges.front().from;
	firstVertex->PrepareToPaint(
			graphCentre,
			QPoint(
					graphCentre.x(),
					graphDiameter + graphCentre.y()
			),
			QSize(7, 7),
			scene
	);
	
	auto it = edges.begin();
	for (size_t i = 1; i < NUMBER_OF_VERTEXES; ++i, ++it)
	{
		Vertex * currentVertex = it->to;
		QPoint currentCoordinates(
				sin(i * 2 * PI / NUMBER_OF_VERTEXES) * graphDiameter + graphCentre.x(),
				cos(i * 2 * PI / NUMBER_OF_VERTEXES) * graphDiameter + graphCentre.y()
		);
		currentVertex->PrepareToPaint(graphCentre, currentCoordinates, QSize(7, 7), scene);
	}
	
	/*
	 * генерация линий ребер
	 */
	for (auto & edge: edges) { edge.PrepareToPaint(graphCentre, scene); }
}

/**
 * @brief отрисовка графа ина графической сцене
 * @param _graphCentre центр окружности, на которой располагаются вершины графа
 * @param _graphDiameter диаметр графа
 * @param graphicsScene графическая сцена, на которой будет отрисован граф
 */
void Graph::Paint(uint32_t edgeColor, uint32_t vertexColor)
{
	/*
	 * графическое представление графа:
	 * 1. вершины располагаются по кругу в том порядке, в котором они связаны ребрами (A -> E -> F -> B ... )
	 * 2. каждая вершина имеет подпись в виде заглавной латинской буквы
	 * 3. вершины связаны ребрами
	 * 4. каждое ребро имеет стоимость прохода по ней
	 */
	
	for (auto & vertex: vertexes) { vertex.Paint(QPen(vertexColor), QBrush(vertexColor)); }
	for (auto & edge: edges) { edge.Paint(QPen(QBrush(edgeColor), 2)); }
}

/// сокрытие ребер графа с графической сцены
void Graph::HideEdges()
{
	for (auto & edge: edges) { edge.Hide(); }
}

/**
 * @drief поиск ребра графа по меткам его вершин
 * @details выполняется поиск ребра (labelFrom, labelTo) или (labelTo, labelFrom)
 * @return указатель на найденное ребро или nullptr, если ребро не найдено в графе
 */
Edge * Graph::FindEdge(char labelFrom, char labelTo) noexcept
{
	Edge * e{nullptr};
	for (auto & edge: edges)
	{
		if ((edge.from->label == labelFrom && edge.to->label == labelTo) ||
		    (edge.from->label == labelTo && edge.to->label == labelFrom))
		{
			e = &edge;
			break;
		}
	}
	
	return e;
}

/// поиск ребра в графе по буквенным меткам ребра edge
Edge * Graph::FindEdge(const Edge & edge) noexcept
{
	return FindEdge(edge.from->label, edge.to->label);
}

/** **************************************************** PRIVATE *************************************************** **/

/**
 * @brief поиск вершины по ее буквенной метке
 * @param label буквенная метка
 * @return указатель на найденную вершину или nullptr, если вершина не найдена в графе
 */
Vertex * Graph::findVertex(char label) noexcept
{
	Vertex * v{nullptr};
	for (auto & vertex: vertexes)
	{
		if (vertex.label == label)
		{
			v = &vertex;
			break;
		}
	}
	
	return v;
}



