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
        buffer += s;
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
    typedef std::shared_ptr<StrImpl> SharedPtr;
    typedef std::weak_ptr<StrImpl> WeakPtr;

public:
	String toString() const;

    inline bool equal(const TStr &str) const { return (this->str == str)!=UBool(0); }
    StrImpl(const TStr &s) : str(s) {}
protected:
    const TStr str;
};
