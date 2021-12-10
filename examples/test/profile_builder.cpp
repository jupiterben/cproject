

#include "profile_builder.h"
#include <algorithm>
/**
 *
 */
class HFEdge;
class HFVertex;
class HFLoop;

class HFVertex : public Point2d
{
public:
	HFVertex(const Point2d &pos) : Point2d(pos.x, pos.y) {}
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
	Path GetPath() const
	{
		Path path;
		for (auto edge : edges)
		{
			path.push_back(Point2d(edge->from->x, edge->from->y));
		}
		return path;
	}

	std::vector<HFEdge*> SimplifyEdges()const
	{
		std::vector<HFEdge *> ret;
		for (auto e : edges)
		{
			if(std::find(edges.begin(),edges.end(),e->partner)==edges.end())
			{
				ret.push_back(e);
			}
		}
		return ret;
	}

	Path GetSimplifyPath()const
	{
		Path path;
		for (auto edge: SimplifyEdges())
		{
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
		if (v1 == v2)
			return;
		if (findEdge(v1, v2))
			return;
		auto e1 = _appendHalfEdge(v1, v2);
		auto e2 = _appendHalfEdge(v2, v1);
		e1->partner = e2;
		e2->partner = e1;
	}

	void build()
	{
		for (auto iter = vertexOutHalfEdges.begin(); iter != vertexOutHalfEdges.end(); ++iter)
		{
			OutEdgeList &outHalfEdges = iter->second;
			if (outHalfEdges.empty())
				continue;
			auto sorter = [](const OutEdge &e1, const OutEdge &e2) -> bool
			{ return e1.second < e2.second; };
			std::sort(outHalfEdges.begin(), outHalfEdges.end(), sorter);

			for (size_t j = outHalfEdges.size() - 1, i = 0; i < outHalfEdges.size(); j = i++)
			{
				HFEdge *pre = outHalfEdges[j].first;
				HFEdge *next = outHalfEdges[i].first;
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
			const HFLoop *loop = iter->get();
			auto path = loop->GetSimplifyPath();
			if (Math2d::Area(path) > 0)
			{
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
	typedef std::pair<HFEdge *, double> OutEdge;
	typedef std::vector<OutEdge> OutEdgeList;
	std::map<HFVertex *, OutEdgeList> vertexOutHalfEdges;

private:
	HFVertex *_appendVertex(const Point2d &pos)
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
	bool IsSamePoint(HFVertex *vertex, const Point2d &pos)
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

	HFEdge *_appendHalfEdge(HFVertex *from, HFVertex *to)
	{
		HFEdge *edge = new HFEdge(from, to);
		auto iter = vertexOutHalfEdges.find(from);
		if (iter == vertexOutHalfEdges.end())
		{
			vertexOutHalfEdges[from] = OutEdgeList({ std::make_pair(edge, 0) });
		}
		else
		{
			HFEdge *base = iter->second[0].first;
			double turnAngle = Math2d::LinelineCCWAngle(*from, *base->to, *to);
			iter->second.push_back(std::make_pair(edge, turnAngle));
		}
		edges.push_back(std::unique_ptr<HFEdge>(edge));
		return edge;
	}

	HFLoop *_findEdgeLoop(HFEdge *start)
	{
		if (start->loop)
			return start->loop;
		HFLoop *loop = new HFLoop();
		loops.push_back(std::unique_ptr<HFLoop>(loop));
		HFEdge *iter = start;
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

/**
 *
 */
std::vector<Path> ProfileBuilder::build(const std::vector<Line2d>& lines, double tol)
{
	LoopBuilder lpb(tol);
	//���߶ηֶμ���LoopBuilder
	LineIntersectionMap intersectMap;
	getLinesIntersections(lines, intersectMap, tol);
	for (int i = 0; i < lines.size(); ++i)
	{
		const Line2d& line = lines[i];
		auto& intersects = intersectMap[i];

		std::sort(intersects.begin(), intersects.end(), [](const PosOnLine& pos1, const PosOnLine& pos2) { return pos1.second < pos2.second;  });
		std::vector<Point2d> pts;
		double lastDist = 0;
		pts.push_back(line.start);
		for (auto iter = intersects.begin(); iter != intersects.end(); ++iter)
		{
			if (iter->second - lastDist < tol) continue;
			lastDist = iter->second;
			pts.push_back(iter->first);
		}
		pts.push_back(line.end);
		//
		for (int i = 0; i < pts.size() - 1; ++i)
		{
			lpb.addLine(pts[i], pts[i + 1]);
		}
	}

	lpb.build();
	return lpb.getInnerPaths();
}

void ProfileBuilder::getLinesIntersections(const std::vector<Line2d> &curves, LineIntersectionMap &cmap, double tol)
{
	auto addCurveIntersect = [&cmap](int index, const Point2d &pos, double pos1d)
	{
		auto findIter = cmap.find(index);
		if (findIter != cmap.end())
		{
			findIter->second.push_back(std::make_pair(pos, pos1d));
		}
		else
		{
			cmap[index] = PosOnLineList({ std::make_pair(pos, pos1d) });
		}
	};

	size_t nCount = curves.size();
	for (int i = 0; i < nCount - 1; ++i)
	{
		auto c1 = curves[i];
		double c1Length = c1.length();
		if (c1Length < EPSILON)
			continue;
		for (int j = i + 1; j < nCount; ++j)
		{
			auto c2 = curves[j];
			double c2Length = c2.length();
			if (c2.length() < EPSILON)
				continue;

			Point2d intPt;
			if (Math2d::lineLineIntersect(c1.start, c1.end, c2.start, c2.end, intPt))
			{
				double pos1 = c1.getDistOnLine(intPt);
				double pos2 = c2.getDistOnLine(intPt);
				if (pos1 > 0 && pos1 < c1Length && pos2 > -tol && pos2 < c2Length + tol)
				{
					addCurveIntersect(i, intPt, pos1);
				}
				if (pos2 > 0 && pos2 < c2Length && pos1 > -tol && pos1 < c1Length + tol)
				{
					addCurveIntersect(j, intPt, pos2);
				}
			}
		}
	}
}
