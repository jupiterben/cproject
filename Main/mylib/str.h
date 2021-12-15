#pragma once

#include "impl.h"
#include <string>

class StringImpl : public IImpl
{
private:
    std::wstring x;
};