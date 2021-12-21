#pragma once
#include <jsc/var.h>

class TStr;
class Console
{
public:
    void log(const var& v)const;
private:
    void log(const TStr &s)const;
};
extern Console console;