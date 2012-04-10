#include <sstream>
#include <fstream>

#include <boost/foreach.hpp>

#include "help_formatter.hpp"
#include "its_data.hpp"
#include "configurator.hpp"

void HelpFormatter::formatHelps(std::ostream& output) {
    std::vector<std::string> processors = MainFactoriesKeeper::getInstance().getProcessorNames();

    BOOST_FOREACH(std::string processorName, processors) {
        output << std::endl;
        formatOneProcessorHelp(processorName, output);
    }
}

void HelpFormatter::formatOneProcessorHelp(std::string processorName, std::ostream& output) {
    doFormatOneProcessorHelp(
        processorName,
        getProcessorDescription(processorName),
        getProcessorOptions(processorName),
        getProcessorUsingExamples(processorName),
        output
    );
}

void HelpFormatter::formatAliases(std::ostream& output) {
    std::set<std::string> aliases = MainFactoriesKeeper::getInstance().getAliasNames();

    BOOST_FOREACH(std::string alias, aliases) {
        formatOneAlias(alias, output);
    }
}

void HelpFormatter::formatOneAlias(std::string aliasName, std::ostream& output) {
    doFormatOneAlias(aliasName, getProcessorNamesForAlias(aliasName), output);
}

void HelpFormatter::formatHelpIntroduction(std::ostream& output) {
    boost::filesystem::path path = Configurator::getInstance().isRunAsInstalled()
        ? Configurator::getInstance().getDataDir().string() + "/framework/description.txt"
        : "../framework/data/description.txt";

    doFormatHelpIntroduction(getFileContent(path), output);
}

HelpFormatter::~HelpFormatter() { }

std::string HelpFormatter::getProcessorDescription(std::string processorName) {
    return MainFactoriesKeeper::getInstance().getProcessorFactory(processorName).getDescription();
}

boost::program_options::options_description HelpFormatter::getProcessorOptions(
    std::string processorName) {
    return MainFactoriesKeeper::getInstance().getProcessorFactory(processorName).optionsHandled();
}

std::vector<TestBatch> HelpFormatter::getProcessorUsingExamples(std::string processorName) {
    boost::filesystem::path processorFile =
        MainFactoriesKeeper::getInstance().getProcessorFactory(processorName).getFile();

    std::vector<boost::filesystem::path> directories;
    directories.push_back(getItsData(processorFile));

    testExtractor_.clearTestBatches();
    testExtractor_.lookForTestBatches(directories, "help");

    return testExtractor_.getTestBatches();
}

std::string HelpFormatter::getFileContent(const boost::filesystem::path& path) {
    std::stringstream content;
    content << std::ifstream( path.string().c_str() ).rdbuf();

    return content.str();
}

std::list<std::string> HelpFormatter::getProcessorNamesForAlias(std::string alias) {
    std::list<ProcessorFactory*> processors =
        MainFactoriesKeeper::getInstance().getProcessorFactoriesForName(alias);

    std::list<std::string> processorNames;
    BOOST_FOREACH(ProcessorFactory* processor, processors) {
        processorNames.push_front(processor->getName());
    }

    return processorNames;
}

const std::string HelpFormatter::EXAMPLES_HEADER = "Examples";
const std::string HelpFormatter::OPTIONS_HEADER = "Options";
const std::string HelpFormatter::ALIASES_HEADER = "Aliases";
