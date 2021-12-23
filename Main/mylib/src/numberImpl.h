#pragma once
#include <jsc/impl.h>
#include "strImpl.h"
#include <sstream>
#include <jsc/str.h>

class Number;
class NumberImpl : public IValue
{
    friend class Number;
public:
    typedef double InternalType;
private:
    const InternalType x;

public:
    inline NumberImpl(double _x) : x(_x) {}
    inline operator double() const { return x; }

public:
    String toString() const
    {
		std::stringstream ss;
        ss << x;
        return String(ss.str());
    }

    size_t getHash()const
    {
        return (size_t)(x);
    }

protected:
    Number operator+(const NumberImpl &other) const;
    Number operator-(const NumberImpl &other) const;
    Number operator*(const NumberImpl &other) const;
    Number operator/(const NumberImpl &other) const;
};
