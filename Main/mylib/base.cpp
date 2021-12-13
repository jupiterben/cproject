#include "base.h"

const var var::undefined;

var var::operator()(void)
{
	if (IFunctorImpl* f = toFunctor())
	{
		return (*f)();
	}
	return (*this);
}

namespace
{
	functor2(add, p1, p2,
	{
		return p1() + p2();
	});

	functor2(sub, p1, p2,
	{
		return p1() - p2();
	});

	functor2(multi, p1, p2,
	{
		return p1() * p2();
	});

	functor2(div, p1, p2,
	{
		return p1() / p2();
	})
}

var var::operator+(var other)const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	// number + 
	NumberImpl* numLeft = toNumber();
	NumberImpl* numRight = other.toNumber();
	if (numLeft && numRight)
	{
		return (*numLeft) + (*numRight);
	}
	//functor
	if (toFunctor() || other.toFunctor())
	{
		return add(*this, other);
	}
	//
	throw undefined;
}
var var::operator-(var other)const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	// number + 
	NumberImpl* numLeft = toNumber();
	NumberImpl* numRight = other.toNumber();
	if (numLeft && numRight)
	{
		return (*numLeft) - (*numRight);
	}
	//functor
	if (toFunctor() || other.toFunctor())
	{
		return sub(*this, other);
	}
	//
	throw undefined;
}
var var::operator*(var other)const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	// number + 
	NumberImpl* numLeft = toNumber();
	NumberImpl* numRight = other.toNumber();
	if (numLeft && numRight)
	{
		return (*numLeft) * (*numRight);
	}
	//functor
	if (toFunctor() || other.toFunctor())
	{
		return multi(*this, other);
	}
	//
	throw undefined;
}
var var::operator/(var other)const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	// number + 
	NumberImpl* numLeft = toNumber();
	NumberImpl* numRight = other.toNumber();
	if (numLeft && numRight)
	{
		return (*numLeft)/(*numRight);
	}
	//functor
	if (toFunctor() || other.toFunctor())
	{
		return div(*this, other);
	}
	//
	throw undefined;
}

//////////////////////////////////////////////////////////////////////////
var NumberImpl::operator+(const NumberImpl&  other) const
{
	return var::createInternal<NumberImpl>(x + other.x);
}

var NumberImpl::operator-(const NumberImpl& other) const
{
	return var::createInternal<NumberImpl>(x - other.x);
}

var NumberImpl::operator*(const NumberImpl& other) const
{
	return var::createInternal<NumberImpl>(x * other.x);
}

var NumberImpl::operator/(const NumberImpl& other) const
{
	return var::createInternal<NumberImpl>(x / other.x);
}
