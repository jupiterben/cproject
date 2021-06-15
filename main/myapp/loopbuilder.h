#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <algorithm>

struct Point2d
{
	double x;
	double y;
	Point2d(double _x, double _y) : x(_x), y(_y) {}
};
typedef std::vector<Point2d> Path;

class Math2d
{
public:
	static double dot(const Point2d &v1, const Point2d &v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	static double cross(const Point2d &v1, const Point2d &v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}
	static Point2d sub(const Point2d &v1, const Point2d &v2)
	{
		return Point2d(v1.x - v2.x, v1.y - v2.y);
	}
	static double pi() { return std::atan(1) * 4; }
	static double LinelineCCWAngle(const Point2d &base, const Point2d &p1, const Point2d &p2)
	{
		auto v1 = Math2d::sub(p1, base);
		auto v2 = Math2d::sub(p2, base);
		auto dot = Math2d::dot(v1, v2);
		auto cross = Math2d::cross(v1, v2);
		double degree = Math2d::toDegrees(atan2(cross, dot));
		return fmod(degree + 360, 360);
	}
	static double Area(const std::vector<Point2d>& polygon)
	{
		double sum = 0;
		for (size_t j = polygon.size() - 1, i = 0; i < polygon.size(); j = i++) {
			const auto& p1 = polygon[j];
			const auto& p2 = polygon[i];
			sum += (p1.x * p2.y - p1.y * p2.x);
		}
		return sum;
	}

	static double toDegrees(double rad) {
		return rad * 180 / Math2d::pi();
	}

	static bool lineLineIntersect(const Point2d& p1, const Point2d& p2, const Point2d& p3, const Point2d& p4, Point2d& out)
	{
		double det = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
		if (det < 0.00000001) return false;
		double invDet = 1.0 / det;
		double _tmp1 = (p1.x * p2.y - p1.y * p2.x), _tmp2 = (p3.x * p4.y - p3.y * p4.x);
		double _x = _tmp1 * (p3.x - p4.x) - (p1.x - p2.x) * _tmp2;
		double _y = _tmp1 * (p3.y - p4.y) - (p1.y - p2.y) * _tmp2;
		out.x = _x * invDet;
		out.y = _y * invDet;
	}
};

class HFEdge;
class HFVertex;
class HFLoop;

class HFVertex : public Point2d
{
public:
	HFVertex(const Point2d& pos) : Point2d(pos.x, pos.y) {}
};
class HFEdge
{
public:
	HFVertex *from;
	HFVertex *to;

	HFEdge *next;
	HFEdge *partner;
	HFLoop *loop;

	HFEdge(HFVertex *f, HFVertex *t)
		: from(f), to(t), next(nullptr), partner(nullptr), loop(nullptr)
	{
	}
};

class HFLoop
{
	friend class LoopBuilder;
public:
	Path GetPath()const
	{
		Path path;
		for (size_t i = 0; i < edges.size(); ++i)
		{
			const HFEdge* edge = edges[i];
			path.push_back(Point2d(edge->from->x, edge->from->y));
		}
		return path;
	}
protected:
	std::vector<HFEdge *> edges;
};

class LoopBuilder
{
protected:
	double tolerance;

public:
	LoopBuilder(double tol = 0.0001)
	{
		tolerance = tol;
	}

	void addLine(const Point2d &p1, const Point2d &p2)
	{
		HFVertex *v1 = _appendVertex(p1);
		HFVertex *v2 = _appendVertex(p2);
		if (v1 == v2) return;
		if (findEdge(v1, v2))
			return;
		auto e1 = _appendHalfEdge(v1, v2);
		auto e2 = _appendHalfEdge(v2, v1);
		e1->partner = e2;
		e2->partner = e1;
	}

	void build() {
		for (auto iter = vertexOutHalfEdges.begin(); iter != vertexOutHalfEdges.end(); ++iter)
		{
			OutEdgeList& outHalfEdges = iter->second;
			if (outHalfEdges.empty())continue;
			auto sorter = [](const OutEdge& e1, const OutEdge& e2)->bool { return e1.second < e2.second; };
			std::sort(outHalfEdges.begin(), outHalfEdges.end(), sorter);

			for (size_t j = outHalfEdges.size() - 1, i = 0; i < outHalfEdges.size(); j = i++)
			{
				HFEdge* pre = outHalfEdges[j].first;
				HFEdge* next = outHalfEdges[i].first;
				next->partner->next = pre;
			}
		}
		for (auto iter = edges.begin(); iter != edges.end(); ++iter)
		{
			_findEdgeLoop(iter->get());
		}
	}

	std::vector<Path> getInnerPaths()
	{
		std::vector<Path> ccwPath;
		for (auto iter = loops.begin(); iter != loops.end(); ++iter)
		{
			const HFLoop* loop = iter->get();
			auto path = loop->GetPath();
			if (Math2d::Area(path) > 0) {
				ccwPath.push_back(path);
			}
		}
		return ccwPath;
	}

public:
	std::vector<std::unique_ptr<HFVertex>> vertices;
	std::vector<std::unique_ptr<HFEdge>> edges;
	std::vector<std::unique_ptr<HFLoop>> loops;
protected:
	typedef std::pair<HFEdge*, double> OutEdge;
	typedef std::vector<OutEdge> OutEdgeList;
	std::map<HFVertex *, OutEdgeList> vertexOutHalfEdges;

private:
	HFVertex *_appendVertex(const Point2d& pos)
	{
		for (auto itr = vertices.begin(); itr != vertices.end(); ++itr)
		{
			if (IsSamePoint(itr->get(), pos))
				return itr->get();
		}
		HFVertex *ret = new HFVertex(pos);
		vertices.push_back(std::unique_ptr<HFVertex>(ret));
		return ret;
	}
	bool IsSamePoint(HFVertex *vertex, const Point2d& pos)
	{
		double diffX = vertex->x - pos.x;
		double diffY = vertex->y - pos.y;
		return sqrt(diffX * diffX + diffY * diffY) < tolerance;
	}

	HFEdge *findEdge(HFVertex *v1, HFVertex *v2)
	{
		for (auto iter = edges.begin(); iter != edges.end(); ++iter)
		{
			HFEdge *edge = (*iter).get();
			if (edge->from == v1 && edge->to == v2)
				return edge;
		}
		return nullptr;
	}

	HFEdge* _appendHalfEdge(HFVertex *from, HFVertex *to)
	{
		HFEdge* edge = new HFEdge(from, to);
		auto iter = vertexOutHalfEdges.find(from);
		if (iter == vertexOutHalfEdges.end()) {
			vertexOutHalfEdges[from] = OutEdgeList({ std::make_pair(edge, 0) });
		}
		else
		{
			HFEdge* base = iter->second[0].first;
			double turnAngle = Math2d::LinelineCCWAngle(*from, *base->to, *to);
			iter->second.push_back(std::make_pair(edge, turnAngle));
		}
		edges.push_back(std::unique_ptr<HFEdge>(edge));
		return edge;
	}

	HFLoop* _findEdgeLoop(HFEdge* start) {
		if (start->loop) return start->loop;
		HFLoop* loop = new HFLoop();
		loops.push_back(std::unique_ptr<HFLoop>(loop));
		HFEdge* iter = start;
		do
		{
			if (iter->loop)
			{
				throw "should not happen!";
			}
			iter->loop = loop;
			loop->edges.push_back(iter);
			iter = iter->next;
		} while (iter && iter != start);
		return start->loop;
	}
};
