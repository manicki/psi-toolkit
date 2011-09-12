#include <string>
#include <boost/bind.hpp>

#include "template_site.hpp"

class PipeSite : public TemplateSite
{

public:

	std::string pipe;
	std::string input;

	PipeSite(PsiServer& server, std::string initialPipe) 
		: TemplateSite(server), pipe(initialPipe)
	{
		psiServer_.registerIncludeCode(
			"pipe_site_input_text", boost::bind(&PipeSite::inputText, this));
		psiServer_.registerActionCode(
			"input_text", boost::bind(&PipeSite::actionInputText, this));

		psiServer_.registerIncludeCode(
			"pipe_site_pipe_text", boost::bind(&PipeSite::pipeText, this));
		psiServer_.registerActionCode(
			"pipe_text", boost::bind(&PipeSite::actionPipeText, this));
	}

	char * pipeText() {
		return stringToChar(pipe);
	}
	
	char * actionPipeText() {
		pipe = psiServer_.findValue("pipe-text");
		return stringToChar(std::string("/index.html"));
	}

	char * inputText() {
		std::string str = "";
		if (input.length()) {
			str = "<p>Your text:</p><p>" + input + "</p>";
		}
		return stringToChar(str);
	}
	
	char * actionInputText() {
		input = psiServer_.findValue("input-text");
		return stringToChar(std::string("/index.html"));
	}
};

