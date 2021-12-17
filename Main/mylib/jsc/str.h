#pragma once

#include <string>
#include "var.h"

class String : public var
{
public:
	String(const var &a);
	String(const TStr& s);
	String operator+(const String &other) const;
	const static String UndefindStr;
	const TStr& cStr()const;
};
