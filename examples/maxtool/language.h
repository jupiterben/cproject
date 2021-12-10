#pragma once
#include <string>
#include <vector>
#include <map>

#include "tinyxml/tinyxml.h"

namespace lang
{
class LanguageType
{
public:
    static std::string ENU;
    static std::string CHS;
};

struct LanguageDict
{
    std::vector<std::pair<std::string, std::string>> val;
};
std::string Translate(const char *cstr, const LanguageDict &dict);

class XAF
{
private:
    typedef std::map<std::string, LanguageDict> AttrDict;
    std::map<std::string, AttrDict> fromEnuDict;
    std::map<std::string, AttrDict> toEnuDict;

    static XAF* instance;
    XAF();
    void Translate(TiXmlNode *node, const AttrDict &dict);
    std::string GuessLanguage(TiXmlElement *node, const std::string &attr);
public:
    static XAF* GetInstnace();
    bool Translate(TiXmlNode *node, std::string from, std::string to);
    std::string GuessLanguage(TiXmlNode *node);
};
}
