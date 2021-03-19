
#include <boost/property_tree/xml_parser.hpp>
#include "../common.h"

void Opt2Impl(const std::string &path, const std::string &outPath)
{
    DebugLogger logger("Opt2 Impl");
    boost::property_tree::ptree tree;
    std::ifstream f(path);
    boost::property_tree::read_xml(f, tree);
}