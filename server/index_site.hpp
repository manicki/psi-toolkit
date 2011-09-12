#include <string>
#include <boost/bind.hpp>

#include "template_site.hpp"

class IndexSite : public TemplateSite
{

public:

	IndexSite(PsiServer& server) : TemplateSite(server) {
		psiServer_.registerIncludeCode(
			"index_site_info", boost::bind(&IndexSite::info, this));
		psiServer_.registerIncludeCode(
			"index_site_input_text", boost::bind(&IndexSite::inputText, this));
		psiServer_.registerActionCode(
			"input_text", boost::bind(&IndexSite::actionInputText, this));

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

	char * inputText() {
		std::string str = "";
		if (inputText_.length()) {
			str = "<p>Your text:</p><p>" + inputText_ + "</p>";
		}
		return stringToChar(str);
	}
	
	char * actionInputText() {
		inputText_ = psiServer_.findValue("input-text");
		return stringToChar(std::string("/index.html"));
	}

private:

	std::string inputText_;

};
