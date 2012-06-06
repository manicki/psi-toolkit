#include <string>

#include <boost/bind.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "index_site.hpp"
#include "version_information.hpp"

IndexSite::IndexSite(PsiServer& server) : TemplateSite(server) {
    psiServer_.registerIncludeCode(
        "index_site_scripts_part", boost::bind(&IndexSite::scriptsPart, this));
    psiServer_.registerIncludeCode(
        "index_site_header_part", boost::bind(&IndexSite::headerPart, this));
    psiServer_.registerIncludeCode(
        "index_site_promotion_part", boost::bind(&IndexSite::promotionPart, this));
    psiServer_.registerIncludeCode(
        "index_site_footer_part", boost::bind(&IndexSite::footerPart, this));
}

char * IndexSite::scriptsPart() {
    return stringToChar(readPsisFile("scripts.psis"));
}

char * IndexSite::headerPart() {
    return stringToChar(readPsisFile("header.psis"));
}

char * IndexSite::promotionPart() {
    return stringToChar(readPsisFile("promotion.psis"));
}

char * IndexSite::footerPart() {
    std::string footer = readPsisFile("footer.psis");
    std::string version = get_psi_toolkit_version_string();

    boost::replace_first(footer, "<!--#psis version -->", version);

    return stringToChar(footer);
}
