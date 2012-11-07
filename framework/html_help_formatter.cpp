#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "html_help_formatter.hpp"
#include "logging.hpp"

#include "sundown/cpp/stringwrapper.hpp"

void HtmlHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    std::string description,
    boost::program_options::options_description options,
    std::list<std::string> aliases,
    std::vector<TestBatch> usingExamples,
    std::ostream& output) {

    output << "<div class=\"help-item\">"
        << "<h2 id=\"" << processorName << "\">" << processorName << "</h2>" << std::endl;

    if (description.size() != 0) {
        output << "<div class=\"help-desc\">" << markdownString2String(description)
            << "</div>" << std::endl;
    }

    if (!aliases.empty()) {
        formatAliases_(aliases, output);
    }

    if (!usingExamples.empty()) {
        formatUsingExamples_(usingExamples, output);
    }

    formatAllowedOptions_(options, output);
    output << "</div>" << std::endl;
}

void HtmlHelpFormatter::formatAliases_(std::list<std::string> aliases, std::ostream& output) {
    output << "<div class=\"help-alias\">"
        << "<h3>" << ALIASES_HEADER << "</h3>";

    BOOST_FOREACH(std::string alias, aliases) {
        output << alias << ", ";
    }

    output << "</div>" << std::endl;
}

void HtmlHelpFormatter::formatUsingExamples_(std::vector<TestBatch> batches, std::ostream& output) {
    output << "<div class=\"help-example\">"
        << "<h3>" << EXAMPLES_HEADER << "</h3>" << std::endl;

    for (unsigned int i = 0; i < batches.size(); i++) {
       output << "<code class=\"example-pipe\">"
            << escapeHTML_(batches[i].getPipeline()) << "</code>" << std::endl;

       output << "<div class=\"example-desc\">"
            << markdownString2String(batches[i].getDescription()) << "</div>" << std::endl;

        std::vector<TestRun> inOuts = batches[i].getTestRuns();
        for (unsigned int j = 0; j < inOuts.size(); j++) {
            output << "<div class=\"in-out\">" << std::endl;

            std::string fileContent = getFileContent(inOuts[j].getInputFilePath());
            output << "<div class=\"in\">in:</div>"
                << "<pre><code>" << escapeHTML_(fileContent) << "</code></pre>" << std::endl;

            fileContent = getFileContent(inOuts[j].getExpectedOutputFilePath());
            output << "<div class=\"out\">out:</div>"
                << "<pre><code>" << escapeHTML_(fileContent) << "</code></pre>" << std::endl;
            output << "</div>" << std::endl;
        }
    }

    output << "</div>" << std::endl;
}

void HtmlHelpFormatter::doFormatHelpIntroduction(std::string text, std::ostream& output) {
    output << markdownString2String(text) << std::endl;
}

void HtmlHelpFormatter::doFormatTutorial(std::string text, std::ostream& output) {
    output << markdownString2String(text) << std::endl;
}

void HtmlHelpFormatter::doFormatLicence(std::string text, std::ostream& output) {
    output << markdownString2String(text) << std::endl;
}

void HtmlHelpFormatter::formatPipelineExamplesInJSON(std::ostream& output) {
    std::vector<std::string> processors = MainFactoriesKeeper::getInstance().getProcessorNames();

    output << "var pipelineExamples = [" << std::endl;

    BOOST_FOREACH(std::string processorName, processors) {
        BOOST_FOREACH(TestBatch testBatch, getProcessorUsingExamples(processorName)) {
            formatPipelineExampleInJSON_(testBatch, output);
        };
    }

    output << "];" << std::endl;
}

void HtmlHelpFormatter::formatPipelineExampleInJSON_(TestBatch batch, std::ostream& output) {
    output << "  {" << std::endl;

    std::string description = batch.getDescription();
    output << "    \"description\" : \"" << escapeJSON_(description) << "\"," << std::endl;

    std::string pipe = batch.getPipeline();
    boost::algorithm::trim(pipe);
    output << "    \"pipe\" : \"" << escapeJSON_(pipe) << "\"," << std::endl;

    std::string text = getFileContent(batch.getTestRuns()[0].getInputFilePath());
    boost::algorithm::trim(text);
    output << "    \"text\" : \"" << escapeJSON_(text) << "\"" << std::endl;

    output << "  }, " << std::endl;
}

void HtmlHelpFormatter::formatAllowedOptions_(boost::program_options::options_description options,
    std::ostream& output) {

    output << "<div class=\"help-opts\">"
        << "<h3>" << OPTIONS_HEADER << "</h3>" << std::endl
        << "<pre>" << options << "</pre>" << std::endl
        << "</div>" << std::endl;
}

void HtmlHelpFormatter::doFormatOneAlias(
    std::string aliasName,
    std::list<std::string> processorNames,
    std::ostream& output) {

    if (processorNames.empty()) return;

    output << "<div class=\"alias-item\">" << aliasName << " &rarr; ";

    unsigned int i = 0;
    BOOST_FOREACH(std::string processorName, processorNames) {
        output << "<a href=\"/help/documentation.html#" << processorName << "\">"
               << processorName << "</a>";
        if (++i != processorNames.size()) output << ", ";
    }
    output << "</div>" << std::endl;
}

HtmlHelpFormatter::~HtmlHelpFormatter() { }

std::string HtmlHelpFormatter::escapeHTML_(const std::string& text) {
    std::string buffer;
    buffer.reserve(text.size());

    for(size_t pos = 0; pos != text.size(); ++pos) {
        switch(text[pos]) {
            case '&':  buffer.append("&amp;");      break;
            case '\"': buffer.append("&quot;");     break;
            case '\'': buffer.append("&apos;");     break;
            case '<':  buffer.append("&lt;");       break;
            case '>':  buffer.append("&gt;");       break;
            default:   buffer.append(1, text[pos]); break;
        }
    }

    return buffer;
}

std::string HtmlHelpFormatter::escapeJSON_(std::string& text) {
    boost::replace_all(text, "\\", "\\\\");
    boost::replace_all(text, "\"", "\\\"");
    boost::replace_all(text, "\n", "\\n");

    return text;
}
