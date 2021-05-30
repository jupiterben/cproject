#pragma once
#include "../maxHeaders.h"
#include "./json.h"

template <class T>
json Format(const T &value)
{
	return value;
}

json Format(const Point3 &value)
{
	return {value.x, value.y, value.z};
}

json Format(const Color &value)
{
	return {value.r, value.g, value.b};
}

json Format(const AngAxis &value)
{
	return {value.axis.x, value.axis.y, value.axis.z, value.angle};
}

json Format(const Quat &value)
{
	return {value.x, value.y, value.z, value.w};
}

json Format(const Matrix3 &value)
{
	return { Format(value.GetRow(0)), Format(value.GetRow(1)), Format(value.GetRow(2)), Format(value.GetRow(3))};
}

json Format(const ScaleValue &value)
{
	json dump(json::value_t::object);
	dump[_T("s")] = Format(value.s);
	dump[_T("q")] = Format(value.q);
	return dump;
}


#define JSON(x) Format(x)