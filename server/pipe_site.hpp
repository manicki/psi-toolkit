#include <string>
#include <boost/bind.hpp>

#include "template_site.hpp"

class PipeSite : public TemplateSite
{

public:

	PipeSite(PsiServer& server) : TemplateSite(server) {
		psiServer_.registerIncludeCode(
			"pipe_site_pipe_text", boost::bind(&PipeSite::tempPipeText, this));
		psiServer_.registerIncludeCode(
			"pipe_site_pipe", boost::bind(&PipeSite::pipeText, this));
		psiServer_.registerActionCode(
			"pipe_text", boost::bind(&PipeSite::actionPipeText, this));
	}

	char * tempPipeText() {
		std::string str = "";
		if (pipeText_.length()) {
			str = "<p>Your pipe:</p><p>" + pipeText_ + "</p>";
		}
		return stringToChar(str);
	}

	char * pipeText() {
		return stringToChar(pipeText_);
	}
	
	char * actionPipeText() {
		pipeText_ = psiServer_.findValue("pipe-text");
		return stringToChar(std::string("/index.html"));
	}

private:

	std::string pipeText_;

};

