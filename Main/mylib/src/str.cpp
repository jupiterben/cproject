#include <jsc/str.h>
#include "strImpl.h"
#include <map>
#include <list>
#include <algorithm>
#include <string>
#include <sstream>

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
	TStr ret = TStr::from(_U("\""));
	return ret + str + ret;
}

const String String::UndefinedStr(_U("undefined"));
const String String::EmptyStr(_U(""));

String::String(const var &a) : var(a, a.getImpl<StrImpl>())
{
}

String::String(const TStr &s) : var(strPool.GetOrCreate(s))
{
}

// String::String(const std::string& s) 
// : var(strPool.GetOrCreate(TStr::fromUTF8(s)))
// {
// }

String::String(const std::u32string& s) 
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

const TStr& String::str()const
{
	StrImpl* p = getImpl<StrImpl>();
	return (p!=nullptr)?p->str:UndefinedStr.str();
}

