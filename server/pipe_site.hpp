#include <string>
#include <boost/bind.hpp>

#include "template_site.hpp"

class PipeSite : public TemplateSite
{

public:

	PipeSite(PsiServer& server) : TemplateSite(server) {
		psi_server_.registerIncludeCode(
			"pipe_site_pipe_text", boost::bind(&PipeSite::pipeText, this));
		psi_server_.registerActionCode(
			"pipe_text", boost::bind(&PipeSite::actionPipeText, this));
	}

	char * pipeText() {
		std::string str = "";
		if (pipe_text_.length()) {
			str = "<p>Your pipe:</p><p>" + pipe_text_ + "</p>";
		}
		return stringToChar(str);
	}
	
	char * actionPipeText() {
		pipe_text_ = psi_server_.findValue("pipe-text");
		return stringToChar(std::string("/index.html"));
	}

private:

	std::string pipe_text_;

};

