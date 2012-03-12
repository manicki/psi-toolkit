#include "pipe_site.hpp"
#include "pipe_runner.hpp"
#include "logging.hpp"
#include "session_manager.hpp"

#include <iostream>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

PipeSite::PipeSite(PsiServer& server, const std::string & pipe)
    : TemplateSite(server),
    initialText("Ala ma kota"), initialPipe(pipe.c_str()), initialOutput(""),
    outputSaver_(std::string(psiServer_.websiteRoot))
{
    registerIncludesAndActions();
}

void PipeSite::registerIncludesAndActions() {
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
    std::string input = getOrSetDefaultData("input-text", initialText);
    return stringToChar(input);
}

char * PipeSite::pipeText() {
    std::string pipe = getOrSetDefaultData("pipe-text", initialPipe);
    return stringToChar(pipe);
}

char * PipeSite::outputText() {
    if (initialOutput.empty()) {
        initialOutput = runPipe(initialText);
    }

    std::string output = getOrSetDefaultData("output-text", initialOutput);
    return stringToChar(generateOutput_(output));
}

char * PipeSite::actionPipe() {
    std::string input = getInput();

    std::string output = runPipe(input);
    psiServer_.session()->setData("output-text", output);

    return stringToChar(std::string("/index.html"));
}

char * PipeSite::hiddenOptions() {
    std::string fileOnOff = psiServer_.session()->getData("radio-file");
    std::string outputFile = psiServer_.session()->getData("output-file");

    std::string opts =
        std::string("psisOptions = {") +
        std::string("'isInputFile' : '") + fileOnOff + std::string("', ") +
        std::string("'fileToDownload' : '") + outputFile + std::string("' ") +
        std::string("};");

    psiServer_.session()->clearData("radio-file");

    return stringToChar(opts);
}

std::string PipeSite::getOrSetDefaultData(const char* name, std::string initialValue) {
    if (!psiServer_.session()->isData(name)) {
        psiServer_.session()->setData(name, initialValue);
    }
    return psiServer_.session()->getData(name);
}

std::string PipeSite::getInput() {
    std::string input = psiServer_.session()->getData("input-text");
    std::string isFile = psiServer_.session()->getData("radio-file");

    if (isFile == "on") {
        input = psiServer_.session()->getData("input-file");
    }

    if (input.empty()) {
        input = initialText;
    }

    return input;
}

std::string PipeSite::runPipe(std::string input) {
    std::string pipe = psiServer_.session()->getData("pipe-text");

    if (input.empty())
        input = initialText;

    std::stringstream iss(input);
    std::ostringstream oss;

    INFO("Constructing pipe [" << pipe << "]...");
    INFO("Input: " << input);

    clearPreviousFileFromOutput();

    try {
        PipeRunner p(pipe);
        INFO("... running");
        p.run(iss, oss);
        INFO("... OK");

        createFileFromOutput(oss.str());
    }
    catch (std::exception& e) {
        oss << "There are some problems: " << e.what() << std::endl
            << "Check the pipe-line specification and try once again.";
    }

    return oss.str();
}

void PipeSite::clearPreviousFileFromOutput() {
    psiServer_.session()->clearData("output-file");
}

void PipeSite::createFileFromOutput(const std::string& output) {
    std::string filename = outputSaver_.storeOutput(output);
    psiServer_.session()->setData("output-file", filename);
}

std::string PipeSite::generateOutput_(const std::string& rawOutput) {
    std::ostringstream output;

    std::string type = fileRecognizer_.recognizeType(rawOutput);

    if (type == "image") {
        if (fileRecognizer_.recognizeExtension(rawOutput) == "svg") {
            output << rawOutput;
        }
        else {
            output << "<a href=\"" << psiServer_.session()->getData("output-file") << "\">"
                << "<img src=\"" << psiServer_.session()->getData("output-file")
                << "\" alt=\"image output\" />"
                << "</a>";
        }
    }
    if (type == "application") {
        output << "No output preview.";
    }
    if (type == "text") {
        output << "<pre>" << rawOutput << "</pre>";
    }
    if (type == FileRecognizer::UNKNOWN_TYPE) {
        output << "<pre>" << rawOutput << "</pre>";
        output << "[output format not recognized]";
    }

    return output.str();
}
