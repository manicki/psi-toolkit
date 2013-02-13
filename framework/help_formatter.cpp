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
        getAliasesForProcessorName(processorName),
        getProcessorUsingExamples(processorName),
        output
    );
}

bool HelpFormatter::formatProcessorHelpsByName(std::string aliasOrProcessorName,
                                               std::ostream& output) {

    std::vector<std::string> processors = MainFactoriesKeeper::getInstance().getProcessorNames();
    bool isProcessor = (std::find(processors.begin(), processors.end(), aliasOrProcessorName)
                            != processors.end());

    if (isProcessor) {
        formatOneProcessorHelp(aliasOrProcessorName, output);
        return true;
    }

    std::set<std::string> aliases = MainFactoriesKeeper::getInstance().getAliasNames();
    bool isAlias = aliases.count(aliasOrProcessorName);

    if (isAlias) {
        BOOST_FOREACH(std::string name, getAliasesForProcessorName(aliasOrProcessorName)) {
            formatOneProcessorHelp(name, output);
        }
        return true;
    }

    return false;
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

void HelpFormatter::formatDescription(std::ostream& output) {
    boost::filesystem::path path = getPathToFrameworkDataFile_("description.txt");

    doFormatDataFile(getFileContent(path), output);
}

void HelpFormatter::formatTutorial(std::ostream& output) {
    boost::filesystem::path path = getPathToFrameworkDataFile_("tutorial.txt");

    doFormatDataFile(getFileContent(path), output);
}

void HelpFormatter::formatLicence(std::ostream& output) {
    boost::filesystem::path path = getPathToFrameworkDataFile_("licence.txt");

    doFormatDataFile(getFileContent(path), output);
}

void HelpFormatter::formatAboutPsiFormat(std::ostream& output) {
    boost::filesystem::path path = getPathToFrameworkDataFile_("psi-format.txt");

    doFormatDataFile(getFileContent(path), output);
}

HelpFormatter::~HelpFormatter() { }

boost::filesystem::path HelpFormatter::getPathToFrameworkDataFile_(const std::string& filename) {
    return Configurator::getInstance().isRunAsInstalled()
        ? Configurator::getInstance().getDataDir().string() + "/framework/" + filename
        : "../framework/data/" + filename;
}

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

    processorNames.sort();
    return processorNames;
}

std::list<std::string> HelpFormatter::getAliasesForProcessorName(std::string processorName) {
    std::set<std::string> aliases =
        MainFactoriesKeeper::getInstance().getAllAliases(processorName);
    return std::list<std::string>(aliases.begin(), aliases.end());
}

const std::string HelpFormatter::EXAMPLES_HEADER = "Examples";
const std::string HelpFormatter::OPTIONS_HEADER = "Options";
const std::string HelpFormatter::ALIASES_HEADER = "Aliases";
