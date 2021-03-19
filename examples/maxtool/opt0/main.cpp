
#include <iostream>
#include <vector>
#include "../common.h"
#include "../tinyxml2/tinyxml2.h"
#include <memory>
#include <fstream>
#include "./base64.hpp"

typedef tinyxml2::XMLDocument TiXmlDocument;
typedef tinyxml2::XMLNode TiXmlNode;

namespace
{
    TiXmlDocument *readXMLDoc(const std::string &fileName)
    {
        std::unique_ptr<TiXmlDocument> xmlDoc(new TiXmlDocument());
        if (tinyxml2::XML_SUCCESS != xmlDoc->LoadFile(fileName.c_str()))
        {
            return nullptr;
        }
        return xmlDoc.release();
    }

    bool saveNodeToFile(TiXmlNode *node, const char *filename, bool compact = false)
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
        tinyxml2::XMLPrinter stream(fp, compact);
        node->Accept(&stream);
        fclose(fp);
        return true;
    }
}

void Opt0Impl(const std::string &path, const std::string &outPath)
{
    DebugLogger logger("Opt 0 Impl : tinyXml2 + macaron Base64 Header");

    logger.LogTime("Read XmlDoc");
    auto xmlDoc = readXMLDoc(path);

    logger.LogTime("Get XAF Part");
    auto rootNode = xmlDoc->FirstChildElement("Max_Custom_Anim");
    // xaf part
    std::string xafFile = outPath + "temp.xaf";
    {
        // TiXmlDocument xafDoc;
        auto xafNode = rootNode->FirstChildElement("MaxAnimation");
        // auto clonedXafNode = xafNode->DeepClone(&xafDoc);
        // xafDoc.LinkEndChild(clonedXafNode);
        // xafDoc.SaveFile(xafFile.c_str());
        saveNodeToFile(xafNode, xafFile.c_str());
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
        std::string bipContent;
        while (bipElem != nullptr)
        {
            std::wstring bipName = str2wstr(bipElem->Attribute("RootObject"));
            std::wstring bipedFilePath = GetBipedFilePath(bipName);
            bipedFilePaths.push_back({bipName, bipedFilePath});

            std::ofstream ofs(bipedFilePath, std::ios::binary | std::ios::out);
            auto bipContentElem = bipElem->FirstChildElement();
            while (bipContentElem != nullptr)
            {
                macaron::Base64::Decode(bipContentElem->Attribute("v"), bipContent);
                bipContentElem = bipContentElem->NextSiblingElement();
                ofs.write((char *)(&bipContent[0]), bipContent.size());
            }
            bipElem = bipElem->NextSiblingElement();
        }
    }
}