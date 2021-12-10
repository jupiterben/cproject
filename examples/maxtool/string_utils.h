#pragma once
#include <string>
#include <vector>
namespace cutils
{
namespace string
{
std::string wstr2str(const std::wstring &wstr);
std::wstring str2wstr(const std::string &str);
std::vector<std::wstring> split_string(const std::wstring &str, wchar_t splitter);
std::string local2utf8(const char* strFrom);
std::string utf82local(const char* strFrom);
wchar_t *local2wstr(unsigned int cpFrom, const char* strFrom);
std::string wstr2local(unsigned int cpTo, const wchar_t* wstrFrom);
}
}
