#include <jsc/console.h>
#include <jsc/var.h>
#include <iostream>
#include <jsc/str.h>
#include "strImpl.h"

#ifdef _WINDOWS
#include <windows.h>
void Console::log(const TStr& s)const
{
	SetConsoleOutputCP(CP_UTF8);
	std::cout << s << std::endl;
}
#else
void Console::log(const TStr &s) const
{
	std::cout << s << std::endl;
}
#endif 

Console console;
void Console::log(const var &v) const
{
	log(v.toString().str());
}
