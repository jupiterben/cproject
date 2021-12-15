
#include "var.h"
#include "number.h"

var NumberImpl::operator+(const NumberImpl &other) const
{
    return var::createInternal<NumberImpl>(x + other.x);
}

var NumberImpl::operator-(const NumberImpl &other) const
{
    return var::createInternal<NumberImpl>(x - other.x);
}

var NumberImpl::operator*(const NumberImpl &other) const
{
    return var::createInternal<NumberImpl>(x * other.x);
}

var NumberImpl::operator/(const NumberImpl &other) const
{
    return var::createInternal<NumberImpl>(x / other.x);
}
