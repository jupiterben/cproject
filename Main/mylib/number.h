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
	inline static NumberImpl* cast(IImpl* impl) { return impl ? impl->toNumber() : nullptr; }
protected:
    var operator+(const NumberImpl &other) const;
    var operator-(const NumberImpl &other) const;
    var operator*(const NumberImpl &other) const;
    var operator/(const NumberImpl &other) const;
};


class Number : public VarT<NumberImpl>
{
public:
	Number(int x):VarT(createInternal<NumberImpl>(x)) {}
	Number(const double& x) : VarT(createInternal<NumberImpl>(x)){}
	Number(const var& v):VarT(v) {}
};
