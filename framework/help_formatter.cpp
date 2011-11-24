#include "help_formatter.hpp"

void HelpFormatter::formatOneProcessorHelp(
    std::string processorName, std::ostream& output)
{
    formatOneProcessorHelp(processorName, getProcessorOptions(processorName), output);
}

void HelpFormatter::formatHelps(std::ostream& output)
{
    std::vector<std::string> processors = MainFactoriesKeeper::getInstance().getProcessorNames();

    for (unsigned int i = 0; i < processors.size(); i++) {
        output << processors[i] << std::endl;
        formatOneProcessorHelp(processors[i], output);
    }
}

boost::program_options::options_description HelpFormatter::getProcessorOptions(
    std::string processorName)
{
    return MainFactoriesKeeper::getInstance().getProcessorFactory(processorName).optionsHandled();
}
