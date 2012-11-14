#ifndef HTML_HELP_FORMATTER_HDR
#define HTML_HELP_FORMATTER_HDR

#include "help_formatter.hpp"
#include "file_storage.hpp"
#include "file_recognizer.hpp"

class HtmlHelpFormatter : public HelpFormatter {

public:
    ~HtmlHelpFormatter();

    void formatPipelineExamplesInJSON(std::ostream& output);
    void formatDocumentationMenu(std::ostream& output);

    /*
     * If you pass FileStorage, some example inputs and outputs in untextual format
     * (such as .rtf, .doc, .jpg etc.) will be displayed as files to download.
     */
    void setFileStorage(FileStorage* fileStorage);
    void unsetFileStorage();

private:

    FileStorage* fileStorage_;
    FileRecognizer fileRecognizer_;

    void doFormatOneProcessorHelp(
        std::string processorName,
        std::string description,
        boost::program_options::options_description options,
        std::list<std::string> aliases,
        std::vector<TestBatch> usingExamples,
        std::ostream& output);

    void formatAliases_(std::list<std::string> aliases, std::ostream& output);

    void formatUsingExamples_(std::vector<TestBatch> batches, std::ostream& output);
    void formatExampleInputOutput_(
        boost::filesystem::path filePath,
        std::ostream& output,
        std::string divClass);

    void formatAllowedOptions_(boost::program_options::options_description options,
        std::ostream& output);

    void doFormatOneAlias(
        std::string aliasName,
        std::list<std::string> processorNames,
        std::ostream& output);

    void doFormatDataFile(std::string text, std::ostream& output);

    void formatPipelineExampleInJSON_(TestBatch batch, std::ostream& output);

    std::string escapeHTML_(const std::string& text);
    std::string escapeJSON_(std::string& text);
};

#endif
