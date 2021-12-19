#pragma once
#include <jsc/impl.h>
#include "strImpl.h"
#include <sstream>
#include <jsc/str.h>

class Number;
class NumberImpl : public IImpl
{
    friend class Number;

private:
    const double x;

public:
    inline NumberImpl(double _x) : x(_x) {}
    inline operator double() const { return x; }

public:
    static String typeId;

    String toString() const
    {
		std::stringstream ss;
        ss << x;
        return String(ss.str());
    }

protected:
    Number operator+(const NumberImpl &other) const;
    Number operator-(const NumberImpl &other) const;
    Number operator*(const NumberImpl &other) const;
    Number operator/(const NumberImpl &other) const;
};
