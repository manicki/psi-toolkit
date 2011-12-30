#include "processor_factory.hpp"
#include "processor_file_fetcher.hpp"
#include "logging.hpp"

#include <sstream>
#include <fstream>

Processor* ProcessorFactory::createProcessor(const boost::program_options::variables_map& options) {
    return doCreateProcessor(options);
}

boost::program_options::options_description ProcessorFactory::optionsHandled() {
    return doOptionsHandled();
}

std::string ProcessorFactory::getContinuation(
    const boost::program_options::variables_map& options) const {

    return doGetContinuation(options);
}

double ProcessorFactory::getQualityScore(
    const boost::program_options::variables_map& options) const {

    return doGetQualityScore(options);
}

double ProcessorFactory::getEstimatedTime(
    const boost::program_options::variables_map& options) const {

    return doGetEstimatedTime(options);
}

double ProcessorFactory::doGetQualityScore(
    const boost::program_options::variables_map& /*options*/) const {

    return 0;
}

double ProcessorFactory::doGetEstimatedTime(
    const boost::program_options::variables_map& /*options*/) const {

    return -1.0;
}

boost::filesystem::path ProcessorFactory::getFile() {
    return doGetFile();
}


std::string ProcessorFactory::getName() {
    return doGetName();
}

std::string ProcessorFactory::getDescription() {
    return doGetDescription();
}

std::string ProcessorFactory::doGetDescription() {
    std::string description("");

    ProcessorFileFetcher fileFetcher(doGetFile());
    try {
        boost::filesystem::path pathToDescriptionFile
            = fileFetcher.getOneFile("%ITSDATA%/description.txt");
        description = getFileContent(pathToDescriptionFile);
    }
    catch (FileFetcher::Exception& err) {
        WARN("An error occured when trying to open the processor's description file: "
             << err.what());
    }

    return description;
}

std::string ProcessorFactory::getFileContent(boost::filesystem::path path) {

    std::stringstream content;
    content << std::ifstream( path.string().c_str() ).rdbuf();

    return content.str();
}

ProcessorFactory::~ProcessorFactory() {
}
