#include <jsc/console.h>
#include <jsc/var.h>
#include <iostream>
#include <jsc/str.h>
#include "c17.hpp"

// https://stackoverflow.com/questions/7232710/convert-between-string-u16string-u32string
// https://stackoverflow.com/questions/32055357/visual-studio-c-2015-stdcodecvt-with-char16-t-or-char32-t
std::string U32toUTF8(const std::u32string& source)
{
    typedef std::codecvt_utf8<char32_t> convert_type;
    return std::wstring_convert<convert_type, char32_t>{}.to_bytes(source);
}

Console console;
void Console::log(var& v)const
{
    
}