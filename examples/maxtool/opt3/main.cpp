
#include <iostream>
#include <vector>
#include "../common.h"
#include <memory>
#include <fstream>
#include "../tinyxml2/tinyxml2.h"
#include "../modp_b64/modp_b64.h"

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

void Opt3Impl(const std::string &path, const std::string &outPath)
{
    DebugLogger logger("Opt 3 Impl : tinyXml2 + chromium Base64");

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
                std::string bipContent = bipContentElem->Attribute("v");
                modp_b64_decode(bipContent);
                bipContentElem = bipContentElem->NextSiblingElement();
                ofs.write((char *)(&bipContent[0]), bipContent.size());
            }
            bipElem = bipElem->NextSiblingElement();
        }
    }
}