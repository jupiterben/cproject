#include <jsc/str.h>
#include "strImpl.h"
#include "valueImplPool.h"

inline TStr TStr::from(const std::u32string &s)
{
	return fromUTF32((const UChar32 *)s.c_str(), s.length());
}

TValueImplPool<StrImpl> StrImplPool;

String StrImpl::toString() const
{
	StringStream ss;
	ss << _U("\"") << internalData << _U("\"");
	return ss.str();
}

const String String::UndefinedStr(_U("undefined"));
const String String::EmptyStr(_U(""));

String::String(const var &a) : var(a, a.getImpl<StrImpl>())
{
}

String::String(const TStr &s) : var(StrImplPool.GetOrCreate(s))
{
}

String::String(const std::string& s)
: String(std::u32string(s.begin(), s.end()))
{
}

String::String(const std::u32string &s)
	: var(StrImplPool.GetOrCreate(TStr::from(s)))
{
}

String String::operator+(const String &other) const
{
	if (isUndefined() || other.isUndefined())
		return undefined;
	StrImpl *p1 = getImpl<StrImpl>();
	StrImpl *p2 = other.getImpl<StrImpl>();
	return String(p1->internalData + p2->internalData);
}

const TStr &String::str() const
{
	StrImpl *p = getImpl<StrImpl>();
	return (p != nullptr) ? p->internalData : UndefinedStr.str();
}

///
StringStream::StringStream() : var(createInternal<StringStreamImpl>())
{
}
StringStream &StringStream::operator<<(const std::u32string &s)
{
	return (*this) << TStr::from(s);
}
StringStream &StringStream::operator<<(const String &s)
{
	StringStreamImpl* impl = getImpl<StringStreamImpl>();
	(*impl) << s.str();
	return *this;
}
String StringStream::str() const
{
	StringStreamImpl* impl = getImpl<StringStreamImpl>();
	return impl->str();
}