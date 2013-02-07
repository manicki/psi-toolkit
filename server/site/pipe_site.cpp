#include "pipe_site.hpp"
#include "pipe_runner.hpp"
#include "logging.hpp"
#include "session_manager.hpp"
#include "main_factories_keeper.hpp"

#include <iostream>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>

const std::string PipeSite::GUESSING_READER = "guessing-reader";

PipeSite::PipeSite(PsiServer& server, const std::string & pipe, const std::string & text)
    : TemplateSite(server),
    initialText_(text.c_str()), initialPipe_(pipe.c_str()), initialOutput_(""),
    fileStorage_(std::string(psiServer_.websiteRoot))
{
    registerIncludesAndActions_();
}

void PipeSite::registerIncludesAndActions_() {
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
    std::string input = getOrSetDefaultData_("input-text", initialText_);
    return stringToChar(input);
}

char * PipeSite::pipeText() {
    std::string pipe = getOrSetDefaultData_("pipe-text", initialPipe_);
    return stringToChar(encodeHTML_(pipe));
}

char * PipeSite::outputText() {
    if (initialOutput_.empty()) {
        initialOutput_ = runPipe_(initialText_);
    }

    std::string output = getOrSetDefaultData_("output-text", initialOutput_);
    return stringToChar(generateOutput_(output));
}

char * PipeSite::actionPipe() {
    std::string input = getInput_();

    std::string output = runPipe_(input);
    psiServer_.session()->setData("output-text", output);

    return stringToChar(std::string("/psitoolkit.html"));
}

char * PipeSite::hiddenOptions() {
    std::string fileOnOff = psiServer_.session()->getData("radio-file");
    std::string outputFile = psiServer_.session()->getData("output-file");
    std::string outputType = psiServer_.session()->getData("output-type");

    std::string opts =
        std::string("psisOptions = {") +
        std::string("'isInputFile' : '") + fileOnOff + std::string("', ") +
        std::string("'fileToDownload' : '") + outputFile + std::string("', ") +
        std::string("'lastOutputType' : '") + outputType + std::string("' ") +
        std::string("};");

    psiServer_.session()->clearData("radio-file");

    return stringToChar(opts);
}

std::string PipeSite::getOrSetDefaultData_(const char* name, std::string initialValue) {
    if (!psiServer_.session()->isData(name)) {
        psiServer_.session()->setData(name, initialValue);
    }
    return psiServer_.session()->getData(name);
}

std::string PipeSite::getInput_() {
    std::string input = psiServer_.session()->getData("input-text");
    std::string isFile = psiServer_.session()->getData("radio-file");

    if (isFile == "on") {
        input = psiServer_.session()->getData("input-file");
        inputFromFile_ = true;
    }
    else {
        inputFromFile_ = false;
    }

    if (input.empty()) {
        input = initialText_;
    }

    return input;
}

std::string PipeSite::runPipe_(std::string input) {
    std::string pipe = psiServer_.session()->getData("pipe-text");

    boost::replace_all(pipe, " | ", " ! ");
    if (inputFromFile_ == true) {
        tryToAddGuessingReader_(pipe);
    }

    if (input.empty()) input = initialText_;

    std::stringstream iss(input);
    std::ostringstream oss;

    INFO("Constructing pipe [" << pipe << "]...");
    INFO("Input: " << input);

    clearPreviousFileFromOutput_();

    try {
        PipeRunner p(pipe);
        INFO("... running");
        p.run(iss, oss);
        INFO("... OK");

        createFileFromOutput_(oss.str());
    }
    catch (std::exception& e) {
        oss << "There are some problems: " << e.what() << std::endl
            << "Check the pipe-line specification and try once again.";
    }

    return oss.str();
}

void PipeSite::clearPreviousFileFromOutput_() {
    psiServer_.session()->clearData("output-file");
}

void PipeSite::createFileFromOutput_(const std::string& output) {
    std::string filename = fileStorage_.storeFile(output);
    psiServer_.session()->setData("output-file", filename);
}

void PipeSite::tryToAddGuessingReader_(std::string& pipe) {
    unsigned found = pipe.find("read");
    if (found != std::string::npos) {
        return;
    }

    std::set<std::string> aliases =
        MainFactoriesKeeper::getInstance().getAllAliases(GUESSING_READER);

    BOOST_FOREACH(std::string alias, aliases) {
        if (boost::starts_with(pipe, alias)) {
            return;
        }
    }

    DEBUG("add guessing-reader to pipeline");
    pipe = GUESSING_READER + " ! " + pipe;
}

std::string PipeSite::generateOutput_(const std::string& rawOutput) {
    std::ostringstream output;

    std::string type;
    std::string ext;
    fileRecognizer_.recognizeMimeTypeAndFileExtension(rawOutput, type, ext);

    if (type == "image") {
        if (ext == "svg") {
            output << rawOutput;
        }
        else {
            output << "<a href=\"" << psiServer_.session()->getData("output-file")
                << "\" target=\"_blank\" >"
                << "<img src=\"" << psiServer_.session()->getData("output-file")
                << "\" alt=\"image output\" />"
                << "</a>";
        }
    }
    if (type == "application") {
        output << "No output preview.";
    }
    if (type == "text") {
        output << "<pre>" << encodeHTML_(rawOutput) << "</pre>";
    }
    if (type == FileRecognizer::UNKNOWN_TYPE) {
        output << "<pre>" << rawOutput << "</pre>";
        output << "[output format not recognized]";
    }

    return output.str();
}

std::string PipeSite::encodeHTML_(const std::string& data) {
    std::string buffer;
    buffer.reserve(data.size());

    for(size_t pos = 0; pos != data.size(); ++pos) {
        switch(data[pos]) {
            case '&':  buffer.append("&amp;");      break;
            case '\"': buffer.append("&quot;");     break;
            case '\'': buffer.append("&apos;");     break;
            case '<':  buffer.append("&lt;");       break;
            case '>':  buffer.append("&gt;");       break;
            default:   buffer.append(1, data[pos]); break;
        }
    }

    return buffer;
}
