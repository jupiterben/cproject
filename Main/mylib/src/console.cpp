#include <jsc/console.h>
#include <jsc/var.h>
#include <iostream>
#include <jsc/str.h>

Console console;
void Console::log(var& v)const
{
    String s = v.toString();
    std::wcout << s.cStr() << std::endl;
}