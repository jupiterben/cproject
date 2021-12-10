
#include <iostream>
#include <vector>
#include "../tinyxml/tinyxml.h"
#include "../common.h"
#include <memory>
#include <fstream>
#include "./base64.h"
#include "../language.h"

TiXmlDocument *readXMLDoc(const std::string &fileName)
{
    std::unique_ptr<TiXmlDocument> xmlDoc(new TiXmlDocument());
    if (!xmlDoc->LoadFile(fileName.c_str()))
    {
        return nullptr;
    }
    return xmlDoc.release();
}

bool saveNodeToFile(TiXmlNode *node, const char *filename)
{
    if (!filename)
    {
        return false;
    }
    FILE *fp = 0;
    const errno_t err = fopen_s(&fp, filename, "w");
    if (err)
    {
        return 0;
    }
    node->Print(fp, 0);
    fclose(fp);
    return true;
}

void BaseImpl(const std::string &path, const std::string &outPath)
{
    DebugLogger logger("Base Impl: TinyXML + slow Base64");

    logger.LogTime("Read XmlDoc");
    auto xmlDoc = readXMLDoc(path);

    logger.LogTime("Get XAF Part");
    auto rootNode = xmlDoc->FirstChildElement("Max_Custom_Anim");

    bool transRes = lang::XAF::GetInstnace()->Translate(rootNode, lang::LanguageType::CHS, lang::LanguageType::ENU);

    // xaf part
    std::string xafFile = outPath + "temp.xaf";
    {
        TiXmlDocument xafDoc;
        auto xafNode = rootNode->FirstChild("MaxAnimation");
        //saveNodeToFile(xafNode, xafFile.c_str());
        auto clonedXafNode = xafNode->Clone();
        xafDoc.LinkEndChild(clonedXafNode);
        xafDoc.SaveFile(xafFile.c_str());
    }

    logger.LogTime("Get Bip Part");
    auto appTempFolder = str2wstr(outPath);
    auto GetFilePath = [&](const std::wstring &filename) -> std::wstring { return appTempFolder + L"/" + filename; };
    auto GetBipedFilePath = [&](const std::wstring &name) -> std::wstring { return GetFilePath(name + L".bip"); };
    // Read Biped files
    std::vector<std::pair<std::wstring, std::wstring>> bipedFilePaths;
    {
        auto bipAnimationNode = rootNode->FirstChildElement("BIP_Animation");
        auto bipElem = bipAnimationNode->FirstChildElement("BIP_Header");
        std::vector<uint8_t> bipContent;
        while (bipElem != nullptr)
        {
            std::wstring bipName = str2wstr(bipElem->Attribute("RootObject"));
            std::wstring bipedFilePath = GetBipedFilePath(bipName);
            bipedFilePaths.push_back({bipName, bipedFilePath});

            std::string rawData;
            auto bipContentElem = bipElem->FirstChildElement();
            while (bipContentElem != nullptr)
            {
                rawData += bipContentElem->Attribute("v");
                bipContentElem = bipContentElem->NextSiblingElement();
            }

            std::vector<uint8_t> bipContent = cutils::algo::Base64Decode(rawData);
            std::ofstream ofs(bipedFilePath, std::ios::binary | std::ios::out);
            ofs.write((char *)(&bipContent[0]), bipContent.size());

            bipElem = bipElem->NextSiblingElement();
        }
    }
}