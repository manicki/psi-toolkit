#ifndef HELP_FORMATTER_HDR
#define HELP_FORMATTER_HDR

#include <vector>

#include <iostream>
#include <boost/program_options/options_description.hpp>

#include "main_factories_keeper.hpp"
#include "test_extractor.hpp"

class HelpFormatter {

public:

    void formatHelps(std::ostream& output);
    void formatOneProcessorHelp(std::string processorName, std::ostream& output);

    void formatOneAlias(std::string aliasName, std::ostream& output);
    void formatAliases(std::ostream& output);

    void formatHelpIntroduction(std::ostream& output);
    void formatTutorial(std::ostream& output);

    virtual ~HelpFormatter();

protected:

    TestExtractor testExtractor_;

    virtual void doFormatOneProcessorHelp(
        std::string processorName,
        std::string description,
        boost::program_options::options_description options,
        std::vector<TestBatch> usingExamples,
        std::ostream& output) =0;

    std::string getProcessorDescription(std::string processorName);
    boost::program_options::options_description getProcessorOptions(std::string processorName);
    std::vector<TestBatch> getProcessorUsingExamples(std::string processorName);

    virtual void doFormatOneAlias(
        std::string aliasName,
        std::list<std::string> processorNames,
        std::ostream& output) =0;

    std::list<std::string> getProcessorNamesForAlias(std::string alias);

    virtual void doFormatHelpIntroduction(std::string text, std::ostream& output) =0;
    virtual void doFormatTutorial(std::string text, std::ostream& output) =0;

    boost::filesystem::path getPathToFrameworkDataFile_(const std::string& filename);
    std::string getFileContent(const boost::filesystem::path& path);

    const static std::string EXAMPLES_HEADER;
    const static std::string OPTIONS_HEADER;
    const static std::string ALIASES_HEADER;
};

#endif
