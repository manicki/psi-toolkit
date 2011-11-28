#include "help_formatter.hpp"

void HelpFormatter::formatOneProcessorHelp(
    std::string processorName, std::ostream& output)
{
    doFormatOneProcessorHelp(processorName, getProcessorOptions(processorName), output);
}

HelpFormatter::~HelpFormatter() { }

void HelpFormatter::formatHelps(std::ostream& output)
{
    std::vector<std::string> processors = MainFactoriesKeeper::getInstance().getProcessorNames();

    for (unsigned int i = 0; i < processors.size(); i++) {
        output << std::endl;
        formatOneProcessorHelp(processors[i], output);
    }
}

boost::program_options::options_description HelpFormatter::getProcessorOptions(
    std::string processorName)
{
    return MainFactoriesKeeper::getInstance().getProcessorFactory(processorName).optionsHandled();
}
