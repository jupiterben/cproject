#include <jsc/str.h>
#include <map>
#include <list>
#include <algorithm>
#include "strImpl.h"

std::wstring str2wstr(const std::string &str)
{
	unsigned len = (unsigned)str.size() * 2 + 10;
	setlocale(LC_CTYPE, "");
	wchar_t *p = new wchar_t[len];
	size_t convertedLen;
	mbstowcs_s(&convertedLen, p, len - 1, str.c_str(), _TRUNCATE);
	std::wstring str1(p);
	delete[] p;
	return str1;
}

class StringImplPool
{
public:
	std::map<size_t, std::list<StrImpl::WeakPtr>> buckets;
	StrImpl::SharedPtr GetOrCreate(const std::wstring& str)
	{
		size_t strHash = hash(str);
		auto& bucket = buckets[strHash];
		for (auto iter = bucket.begin(); iter != bucket.end(); ++iter)
		{
			if (auto s = (*iter).lock())
			{
				if (s->equal(str)) return s;
			}
			else
			{
				bucket.erase(iter);
			}
		}

		auto ptr = std::make_shared<StrImpl>(str);
		bucket.push_back(ptr);
		return ptr;
	}
	static size_t hash(const std::wstring& str)
	{
		size_t h = 0;
		size_t step = str.size() / 9 +1;
		for (size_t i = 0; i < str.size(); i+=step)
		{
			h = h ^ str[i];
		}
		return h;
	}
};
StringImplPool strPool;

String::String(const std::string& s):var(strPool.GetOrCreate(str2wstr(s)))
{
}

String::String(const std::wstring& s):var(strPool.GetOrCreate(s))
{
}

String::String(const var& a) : var(a, a.getImpl<StrImpl>())
{

}

String String::operator+(const String& other) const
{
	if (isUndefined() || other.isUndefined()) return undefined;
	StrImpl* p1 = getImpl<StrImpl>();
	StrImpl* p2 = other.getImpl<StrImpl>();
	return String(p1->str + p2->str);
}
