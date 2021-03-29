#include "./tinyxml.h"
#include <iostream>

int main()
{
    TiXmlDocument xmlDoc;

    TiXmlElement *rootNode = new TiXmlElement("Max_Custom_Anim");
    TiXmlElement *xmlElement = new TiXmlElement("");
    std::string content = "<xxx></xxx><xxx></xxx>";
    xmlElement->Parse(content.c_str(), 0, TIXML_ENCODING_UTF8);
    rootNode->LinkEndChild(xmlElement);
    xmlDoc.LinkEndChild(rootNode);
    xmlDoc.SaveFile("d:\\a.xml");
}