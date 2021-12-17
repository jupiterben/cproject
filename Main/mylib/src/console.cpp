#include <jsc/console.h>
#include <jsc/var.h>
#include <iostream>
#include <jsc/str.h>
#include "c17.hpp"

// https://stackoverflow.com/questions/7232710/convert-between-string-u16string-u32string
std::string U32toUTF8()
{
    std::string result;
    std::codecvt_utf8<T::value_type>
        std::wstring_convert<codecvt_utf8_utf16<T>, T> convertor;
    result = convertor.to_bytes(source);
    return result;
    
}


Console console;
void Console::log(var& v)const
{
	std::cout << c17::toUTF8(v.toString().cStr()) << std::endl;
    //std::wcout << v << std::endl;
}