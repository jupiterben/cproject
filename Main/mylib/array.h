#pragma once
#include  <initializer_list>
#include "impl.h"
#include "var.h"

class ArrayImpl : public IImpl
{
public:
    ArrayImpl(std::initializer_list<var> list);
    ArrayImpl *toArray(){ return this;}
    inline static ArrayImpl* cast(IImpl* impl) { return impl ? impl->toArray() : nullptr; }

};