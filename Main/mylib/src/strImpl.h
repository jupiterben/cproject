#pragma once
#include <unicode/utypes.h>
#include <unicode/stringpiece.h>
#include <unicode/utf8.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>

class StrImpl : public IImpl
{
    friend class String;

public:
    typedef std::shared_ptr<StrImpl> SharedPtr;
    typedef std::weak_ptr<StrImpl> WeakPtr;

public:
    virtual StrImpl *toStrImpl() { return this; }
    inline static StrImpl *cast(IImpl *impl) { return impl ? impl->toStrImpl() : nullptr; }
	TStr toString() const
    { 
        return _TS("\"") + str + _TS("\"");
    }

    inline bool equal(const TStr &str) const { return this->str == str; }
    StrImpl(const TStr &s) : str(s) {}

private:
    const TStr str;
};
