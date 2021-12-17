#pragma once

#include <string>
#include <sstream>
class NumberImpl;
class IFunctorImpl;
class StrImpl;
class ArrayImpl;
class ObjectImpl;

typedef std::u32string TStr;
typedef std::basic_stringstream<TStr::value_type, TStr::traits_type, TStr::allocator_type> TStrStream;
#define _TS(x) U##x
typedef std::basic_ostream<TStr::value_type, TStr::traits_type> TOstream;

class IImpl
{
public:
	virtual IFunctorImpl *toFunctorImpl() { return nullptr; }
	virtual NumberImpl *toNumberImpl() { return nullptr; }
	virtual StrImpl * toStrImpl() { return nullptr; }
	virtual ArrayImpl* toArrayImpl() { return nullptr; }
	virtual ObjectImpl* toObjectImpl(){ return nullptr; }
	static IImpl* cast(IImpl* impl) { return impl; }
	//
	virtual TStr toString() const = 0;
};