#ifndef SRX_RULES_HPP
#define SRX_RULES_HPP

#include <string>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

#include "xml_property_tree.hpp"

class SrxRule {

public:
    SrxRule(bool breakable, const std::string& regexp);

private:
    bool breakable_;
    std::string regexp_;
};

class SrxRulesReader {
public:
    SrxRulesReader(
        const boost::filesystem::path& filePath,
        const std::string& lang);

    template<typename Out>
    void getRules(Out outputIterator) {
    }

private:
    void findLanguageRuleNames_();
    bool processMapRules_(boost::property_tree::ptree& maprules);
    bool processLanguageMap_(boost::property_tree::ptree& langmap);

    std::set<std::string> languageRuleNames_;

    boost::shared_ptr<XmlPropertyTree> xmlParsed_;
    const std::string& lang_;
    unsigned int srx_version;
    bool cascade_;
};

#endif
