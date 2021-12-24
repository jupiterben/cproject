
#include <jsc/var.h>
#include <jsc/number.h>
#include "numberImpl.h"

Number NumberImpl::operator+(const NumberImpl &other) const
{
	return Number(internalData + other.internalData);
}

Number NumberImpl::operator-(const NumberImpl &other) const
{
	return Number(internalData - other.internalData);
}

Number NumberImpl::operator*(const NumberImpl &other) const
{
	return Number(internalData * other.internalData);
}

Number NumberImpl::operator/(const NumberImpl &other) const
{
	return Number(internalData / other.internalData);
}

TValueImplPool<NumberImpl> NumberImplPool;

//
Number::Number(int x) : var(NumberImplPool.GetOrCreate(x)) {}
Number::Number(const double &x) : var(NumberImplPool.GetOrCreate(x)) {}
Number::Number(const var &v) : var(v, v.getImpl<NumberImpl>()) {}
Number Number::operator+(Number other) const
{
	if (isUndefined() || other.isUndefined())
		return undefined;
	return (*getImpl<NumberImpl>()) + (*other.getImpl<NumberImpl>());
}
Number Number::operator-(Number other) const
{
	if (isUndefined() || other.isUndefined())
		return undefined;
	return (*getImpl<NumberImpl>()) - (*other.getImpl<NumberImpl>());
}
Number Number::operator*(Number other) const
{
	if (isUndefined() || other.isUndefined())
		return undefined;
	return (*getImpl<NumberImpl>()) * (*other.getImpl<NumberImpl>());
}
Number Number::operator/(Number other) const
{
	if (isUndefined() || other.isUndefined())
		return undefined;
	return (*getImpl<NumberImpl>()) / (*other.getImpl<NumberImpl>());
}