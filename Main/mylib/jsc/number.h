#pragma once

#include "impl.h"
#include "var.h"
//
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
    virtual NumberImpl *toNumberImpl() { return this; }
    inline static NumberImpl *cast(IImpl *impl) { return impl ? impl->toNumberImpl() : nullptr; }
	std::wstring toString() const { return L""; }

protected:
	Number operator+(const NumberImpl &other) const;
	Number operator-(const NumberImpl &other) const;
	Number operator*(const NumberImpl &other) const;
	Number operator/(const NumberImpl &other) const;
};


class Number : public var
{
public:
	Number(int x):var(createInternal<NumberImpl>(x)) {}
	Number(const double& x) : var(createInternal<NumberImpl>(x)){}
	Number(const var& v):var(v, v.getImpl<NumberImpl>()) {}

	Number operator+(Number other) const;
	Number operator-(Number other) const;
	Number operator*(Number other) const;
	Number operator/(Number other) const;
};
