#include "./common.h"

std::string parseFilepath(const std::string &filepath)
{
    if (!filepath.empty())
    {
        size_t locpoint = filepath.find_last_of('.');
        size_t locfilename = filepath.find_last_of('\\');
        return filepath.substr(0, locfilename + 1);
    }
    return "";
}

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
