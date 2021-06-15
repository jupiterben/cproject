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
        return fmod(degree + 360,   360);
    }

	static double toDegrees(double rad) {
		return rad * 180 / Math2d::pi();
	}
};

class HFHalfEdge;
class HFVertex;

class HFVertex : public Point2d
{
public:
public:
    HFVertex(double _x, double _y) : Point2d(_x, _y) {}
};

class HFLoop
{
public:
    std::vector<HFHalfEdge *> edges;
};

class HFHalfEdge
{
public:
    HFVertex *from;
    HFVertex *to;

    HFHalfEdge *next;
    HFHalfEdge *partner;
    HFLoop *loop;

    HFHalfEdge(HFVertex *f, HFVertex *t)
        : from(f), to(t), next(nullptr), partner(nullptr), loop(nullptr)
    {
    }
};

class LoopBuilder
{
protected:
    double tolerance;

public:
    LoopBuilder(double tol)
    {
        tolerance = tol;
    }

    void addLine(const Point2d &p1, const Point2d &p2)
    {
        HFVertex *v1 = _appendVertex(p1.x, p1.y);
        HFVertex *v2 = _appendVertex(p2.x, p2.y);
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
			auto sorter = [](const OutEdge& e1,  const OutEdge& e2)->bool { return e1.second < e2.second; };
			std::sort(outHalfEdges.begin(), outHalfEdges.end(), sorter);
			
			OutEdgeList sortedEdges = outHalfEdges;
			sortedEdges.push_back(outHalfEdges[0]);
			for (int i = 0; i < sortedEdges.size()-1; ++i)
			{
				HFHalfEdge* pre = sortedEdges[i].first;
				HFHalfEdge* next = sortedEdges[i+1].first;
				next->partner->next = pre;
			}
		}
		for (auto iter = edges.begin(); iter != edges.end(); ++iter)
		{
			findEdgeLoop(iter->get());
		}
	}

	HFLoop* findEdgeLoop(HFHalfEdge* start) {
		if (start->loop) return start->loop;
		HFLoop* loop = new HFLoop();
		loops.push_back(std::unique_ptr<HFLoop>(loop));
		HFHalfEdge* iter = start;
		do 
		{
			if (iter->loop) break;
			iter->loop = loop;
			loop->edges.push_back(iter);
			iter = iter->next;
		} while (iter && iter!= start);
		return start->loop;
	}

	public:
    std::vector<std::unique_ptr<HFVertex>> vertices;
    std::vector<std::unique_ptr<HFHalfEdge>> edges;
    std::vector<std::unique_ptr<HFLoop>> loops;
protected:
	typedef std::pair<HFHalfEdge*, double> OutEdge;
	typedef std::vector<OutEdge> OutEdgeList;
    std::map<HFVertex *, OutEdgeList> vertexOutHalfEdges;

private:
    HFVertex *_appendVertex(double x, double y)
    {
        for (auto itr = vertices.begin(); itr != vertices.end(); ++itr)
        {
            if (IsSamePoint(itr->get(), x, y))
                return itr->get();
        }
        HFVertex *ret = new HFVertex(x, y);
        vertices.push_back(std::unique_ptr<HFVertex>(ret));
        return ret;
    }
    bool IsSamePoint(HFVertex *vertex, double x, double y)
    {
        double diffX = vertex->x - x;
        double diffY = vertex->y - y;
        return sqrt(diffX * diffX + diffY * diffY) < tolerance;
    }

    HFHalfEdge *findEdge(HFVertex *v1, HFVertex *v2)
    {
        for (auto iter = edges.begin(); iter != edges.end(); ++iter)
        {
            HFHalfEdge *edge = (*iter).get();
            if (edge->from == v1 && edge->to == v2)
                return edge;
        }
        return nullptr;
    }

	HFHalfEdge* _appendHalfEdge(HFVertex *from, HFVertex *to)
    {
		HFHalfEdge* edge = new HFHalfEdge(from, to);
		auto iter = vertexOutHalfEdges.find(from);
		if (iter == vertexOutHalfEdges.end()) {
			vertexOutHalfEdges[from] = OutEdgeList({std::make_pair(edge, 0)});
		}
		else
		{
			HFHalfEdge* base = iter->second[0].first;
			double turnAngle = Math2d::LinelineCCWAngle(*from, *base->to, *to);
			iter->second.push_back(std::make_pair(edge, turnAngle));
		}
		edges.push_back(std::unique_ptr<HFHalfEdge>(edge));
		return edge;
    }
};
