#pragma once

class NumberImpl;
class IFunctorImpl;
class StrImpl;
class ArrayImpl;
class ObjectImpl;

#include <string>
#include <sstream>
#include <locale>
typedef std::u32string TStr;
typedef std::basic_stringstream<TStr::value_type> TStrStream;
#define _TS(x) U##x
typedef std::basic_ostream<TStr::value_type> TOstream;

// #include <unicode/unistr.h>
// #include <unicode/ustream.h>
// typedef icu::UnicodeString TStr;
// typedef std::ostream TOstream;
// #define _TS(x) icu::UnicodeString(x)
// typedef std::stringstream TStrStream;

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