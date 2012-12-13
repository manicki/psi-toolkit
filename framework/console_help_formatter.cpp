#include <boost/algorithm/string/join.hpp>

#include "console_help_formatter.hpp"

void ConsoleHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    std::string description,
    boost::program_options::options_description options,
    std::list<std::string> aliases,
    std::vector<TestBatch> usingExamples,
    std::ostream& output) {

    output << processorName << std::endl << description << std::endl;

    if (!aliases.empty()) {
        formatAliases_(aliases, output);
    }

    if (!usingExamples.empty())
        output << "To see one of the " << usingExamples.size()
            << " examples check installed files or visit PSI website."
            << std::endl << std::endl;

    output << options;
}

void ConsoleHelpFormatter::formatAliases_(std::list<std::string> aliases, std::ostream& output) {
    output << "Aliases: ";
    BOOST_FOREACH(std::string alias, aliases) {
        output << alias << ", ";
    }
    output << std::endl;
}

void ConsoleHelpFormatter::doFormatOneAlias(
    std::string aliasName,
    std::list<std::string> processorNames,
    std::ostream& output) {

    if (processorNames.empty()) return;

    output << aliasName << " -> " << boost::algorithm::join(processorNames, ", ") << std::endl;
}

void ConsoleHelpFormatter::doFormatDataFile(std::string text, std::ostream& output) {
    output << text << std::endl;
}

ConsoleHelpFormatter::~ConsoleHelpFormatter() { }
