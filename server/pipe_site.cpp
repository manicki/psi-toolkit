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
    fileName = "";

    psiServer_.registerIncludeCode(
        "pipe_site_input_text", boost::bind(&PipeSite::inputText, this));

    psiServer_.registerIncludeCode(
        "pipe_site_pipe_text", boost::bind(&PipeSite::pipeText, this));

    psiServer_.registerActionCode(
        "pipe", boost::bind(&PipeSite::actionPipe, this));

    psiServer_.registerIncludeCode(
        "pipe_site_output_text", boost::bind(&PipeSite::outputText, this));

    psiServer_.registerIncludeCode(
        "pipe_site_options", boost::bind(&PipeSite::hiddenOptions, this));
}

char * PipeSite::inputText() {
    return stringToChar(input);
}

char * PipeSite::pipeText() {
    return stringToChar(pipe);
}

char * PipeSite::actionPipe() {
    input = psiServer_.findValue("input-text");
    pipe = psiServer_.findValue("pipe-text");

    fileName = "";

    std::string in = input;

    std::string is_file = psiServer_.findValue("radio-file");
    if (is_file == "on") {
        fileName = psiServer_.findValue("input-file-filename");
        in = psiServer_.findValue("input-file");
    }

    output = runPipe(in);

    return stringToChar(std::string("/index.html"));
}

char * PipeSite::outputText() {
    std::string out = std::string("<pre>") + output + std::string("</pre>");
    return stringToChar(out);
}

char * PipeSite::hiddenOptions() {
    std::string is_input_file_on = fileName.empty() ? "" : "on";
    std::string opts =
        std::string("<div input_file=\"") + is_input_file_on
        + std::string("\" />");

    return stringToChar(opts);
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
        oss << "There are some problems: " << e.what() << std::endl
            << "Check the pipe-line specification and try once again.";
    }

    return oss.str();
}

