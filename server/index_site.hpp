#include <string>
#include <boost/bind.hpp>

#include "template_site.hpp"

class IndexSite : public TemplateSite
{

public:

	IndexSite(PsiServer& server) : TemplateSite(server) {
		psi_server_.registerIncludeCode(
			"index_site_info", boost::bind(&IndexSite::info, this));
		psi_server_.registerIncludeCode(
			"index_site_input_text", boost::bind(&IndexSite::inputText, this));
		psi_server_.registerActionCode(
			"input_text", boost::bind(&IndexSite::actionInputText, this));
	}
	
	char * info() {
		std::string str = "<p>Some string from IndexSite class!</p>";
		return stringToChar(str);
	}

	char * inputText() {
		std::string str = "";
		if (input_text_.length()) {
			str = "<p>Your text:</p><p>" + input_text_ + "</p>";
		}
		return stringToChar(str);
	}
	
	char * actionInputText() {
		input_text_ = psi_server_.findValue("input-text");
		return stringToChar(std::string("/index.html"));
	}

private:

	std::string input_text_;

};
