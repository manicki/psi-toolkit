#include <string>
#include <iostream>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include "template_site.hpp"
#include "pipe_runner.hpp"

class PipeSite : public TemplateSite
{

public:

	std::string pipe;
	std::string input;
	std::string output;

	PipeSite(PsiServer& server, std::string initialPipe) 
		: TemplateSite(server), pipe(initialPipe)
	{
		output = runPipe(std::string("Ala ma kota"));

		psiServer_.registerIncludeCode(
			"pipe_site_input_text", boost::bind(&PipeSite::inputText, this));
		psiServer_.registerActionCode(
			"input_text", boost::bind(&PipeSite::actionInputText, this));

		psiServer_.registerIncludeCode(
			"pipe_site_pipe_text", boost::bind(&PipeSite::pipeText, this));
		psiServer_.registerActionCode(
			"pipe_text", boost::bind(&PipeSite::actionPipeText, this));
	#include <boost/algorithm/string.hpp>

		psiServer_.registerIncludeCode(
			"pipe_site_output_text", boost::bind(&PipeSite::outputText, this));
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

	char * pipeText() {
		return stringToChar(pipe);
	}
	
	char * actionPipeText() {
		pipe = psiServer_.findValue("pipe-text");
		return stringToChar(std::string("/index.html"));
	}

	char * outputText() {
		std::string out = runPipe(input);
		return stringToChar(out);
	}	

	std::string runPipe(std::string input) {
		std::stringstream iss;
		std::ostringstream oss;

		std::vector<std::string> sp = splittedPipe();
		PipeRunner p(sp);
		p.run(iss, oss);
		
		iss << input << std::endl;
		return oss.str();
	}

	std::vector<std::string> splittedPipe() {
		std::vector<std::string> strs;
		boost::split(strs, pipe, boost::is_any_of(" "));
		return strs;
	}

};

