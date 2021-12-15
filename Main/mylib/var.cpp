#include "var.h"
#include "number.h"
#include "functor.h"

const var var::undefined;

var::var(const double& x) : VarT(createInternal<NumberImpl>(x))
{

}

var var::operator()(void)
{
    if (IFunctorImpl *f = toFunctor())
    {
        return (*f)();
    }
    return (*this);
}

namespace
{
    var add(var p1, var p2) { return p1() + p2(); }
    var sub(var p1, var p2) { return p1() - p2(); }
    var multi(var p1, var p2) { return p1() * p2(); }
    var div(var p1, var p2) { return p1() / p2(); }
}

var var::operator+(var other) const
{
    if (isUndefined() || other.isUndefined())
        return undefined;
    // number +
    NumberImpl *numLeft = toNumber();
    NumberImpl *numRight = other.toNumber();
    if (numLeft && numRight)
    {
        return (*numLeft) + (*numRight);
    }
    // functor
    if (toFunctor() || other.toFunctor())
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
    NumberImpl *numLeft = toNumber();
    NumberImpl *numRight = other.toNumber();
    if (numLeft && numRight)
    {
        return (*numLeft) - (*numRight);
    }
    // functor
    if (toFunctor() || other.toFunctor())
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
    NumberImpl *numLeft = toNumber();
    NumberImpl *numRight = other.toNumber();
    if (numLeft && numRight)
    {
        return (*numLeft) * (*numRight);
    }
    // functor
    if (toFunctor() || other.toFunctor())
    {
        return multi(*this, other);
    }
    //
    throw undefined;
}
var var::operator/(var other) const
{
    if (isUndefined() || other.isUndefined())
        return undefined;
    // number +
    NumberImpl *numLeft = toNumber();
    NumberImpl *numRight = other.toNumber();
    if (numLeft && numRight)
    {
        return (*numLeft) / (*numRight);
    }
    // functor
    if (toFunctor() || other.toFunctor())
    {
        return div(*this, other);
    }
    //
    throw undefined;
}
