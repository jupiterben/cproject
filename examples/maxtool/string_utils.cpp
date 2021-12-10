#include "string_utils.h"

#include <string>
#include <locale.h> 
#include <sstream>
#include <iostream>
#include <windows.h>

namespace cutils
{
namespace string
{
std::wstring str2wstr(const std::string &str)
{
    unsigned len = (unsigned)str.size() * 2 + 10;
    setlocale(LC_CTYPE, "");
    wchar_t *p = new wchar_t[len];
    size_t convertedLen;
    mbstowcs_s(&convertedLen, p, len - 1, str.c_str(), _TRUNCATE);
    std::wstring str1(p);
    delete[] p;
    return str1;
}

std::vector<std::wstring> split_string(const std::wstring & str, wchar_t splitter)
{
	std::vector<std::wstring> ret;
	std::wstring segment;
	std::wstringstream ss(str);
	while (std::getline(ss, segment, splitter))
	{
		ret.push_back(segment);
	}
	return ret;
}

std::string wstr2str(const std::wstring &str)
{
    unsigned len = (unsigned)str.size() * 4 + 10;
    setlocale(LC_CTYPE, "");
    char *p = new char[len];
    size_t convertedLen;
    wcstombs_s(&convertedLen, p, len - 1, str.c_str(), _TRUNCATE);
    std::string str1(p);
    delete[] p;
    return str1;
}

std::string wstr2local(unsigned int cpTo, const wchar_t * wstrFrom)
{
    int len = WideCharToMultiByte(cpTo, 0, wstrFrom, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(cpTo, 0, wstrFrom, -1, str, len, NULL, NULL);
    std::string localStr = str;
    if (str) delete[] str;
    return localStr;
}
wchar_t *local2wstr(unsigned int cpFrom, const char* strFrom)
{
    int len = MultiByteToWideChar(cpFrom, 0, strFrom, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(cpFrom, 0, strFrom, -1, wstr, len);
    return wstr;
}

std::string CodePageTransfer(int cpFrom, int cpTo, const char* strFrom)
{
    wchar_t* wstr = local2wstr(cpFrom, strFrom);
    std::string strTo = wstr2local(cpTo, wstr);
    if (wstr) delete[] wstr;
    return strTo;
}

std::string local2utf8(const char* fromStr)
{
    return CodePageTransfer(CP_ACP, CP_UTF8, fromStr);
}

std::string utf82local(const char * fromStr)
{
    return CodePageTransfer(CP_UTF8, CP_ACP, fromStr);
}
}
}
