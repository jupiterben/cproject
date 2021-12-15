#pragma once

#include "impl.h"
//
class NumberImpl : public IImpl
{
    friend class var;

private:
    double x;

public:
    inline NumberImpl(double _x) : x(_x) {}
    inline operator double() const { return x; }
    virtual NumberImpl *toNumber() { return this; }

protected:
    var operator+(const NumberImpl &other) const;
    var operator-(const NumberImpl &other) const;
    var operator*(const NumberImpl &other) const;
    var operator/(const NumberImpl &other) const;
};
