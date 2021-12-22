#include <jsc/str.h>
#include "strImpl.h"
#include <map>
#include <list>
#include <algorithm>
#include <string>

inline TStr TStr::from(const std::u32string &s)
{
	return fromUTF32((const UChar32 *)s.c_str(), s.length());
}
/////
class StringImplPool
{
public:
	std::map<size_t, std::list<StrImpl::WeakPtr>> buckets;

	StrImpl::SharedPtr GetOrCreate(const TStr &str)
	{
		size_t strHash = hash(str);
		auto &bucket = buckets[strHash];
		for (auto iter = bucket.begin(); iter != bucket.end();)
		{
			if (auto s = (*iter).lock())
			{
				if (s->equal(str))
					return s;
				++iter;
			}
			else
			{
				iter = bucket.erase(iter);
			}
		}

		auto ptr = std::make_shared<StrImpl>(str);
		bucket.push_back(ptr);
		return ptr;
	}
	static size_t hash(const TStr &str)
	{
		size_t h = 0;
		size_t step = str.length() / 9 + 1;
		for (size_t i = 0; i < str.length(); i += step)
		{
			h = h ^ (size_t)(str[i]);
		}
		return h;
	}
};
StringImplPool strPool;

String StrImpl::toString() const
{
	StringStream ss;
	ss << _U("\"") << str << _U("\"");
	return ss.str();
}

const String String::UndefinedStr(_U("undefined"));
const String String::EmptyStr(_U(""));

String::String(const var &a) : var(a, a.getImpl<StrImpl>())
{
}

String::String(const TStr &s) : var(strPool.GetOrCreate(s))
{
}

String::String(const std::string& s)
: String(std::u32string(s.begin(), s.end()))
{
}

String::String(const std::u32string &s)
	: var(strPool.GetOrCreate(TStr::from(s)))
{
}

String String::operator+(const String &other) const
{
	if (isUndefined() || other.isUndefined())
		return undefined;
	StrImpl *p1 = getImpl<StrImpl>();
	StrImpl *p2 = other.getImpl<StrImpl>();
	return String(p1->str + p2->str);
}

const TStr &String::str() const
{
	StrImpl *p = getImpl<StrImpl>();
	return (p != nullptr) ? p->str : UndefinedStr.str();
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