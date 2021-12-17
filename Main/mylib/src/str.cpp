#include <jsc/str.h>
#include "strImpl.h"
#include <map>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <string>
#include <locale.h>

// std::wstring s2ws(const std::string &str)
// {
// 	if (str.empty())
// 	{
// 		return L"";
// 	}
// 	unsigned len = str.size() + 1;
// 	setlocale(LC_CTYPE, "en_US.UTF-8");
// 	std::unique_ptr<wchar_t[]> p(new wchar_t[len]);
// 	mbstowcs(p.get(), str.c_str(), len);
// 	std::wstring w_str(p.get());
// 	return w_str;
// }
// 
// std::string ws2s(const std::wstring &w_str)
// {
// 	if (w_str.empty())
// 	{
// 		return "";
// 	}
// 	unsigned len = w_str.size() * 4 + 1;
// 	setlocale(LC_CTYPE, "en_US.UTF-8");
// 	std::unique_ptr<char[]> p(new char[len]);
// 	wcstombs(p.get(), w_str.c_str(), len);
// 	std::string str(p.get());
// 	return str;
// }

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

// String::String(std::string &&s) : var(strPool.GetOrCreate(s2ws(s)))
// {
// }
// 
// String::String(std::wstring &&s) : var(strPool.GetOrCreate(s))
// {
// }

String::String(const var &a) : var(a, a.getImpl<StrImpl>())
{
}
// 
// String::String(const char s[]) : String(std::string(s))
// {
// }
// 
// String::String(const wchar_t s[]) : String(std::wstring(s))
// {
// }

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