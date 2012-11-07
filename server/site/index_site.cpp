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
    psiServer_.registerIncludeCode(
        "index_site_select_output_part", boost::bind(&IndexSite::selectOutputPart, this));
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
    std::string version = std::string("psi-toolkit v. ")
        + get_psi_toolkit_last_date_string()
        + " [<a target=\"_blank\" href=\""
        + "http://mrt.wmi.amu.edu.pl:8000/projects/psi-toolkit/repository/revisions/"
        + get_psi_toolkit_last_commit_string() + "\">"
        + get_psi_toolkit_last_commit_string()
        + "</a>]";

    boost::replace_first(footer, "<!--#psis version -->", version);

    return stringToChar(footer);
}

char * IndexSite::selectOutputPart() {
    return stringToChar(readPsisFile("select_output.psis"));
}
