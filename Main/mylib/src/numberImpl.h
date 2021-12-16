#pragma once
#include <jsc/impl.h>
#include <sstream>

class Number;
class NumberImpl : public IImpl
{
    friend class Number;

private:
    const double x;

public:
    inline NumberImpl(double _x) : x(_x) {}
    inline operator double() const { return x; }

public:
    virtual NumberImpl *toNumberImpl() { return this; }
    inline static NumberImpl *cast(IImpl *impl) { return impl ? impl->toNumberImpl() : nullptr; }
    std::wstring toString() const
    {
        std::wstringstream ss;
        ss << x;
        return ss.str();
    }

protected:
    Number operator+(const NumberImpl &other) const;
    Number operator-(const NumberImpl &other) const;
    Number operator*(const NumberImpl &other) const;
    Number operator/(const NumberImpl &other) const;
};
