#include "fstring.h"
#include <locale>
#include <codecvt>

inline std::string UTF8Code(const TSTR &str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(str.data());
}

inline std::string UTF8Code(const wchar_t *str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(str);
}

inline std::wstring Unicode(const char *str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}

FString::FString(const TSTR &s) : data(UTF8Code(s))
{
}

FString::FString(const TCHAR *s) : data(UTF8Code(s))
{
}

FString::FString(const char *s):data(s)
{
}

void FString::resize(size_t l, char ch)
{
	data.resize(l, ch);
}

FString& FString::append(const char* s, size_t len)
{
	data.append(s, len);
	return *this;
}
