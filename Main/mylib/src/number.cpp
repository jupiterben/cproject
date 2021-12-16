
#include <jsc/var.h>
#include <jsc/number.h>

Number NumberImpl::operator+(const NumberImpl &other) const
{
    return Number(x + other.x);
}

Number NumberImpl::operator-(const NumberImpl &other) const
{
    return Number(x - other.x);
}

Number NumberImpl::operator*(const NumberImpl &other) const
{
    return Number(x * other.x);
}

Number NumberImpl::operator/(const NumberImpl &other) const
{
    return Number(x / other.x);
}

//
Number Number::operator+(Number other) const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	return (*getImpl<NumberImpl>()) + (*other.getImpl<NumberImpl>());
}
Number Number::operator-(Number other) const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	return (*getImpl<NumberImpl>()) - (*other.getImpl<NumberImpl>());
}
Number Number::operator*(Number other) const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	return (*getImpl<NumberImpl>()) * (*other.getImpl<NumberImpl>());
}
Number Number::operator/(Number other) const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	return (*getImpl<NumberImpl>()) / (*other.getImpl<NumberImpl>());
}