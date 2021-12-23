#pragma once
#include <jsc/impl.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

typedef icu::UnicodeString ICUStr;
class TStr : public ICUStr
{
public:
	using UnicodeString::UnicodeString;
	inline TStr(const ICUStr&s) :ICUStr(s) {}
	static TStr from(const std::u32string& s);
};

class StringStreamImpl : public IImpl
{
public:
    StringStreamImpl &operator<<(const TStr &s)
    {
        buffer.append(s);
        return *this;
    }
    TStr str() const { return buffer; }
    inline void Clear() { buffer = TStr(); }
protected:
    TStr buffer;
};

class StrImpl : public IValue
{
    friend class String;
    friend class StringStream;
public:
    typedef TStr InternalType;
public:
	String toString() const;

    static size_t hashCode(const TStr& str)
    {
        size_t h = 0;
		size_t step = str.length() / 9 + 1;
		for (size_t i = 0; i < str.length(); i += step)
		{
			h = h ^ (size_t)(str[i]);
		}
		return h;
    }
    virtual size_t getHash()const
    {
        return hashCode(str);
    }

    inline bool equal(const TStr &str) const { return (this->str == str)!=UBool(0); }
    StrImpl(const TStr &s) : str(s) {}
protected:
    const TStr str;
};
