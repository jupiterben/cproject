#include <jsc/var.h>
#include <jsc/number.h>
#include <jsc/functor.h>
#include <jsc/array.h>
#include <jsc/str.h>

#include "strImpl.h"

const var var::undefined;

var::var(double x) : var(Number(x))
{
}
var::var(const std::string&s ):var(String(s)){}
var::var(const std::u32string&s ):var(String(s)){}

var var::operator()(void)
{
	Functor f(*this);
	return f.isUndefined() ? (*this) : f();
}

String var::toString() const
{
	if (isUndefined())
		return String::UndefinedStr;
	else
		return getImpl<IImpl>()->toString();
}

