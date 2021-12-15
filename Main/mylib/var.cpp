#include "var.h"
#include "number.h"
#include "functor.h"
#include "array.h"

const var var::undefined;

var::var(std::initializer_list<var> list) : VarT(createInternal<ArrayImpl>(list))
{

}

var var::operator()(void)
{
    if (auto *f = toImpl<IFunctorImpl>() )
    {
        return (*f)();
    }
    return (*this);
}

namespace
{
    var add(var p1, var p2) { return p1() + p2(); }
    var sub(var p1, var p2) { return p1() - p2(); }
    var multiply(var p1, var p2) { return p1() * p2(); }
    var divide(var p1, var p2) { return p1() / p2(); }
}

var var::operator+(var other) const
{
    if (isUndefined() || other.isUndefined())
        return undefined;
    // number +
	NumberImpl *numLeft = toImpl<NumberImpl>();
    NumberImpl *numRight = other.toImpl<NumberImpl>();
    if (numLeft && numRight)
    {
        return (*numLeft) + (*numRight);
    }
    // functor
    if (toImpl<IFunctorImpl>() || other.toImpl<IFunctorImpl>())
    {
        return Functor(add, *this, other);
    }
    //
    throw undefined;
}
var var::operator-(var other) const
{
    if (isUndefined() || other.isUndefined())
        return undefined;
    // number +
    NumberImpl *numLeft = toImpl<NumberImpl>();
    NumberImpl *numRight = other.toImpl<NumberImpl>();
    if (numLeft && numRight)
    {
        return (*numLeft) - (*numRight);
    }
    // functor
    if (toImpl<IFunctorImpl>() || other.toImpl<IFunctorImpl>())
    {
        return Functor(sub, *this, other);
    }
    //
    throw undefined;
}
var var::operator*(var other) const
{
    if (isUndefined() || other.isUndefined())
        return undefined;
    // number +
    NumberImpl *numLeft = toImpl<NumberImpl>();
    NumberImpl *numRight = other.toImpl<NumberImpl>();
    if (numLeft && numRight)
    {
        return (*numLeft) * (*numRight);
    }
    // functor
	if (toImpl<IFunctorImpl>() || other.toImpl<IFunctorImpl>())
    {
        return Functor(multiply, *this, other);
    }
    //
    throw undefined;
}
var var::operator/(var other) const
{
    if (isUndefined() || other.isUndefined())
        return undefined;
    // number +
    NumberImpl *numLeft = toImpl<NumberImpl>();
    NumberImpl *numRight = other.toImpl<NumberImpl>();
    if (numLeft && numRight)
    {
        return (*numLeft) / (*numRight);
    }
    // functor
	if (toImpl<IFunctorImpl>() || other.toImpl<IFunctorImpl>())
    {
        return Functor(divide, *this, other);
    }
    //
    throw undefined;
}
