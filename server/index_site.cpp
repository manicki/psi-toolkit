#include <string>
#include <boost/bind.hpp>

#include "index_site.hpp"

IndexSite::IndexSite(PsiServer& server) : TemplateSite(server) {
    psiServer_.registerIncludeCode(
        "index_site_info", boost::bind(&IndexSite::info, this));

    psiServer_.registerIncludeCode(
        "index_site_scripts_part", boost::bind(&IndexSite::scriptsPart, this));
    psiServer_.registerIncludeCode(
        "index_site_header_part", boost::bind(&IndexSite::headerPart, this));
    psiServer_.registerIncludeCode(
        "index_site_footer_part", boost::bind(&IndexSite::footerPart, this));
}

char * IndexSite::info() {
    std::string str = "<p>Some string from IndexSite class!</p>";
    return stringToChar(str);
}

char * IndexSite::footerPart() {
    return stringToChar(readPsisFile("footer.psis"));
}

char * IndexSite::scriptsPart() {
    return stringToChar(readPsisFile("scripts.psis"));
}

char * IndexSite::headerPart() {
    return stringToChar(readPsisFile("header.psis"));
}
