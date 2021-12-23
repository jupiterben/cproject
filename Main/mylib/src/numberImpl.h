#pragma once
#include <jsc/impl.h>
#include "strImpl.h"
#include <sstream>
#include <jsc/str.h>

class Number;
class NumberImpl : public TValueImpl<double>
{
    friend class Number;
    using TValueImpl::TValueImpl;
public:
    inline operator double() const { return internalData; }
public:
    String toString() const
    {
		std::stringstream ss;
        ss << internalData;
        return String(ss.str());
    }
protected:
    Number operator+(const NumberImpl &other) const;
    Number operator-(const NumberImpl &other) const;
    Number operator*(const NumberImpl &other) const;
    Number operator/(const NumberImpl &other) const;
};
