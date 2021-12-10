
#pragma once

#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <algorithm>

#define EPSILON 0.000001

struct Point2d
{
	double x;
	double y;
	Point2d() : x(0), y(0) {}
	Point2d(double _x, double _y) : x(_x), y(_y) {}
	Point2d operator-(const Point2d &other) const { return Point2d(x - other.x, y - other.y); }
	Point2d operator+(const Point2d &other) const { return Point2d(x + other.x, y + other.y); }
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
	static double distance(const Point2d &v1, const Point2d &v2)
	{
		auto diff = sub(v1, v2);
		return sqrt(dot(diff, diff));
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
	static double Area(const std::vector<Point2d> &polygon)
	{
		double sum = 0;
		for (size_t j = polygon.size() - 1, i = 0; i < polygon.size(); j = i++)
		{
			const auto &p1 = polygon[j];
			const auto &p2 = polygon[i];
			sum += (p1.x * p2.y - p1.y * p2.x);
		}
		return sum;
	}

	static double toDegrees(double rad)
	{
		return rad * 180 / Math2d::pi();
	}

	static bool lineLineIntersect(const Point2d &p1, const Point2d &p2, const Point2d &p3, const Point2d &p4, Point2d &out)
	{
		double det = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
		if ( abs(det) < EPSILON)
			return false;
		double invDet = 1.0 / det;
		double _tmp1 = (p1.x * p2.y - p1.y * p2.x), _tmp2 = (p3.x * p4.y - p3.y * p4.x);
		double _x = _tmp1 * (p3.x - p4.x) - (p1.x - p2.x) * _tmp2;
		double _y = _tmp1 * (p3.y - p4.y) - (p1.y - p2.y) * _tmp2;
		out.x = _x * invDet;
		out.y = _y * invDet;
		return true;
	}
};

struct Line2d
{
	Point2d start;
	Point2d end;
	Line2d(const Point2d &s, const Point2d &e) : start(s), end(e) {}
	double length() const { return Math2d::distance(start, end); }
	double getDistOnLine(const Point2d &p) const
	{
		return Math2d::dot(p - start, end - start) / length();
	}
};


class ProfileBuilder
{
public:
	std::vector<Path> build(const std::vector<Line2d>& lines, double tol);
protected:
	typedef std::pair<Point2d, double> PosOnLine;
	typedef std::vector<PosOnLine> PosOnLineList;
	typedef std::map<int, PosOnLineList> LineIntersectionMap;
	void getLinesIntersections(const std::vector<Line2d> &curves, LineIntersectionMap &cmap, double tol);
};