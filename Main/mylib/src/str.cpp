#include <jsc/str.h>
#include "strImpl.h"
#include <map>
#include <list>
#include <algorithm>
#include <string>
#include <sstream>

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
			h = h ^ str[i];
		}
		return h;
	}
};
StringImplPool strPool;

String StrImpl::toString() const
{
	StringStream ss;
	ss << "\"" << str << "\"";
	return ss.str();
}

///////
const String String::UndefinedStr(_A("undefined"));

String::String(const var &a) : var(a, a.getImpl<StrImpl>())
{
}

String::String(const TStr &s) : var(strPool.GetOrCreate(s))
{
}

String::String(const std::string& s) 
: var(strPool.GetOrCreate(TStr::fromUTF8(s)))
{
}

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

////////////
StringStream& StringStream::operator<<(const String& s)
{
	StrImpl* sInternal = s.getImpl<StrImpl>();
	if(sInternal!=nullptr)
	{
		ss << sInternal->str;
	}
	return *this;
}
StringStream& StringStream::operator<<(const char s[])
{
	ss << s;
	return *this;
}

String StringStream::str()const
{
	auto tStr = TStr::fromUTF8(icu::StringPiece(ss.str()));
	return String(tStr);
}