#include <boost/algorithm/string/trim.hpp>

#include "apertium_lattice_reader.hpp"
#include "processor_file_fetcher.cpp"


std::string ApertiumLatticeReader::getFormatName() {
    return "Apertium";
}

std::string ApertiumLatticeReader::doInfo() {
    return "Apertium reader";
}

std::string ApertiumLatticeReader::deformat(const std::string& input) {
    return apertiumDeformatter_.deformat(input);
}


const std::string ApertiumLatticeReader::Factory::DEFAULT_SPEC_FILES_DIR = "%ITSDATA%/";
const std::string ApertiumLatticeReader::Factory::DEFAULT_SPEC_FILE_ENDING = "-format.xml";

ApertiumLatticeReader::ApertiumLatticeReader(const boost::filesystem::path& specificationFile)
    : apertiumDeformatter_(specificationFile) {

    SET_LOGGING_LEVEL("DEBUG"); //FIXME
}

ApertiumLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* ApertiumLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& options) {

    std::string specFilePath;

    if (options.count("specification-file")) {
        specFilePath = options["specification-file"].as<std::string>();
    }
    else {
        specFilePath = DEFAULT_SPEC_FILES_DIR + "/"
            + options["format"].as<std::string>() + DEFAULT_SPEC_FILE_ENDING;
    }

    ProcessorFileFetcher fileFetcher(__FILE__);
    boost::filesystem::path specificationFile = fileFetcher.getOneFile(specFilePath);

    return new ApertiumLatticeReader(specificationFile);
}

boost::program_options::options_description ApertiumLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("format", boost::program_options::value<std::string>()
            ->default_value("xhtml"),
            "type of file to deformatting")
        ("specification-file", boost::program_options::value<std::string>(),
            "specification file path");

    return optionsDescription;
}

std::string ApertiumLatticeReader::Factory::doGetName() {
    return "apertium-reader";
}

boost::filesystem::path ApertiumLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

ApertiumLatticeReader::Worker::Worker(ApertiumLatticeReader& processor,
                                      std::istream& inputStream,
                                      Lattice& lattice)
    : ReaderWorker<std::istream>(inputStream, lattice), processor_(processor) {
}

void ApertiumLatticeReader::Worker::doRun() {
    DEBUG("starting apertium reader...");

    std::string input;
    std::string line;

    while (std::getline(inputStream_, line)) {
        if (boost::algorithm::trim_copy(line).empty()) {
            continue;
        }
        input += line + "\n";
    }

    std::string output = processor_.deformat(input);
    DEBUG(output);
}
