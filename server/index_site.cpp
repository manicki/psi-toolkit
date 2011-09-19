#include <string>
#include <boost/bind.hpp>

#include "index_site.hpp"

IndexSite::IndexSite(PsiServer& server) : TemplateSite(server) {
    psiServer_.registerIncludeCode(
        "index_site_info", boost::bind(&IndexSite::info, this));
    psiServer_.registerIncludeCode(
        "index_site_footer_part", boost::bind(&IndexSite::footerPart, this));
}
	
char * IndexSite::info() {
    std::string str = "<p>Some string from IndexSite class!</p>";
    return stringToChar(str);
}

char * IndexSite::footerPart() {
    std::string footerPsis = readPsisFile("footer.psis");
    return stringToChar(footerPsis);
}

