#include <jsc/console.h>
#include <jsc/var.h>
#include <iostream>
#include <jsc/str.h>

Console console;
void Console::log(var& v)const
{
    std::wcout << v << std::endl;
}