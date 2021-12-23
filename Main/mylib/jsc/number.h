#pragma once

#include "var.h"
//
class Number : public var
{
public:
	Number(int x);
	Number(const double& x) ;
	Number(const var& v);

	Number operator+(Number other) const;
	Number operator-(Number other) const;
	Number operator*(Number other) const;
	Number operator/(Number other) const;
};
