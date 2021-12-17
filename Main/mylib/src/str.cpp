#include <jsc/str.h>
#include "strImpl.h"
#include <map>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <string>
#include <locale.h>

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
		size_t step = str.size() / 9 + 1;
		for (size_t i = 0; i < str.size(); i += step)
		{
			h = h ^ str[i];
		}
		return h;
	}
};
StringImplPool strPool;

const String String::UndefindStr(_TS("undefined"));

String::String(const var &a) : var(a, a.getImpl<StrImpl>())
{
}

String::String(const TStr& s):var(strPool.GetOrCreate(s))
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

const TStr &String::cStr() const
{
	if (isUndefined())
	{
		return UndefindStr.cStr();
	}
	else
	{
		return getImpl<StrImpl>()->str;
	}
}