#pragma once
#include <jsc/impl.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

typedef icu::UnicodeString ICUStr;
class TStr : public ICUStr
{
public:
    using UnicodeString::UnicodeString;
    inline TStr(const ICUStr &s) : ICUStr(s) {}
    static TStr from(const std::u32string &s);
};


class StringStreamImpl : public IImpl
{
public:
	StringStreamImpl():buffer(nullptr, 50) {}
    StringStreamImpl &operator<<(const TStr &s)
    {
        buffer.append(s);
        return *this;
    }
    TStr str() const { return buffer; }
    inline void Clear() { buffer = TStr(nullptr, 50); }

protected:
    TStr buffer;
};

struct TStrHash
{
	std::size_t operator()(const TStr &str) const noexcept
	{
		return str.hashCode();
	}
};
struct TStrEqual
{
	bool operator()(const TStr& s1, const TStr& s2)const noexcept
	{
		return (s1 == s2) != UBool(0);
	}
};

class StrImpl : public TValueImpl<TStr, TStrHash, TStrEqual>
{
    friend class String;

public:
    String toString() const;
    StrImpl(const TStr &s) : TValueImpl(s) {}
};
