#include <iostream>
#include <vector>
#include "../common.h"
#include "../rapidxml/rapidxml.hpp"
#include <memory>
#include <fstream>
#include "../base/base64.h"

typedef rapidxml::xml_document<> TiXmlDocument;

// TiXmlDocument *
// readXMLDoc(const std::string &fileName)
// {
//     std::unique_ptr<TiXmlDocument> xmlDoc(new TiXmlDocument);
//     if (tinyxml2::XML_SUCCESS != xmlDoc->LoadFile(fileName.c_str()))
//     {
//         return nullptr;
//     }
//     return xmlDoc.release();
// }

void Opt1Impl(const std::string &path, const std::string &outPath)
{
    // DebugLogger logger("Opt 0 Impl");

    // logger.LogTime("Read XmlDoc");
    // auto xmlDoc = readXMLDoc(path);

    // logger.LogTime("Get XAF Part");
    // auto rooNtode = xmlDoc->FirstChildElement("Max_Custom_Anim");
    // // xaf part
    // std::string xafFile = outPath + "temp.xaf";
    // {
    //     TiXmlDocument xafDoc;
    //     auto xafNode = rootNode->FirstChildElement("MaxAnimation");
    //     auto clonedXafNode = xafNode->DeepClone(&xafDoc);
    //     xafDoc.LinkEndChild(clonedXafNode);
    //     xafDoc.SaveFile(xafFile.c_str());
    // }

    // logger.LogTime("Get Bip Part");
    // auto appTempFolder = str2wstr(outPath);
    // auto GetFilePath = [&](const std::wstring &filename) -> std::wstring { return appTempFolder + L"/" + filename; };
    // auto GetBipedFilePath = [&](const std::wstring &name) -> std::wstring { return GetFilePath(name + L".bip"); };
    // // Read Biped files
    // std::vector<std::pair<std::wstring, std::wstring>> bipedFilePaths;
    // {
    //     auto bipAnimationNode = rootNode->FirstChildElement("BIP_Animation");
    //     auto bipElem = bipAnimationNode->FirstChildElement("BIP_Header");
    //     std::vector<uint8_t> bipContent;
    //     while (bipElem != nullptr)
    //     {
    //         std::wstring bipName = str2wstr(bipElem->Attribute("RootObject"));
    //         std::wstring bipedFilePath = GetBipedFilePath(bipName);
    //         bipedFilePaths.push_back({bipName, bipedFilePath});

    //         std::ofstream ofs(bipedFilePath, std::ios::binary | std::ios::out);
    //         auto bipContentElem = bipElem->FirstChildElement();
    //         while (bipContentElem != nullptr)
    //         {
    //             std::string rawData = bipContentElem->Attribute("v");
    //             bipContent = cutils::algo::Base64Decode(rawData);
    //             bipContentElem = bipContentElem->NextSiblingElement();
    //             ofs.write((char *)(&bipContent[0]), bipContent.size());
    //         }
    //         bipElem = bipElem->NextSiblingElement();
    //     }
    // }
}