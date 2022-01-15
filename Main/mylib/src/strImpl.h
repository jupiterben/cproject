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

class StringStreamImpl
{
public:
    StringStreamImpl() : buffer(nullptr, 10) {}
    StringStreamImpl &operator<<(const TStr &s)
    {
        buffer.append(s);
        return *this;
    }
    TStr str() const { return buffer; }
    inline void Clear() { buffer = TStr(nullptr, 10); }
protected:
    TStr buffer;
};

template <>
struct std::hash<TStr>
{
    std::size_t operator()(const TStr &str) const noexcept
    {
        return str.hashCode();
    }
};
template <>
struct std::equal_to<TStr>
{
    bool operator()(const TStr &s1, const TStr &s2) const noexcept
    {
        return (s1 == s2) != UBool(0);
    }
};

class StrImpl : public TValueImpl<TStr>
{
    friend class String;

public:
    String toString() const;
    StrImpl(const TStr &s, size_t hash) : TValueImpl(s, hash) {}
};
