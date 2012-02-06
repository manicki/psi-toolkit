#include "help_formatter.hpp"

void HelpFormatter::formatOneProcessorHelp(
    std::string processorName, std::ostream& output) {

    doFormatOneProcessorHelp(
        processorName,
        getProcessorDescription(processorName),
        getProcessorOptions(processorName),
        getProcessorUsingExamples(processorName),
        output
    );
}

HelpFormatter::~HelpFormatter() { }

void HelpFormatter::formatHelps(std::ostream& output) {
    std::vector<std::string> processors = MainFactoriesKeeper::getInstance().getProcessorNames();

    for (unsigned int i = 0; i < processors.size(); i++) {
        output << std::endl;
        formatOneProcessorHelp(processors[i], output);
    }
}

std::string HelpFormatter::getProcessorDescription(std::string processorName) {
    return MainFactoriesKeeper::getInstance().getProcessorFactory(processorName).getDescription();
}

boost::program_options::options_description HelpFormatter::getProcessorOptions(
    std::string processorName) {
    return MainFactoriesKeeper::getInstance().getProcessorFactory(processorName).optionsHandled();
}

std::vector<TestBatch> HelpFormatter::getProcessorUsingExamples(std::string processorName) {
    boost::filesystem::path processorFile = MainFactoriesKeeper::getInstance()
        .getProcessorFactory(processorName).getFile();

    std::vector<boost::filesystem::path> directories;
    directories.push_back(processorFile.parent_path());

    testExtractor_.lookForTestBatches(directories, "help");

    return testExtractor_.getTestBatches();
}
