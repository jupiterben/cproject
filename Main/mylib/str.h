#pragma once

#include "impl.h"
#include <string>

class StringImpl : public IImpl
{
private:
    std::wstring x;
public:
    virtual StringImpl *toString() { return this; }
	inline static StringImpl* cast(IImpl* impl) { return impl ? impl->toString() : nullptr; }
};