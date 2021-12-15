#pragma once
#include "impl.h"

class ArrayImpl : public IImpl
{
public:
    ArrayImpl(std::initializer_list<var> list) {}
    ArrayImpl *toArray(){ return this;}
};