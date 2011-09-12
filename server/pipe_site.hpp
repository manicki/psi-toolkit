#include <string>
#include <boost/bind.hpp>

#include "template_site.hpp"

class PipeSite : public TemplateSite
{

public:

	PipeSite(PsiServer& server, std::string initialPipe) 
		: TemplateSite(server), pipeText_(initialPipe)
	{
		psiServer_.registerIncludeCode(
			"pipe_site_pipe_text", boost::bind(&PipeSite::pipeText, this));
		psiServer_.registerActionCode(
			"pipe_text", boost::bind(&PipeSite::actionPipeText, this));
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

