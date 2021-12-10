#include "language.h"
#include <vector>
#include <map>

#include "string_utils.h"

namespace lang
{

std::string LanguageType::ENU = "ENU";
std::string LanguageType::CHS = "CHS";

XAF *XAF::instance = nullptr;
XAF *XAF::GetInstnace()
{
    if (instance == nullptr)
    {
        instance = new XAF();
    }
    return instance;
}

XAF::XAF()
{
    // CHS
    {
        toEnuDict[LanguageType::CHS]["parentNode"].val = {
            { "场景根", "Scene Root"}
        };
        toEnuDict[LanguageType::CHS]["classOf"].val = {
            { "躯干", "Body" }
            ,{ "浮点", "Float" }
            ,{ "位置", "Position" }
            ,{ "旋转", "Rotation" }
            ,{ "缩放", "Scale" }
        };
        toEnuDict[LanguageType::CHS]["name"].val = {
            { "轴旋转", "Rotation" }
            ,{ "旋转", "Rotation" }
            ,{ "位置", "Position" }
            ,{ "变换", "Transform" }
            ,{ "缩放", "Scale" }
            ,{ "垂直", "Vertical" }
            ,{ "水平", "Horizontal" }
            ,{ "翻转", "Turning" }
            ,{ "修改对象", "Modified Object" }
            ,{ "变形器", "Morpher" }
            ,{ "无目标", "No Target" }
			,{ "对象 (目标摄影机)", "Object (Target Camera)" }
            ,{ "视野", "FOV"}
        };

        fromEnuDict[LanguageType::CHS]["parentNode"].val = {
            { "Scene Root", "场景根" }
        };
        fromEnuDict[LanguageType::CHS]["classOf"].val = {
            { "Body", "躯干" }
            ,{ "Float", "浮点" }
            ,{ "Position", "位置" }
            ,{ "Rotation", "旋转" }
            ,{ "Scale", "缩放" }
        };
        fromEnuDict[LanguageType::CHS]["name"].val = {
            { "X Rotation", "X 轴旋转" }
            ,{ "Y Rotation", "Y 轴旋转" }
            ,{ "Z Rotation", "Z 轴旋转" }
            ,{ "Rotation", "旋转" }
            ,{ "Position", "位置" }
            ,{ "Transform", "变换" }
            ,{ "Scale", "缩放" }
            ,{ "Vertical", "垂直" }
            ,{ "Horizontal", "水平" }
            ,{ "Turning", "翻转" }
            ,{ "Modified Object", "修改对象" }
            ,{ "Morpher", "变形器" }
            ,{ "No Target", "无目标" }
			,{ "FOV","视野" }
        };
    }

    // All to UTF8
    std::map<std::string, AttrDict> *xafDict[2];
    xafDict[0] = &toEnuDict;
    xafDict[1] = &fromEnuDict;
    for (int di = 0; di < 2; di++)
    {
        for (auto iter = xafDict[di]->begin(); iter != xafDict[di]->end(); iter++)
        {
            AttrDict &attrDict = iter->second;
            for (auto iter2 = attrDict.begin(); iter2 != attrDict.end(); iter2++)
            {
                std::vector<std::pair<std::string, std::string>> &dict = iter2->second.val;
                for (int i = 0; i < dict.size(); i++)
                {
                    dict[i].first = cutils::string::local2utf8(dict[i].first.c_str());
                    dict[i].second = cutils::string::local2utf8(dict[i].second.c_str());
                }
            }
        }
    }
}

std::string Translate(const char *cstr, const LanguageDict &dict)
{
    std::string str(cstr);
    for (int i = 0; i < dict.val.size(); i++)
    {
        std::string::size_type pos = 0u;
        while ((pos = str.find(dict.val[i].first, pos)) != std::string::npos)
        {
            str.replace(pos, dict.val[i].first.length(), dict.val[i].second);
            pos += dict.val[i].second.length();
        }
    }
    return str;
}

void XAF::Translate(TiXmlNode *root, const AttrDict &dict)
{
    static const int nodeAttrCnt = 1;
    static const char nodeAttr[nodeAttrCnt][16] = {
        "parentNode"
    };
    static const int controllerAttrCnt = 2;
    static const char controllerAttr[controllerAttrCnt][16] = {
        "name", "classOf"
    };

    if (root == nullptr) return;
    TiXmlNode *anim = root->FirstChild("MaxAnimation");
    if (anim == nullptr) return;
#pragma region translate node
    auto node = anim->FirstChildElement("Node");
    while (node)
    {
        for (int i = 0; i < nodeAttrCnt; i++)
        {
            auto iter = dict.find(nodeAttr[i]);
            if (iter == dict.cend()) continue;
            node->SetAttribute(nodeAttr[i], lang::Translate(node->Attribute(nodeAttr[i]), iter->second).c_str());
        }
        
#pragma region translate controller
        auto controller = node->FirstChildElement("Controller");
        while (controller)
        {
            for (int i = 0; i < controllerAttrCnt; i++)
            {
                auto iter = dict.find(controllerAttr[i]);
                if (iter == dict.cend()) continue;
                controller->SetAttribute(controllerAttr[i], lang::Translate(controller->Attribute(controllerAttr[i]), iter->second).c_str());
            }
            controller = controller->NextSiblingElement();
        }
#pragma endregion

        node = node->NextSiblingElement();
    }
#pragma endregion
}
bool XAF::Translate(TiXmlNode *node, std::string from, std::string to)
{
    // 因为现在有些动画文件是中文的，所以要猜一下。以后都用当前版本逻辑的话，只会生成英文版，到时就不需要猜了
    // std::string guessLang = GuessLanguage(node);
    // if (guessLang != "" && from != guessLang)
    // {
    //     from = guessLang;
    // }

    if (from == to) return true;
    if (from != LanguageType::ENU)
    {
        if (toEnuDict.find(from) == toEnuDict.end()) return false;
        Translate(node, toEnuDict[from]);
    }
    if (to != LanguageType::ENU)
    {
        if (fromEnuDict.find(to) == fromEnuDict.end()) return false;
        Translate(node, fromEnuDict[to]);
    }
    return true;
}

std::string XAF::GuessLanguage(TiXmlElement *node, const std::string &attr)
{
    std::vector<std::string> langs = {
        LanguageType::CHS,
        LanguageType::ENU
    };
    std::vector<AttrDict*> attrDicts = {
        &toEnuDict[LanguageType::CHS],
        &fromEnuDict[LanguageType::CHS]
    };
    try
    {
        std::string val = node->Attribute(attr.c_str());
        for (int i = 0; i < langs.size(); i++)
        {
            std::string lang = langs[i];
            AttrDict *attrDict = attrDicts[i];
            auto iter = attrDict->find(attr);
            if (iter == attrDict->end()) continue;
            std::vector<std::pair<std::string, std::string>> &dict = iter->second.val;
            for (int i = 0; i < dict.size(); i++)
            {
                if (val.find(dict[i].first, 0) != std::string::npos)
                {
                    return lang;
                }
            }
        }
    }
    catch (...)
    {

    }
    return "";
}

std::string XAF::GuessLanguage(TiXmlNode *root)
{
    std::string defaultLang = "";

    static const int nodeAttrCnt = 1;
    static const char nodeAttr[nodeAttrCnt][16] = {
        "parentNode"
    };
    static const int controllerAttrCnt = 2;
    static const char controllerAttr[controllerAttrCnt][16] = {
        "name", "classOf"
    };

    if (root == nullptr) return defaultLang;
    TiXmlNode *anim = root->FirstChild("MaxAnimation");
    if (anim == nullptr) return defaultLang;

    int guessTime = 0;
    static const int maxGuessTime = 20;
    std::string guessResult;

#pragma region translate node
    auto node = anim->FirstChildElement("Node");
    while (node && guessTime < maxGuessTime)
    {
        for (int i = 0; i < nodeAttrCnt && guessTime < maxGuessTime; i++)
        {
            guessResult = GuessLanguage(node, nodeAttr[i]);
            if (guessResult != "") return guessResult;
            guessTime++;
        }

#pragma region translate controller
        auto controller = node->FirstChildElement("Controller");
        while (controller)
        {
            for (int i = 0; i < controllerAttrCnt && guessTime < maxGuessTime; i++)
            {
                guessResult = GuessLanguage(controller, controllerAttr[i]);
                if (guessResult != "") return guessResult;
                guessTime++;
            }
            controller = controller->NextSiblingElement();
        }
#pragma endregion

        node = node->NextSiblingElement();
    }
#pragma endregion
    return defaultLang;
}

}