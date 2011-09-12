#include <string>
#include <boost/bind.hpp>

#include "template_site.hpp"

class IndexSite : public TemplateSite
{

public:

	std::string inputText;

	IndexSite(PsiServer& server) : TemplateSite(server) {
		psiServer_.registerIncludeCode(
			"index_site_info", boost::bind(&IndexSite::info, this));
		psiServer_.registerIncludeCode(
			"index_site_footer_part", boost::bind(&IndexSite::footerPart, this));
	}
	
	char * info() {
		std::string str = "<p>Some string from IndexSite class!</p>";
		return stringToChar(str);
	}

	char * footerPart() {
		std::string footerPsis = readPsisFile("footer.psis");
		return stringToChar(footerPsis);
	}

};
