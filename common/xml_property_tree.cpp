#include "xml_property_tree.hpp"

#include <boost/property_tree/xml_parser.hpp>

XmlPropertyTree::XmlPropertyTree(const boost::filesystem::path& xmlFilePath) {
    boost::property_tree::read_xml(xmlFilePath.string(), (boost::property_tree::ptree&)*this);
}
