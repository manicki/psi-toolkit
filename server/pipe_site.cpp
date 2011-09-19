#include "pipe_site.hpp"
#include "pipe_runner.hpp"
#include "logging.hpp"

#include <iostream>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

const std::string PipeSite::INITIAL_TEXT = "Ala ma kota.";

PipeSite::PipeSite(PsiServer& server, std::string initialPipe)
    : TemplateSite(server), pipe(initialPipe)
{
    input = INITIAL_TEXT;
    output = runPipe(input);

    psiServer_.registerIncludeCode(
        "pipe_site_input_text", boost::bind(&PipeSite::inputText, this));
    psiServer_.registerActionCode(
        "input_text", boost::bind(&PipeSite::actionInputText, this));

    psiServer_.registerIncludeCode(
        "pipe_site_pipe_text", boost::bind(&PipeSite::pipeText, this));
    psiServer_.registerActionCode(
        "pipe_text", boost::bind(&PipeSite::actionPipeText, this));

    psiServer_.registerIncludeCode(
        "pipe_site_output_text", boost::bind(&PipeSite::outputText, this));
}

char * PipeSite::inputText() {
    return stringToChar(input);
}

char * PipeSite::actionInputText() {
    input = psiServer_.findValue("input-text");
    return stringToChar(std::string("/index.html"));
}

char * PipeSite::pipeText() {
    return stringToChar(pipe);
}

char * PipeSite::actionPipeText() {
    pipe = psiServer_.findValue("pipe-text");
    return stringToChar(std::string("/index.html"));
}

char * PipeSite::outputText() {
    std::string out = std::string("<pre>") + runPipe(input) + std::string("</pre>");
    return stringToChar(out);
}

std::string PipeSite::runPipe(std::string input) {
    if (input.empty())
        input = INITIAL_TEXT;            

    std::stringstream iss(input);
    std::ostringstream oss;

    INFO("Constructing pipe [" << pipe << "]...");
    INFO("Input is: " << input);

    try {
        PipeRunner p(pipe);
        INFO("... running");
        p.run(iss, oss);
        INFO("... OK");
    } 
    catch(std::exception& e) {
        oss << "There is some problem: " << e.what() << std::endl
            << "Check the pipe-line specification and try once again.";
    }

    return oss.str();
}

