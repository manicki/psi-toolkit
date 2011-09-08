#include "srx_rules.hpp"

#include <boost/foreach.hpp>

#include "object_cache.hpp"
#include "regexp.hpp"

#include "logging.hpp"

SrxRulesReader::SrxRulesReader(
    const boost::filesystem::path& filePath,
    const std::string& lang)
    :xmlParsed_(ObjectCache::getInstance().getObject<XmlPropertyTree>(filePath)),
     lang_(lang),
     srx_version(1),
     cascade_(false) {
    findLanguageRuleNames_();
}

void SrxRulesReader::findLanguageRuleNames_() {
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                  xmlParsed_->get_child("srx.body")) {
        if (v.first == "maprules") {
            if (processMapRules_(v.second))
                break;
        }
    }
}

bool SrxRulesReader::processMapRules_(boost::property_tree::ptree& maprules) {
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, maprules.get_child("")) {
        if (v.first == "languagemap") {
            if (processLanguageMap_(v.second))
                return true;
        }
    }

    return false;
}

bool SrxRulesReader::processLanguageMap_(boost::property_tree::ptree& langmap) {
    std::string languagePattern = langmap.get<std::string>("<xmlattr>.languagepattern");
    std::string languageRuleName = langmap.get<std::string>("<xmlattr>.languagerulename");

    if (PerlRegExp::FullMatch(languagePattern, lang_)) {
        languageRuleNames_.insert(languageRuleName);

        INFO("SRX: " << languageRuleName << " for language: " << lang_);

        if (cascade_)
            return true;
    }

    return false;
}
