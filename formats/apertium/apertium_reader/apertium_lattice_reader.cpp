#include <boost/algorithm/string/trim.hpp>

#include "apertium_lattice_reader.hpp"


std::string ApertiumLatticeReader::getFormatName() {
    return "Apertium";
}
std::string ApertiumLatticeReader::doInfo() {
    return "Apertium reader";
}


const std::string ApertiumLatticeReader::Factory::DEFAULT_SPEC_FILES_DIR = "%ITSDATA%/";

ApertiumLatticeReader::ApertiumLatticeReader(const boost::filesystem::path& specificationFile)
    : specificationFile_(specificationFile) {

    SET_LOGGING_LEVEL("DEBUG");
}

ApertiumLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* ApertiumLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& options) {

    boost::filesystem::path specificationFile;

    if (options.count("specification-file")) {
        specificationFile = options["specification-file"].as<std::string>();
    }
    else {
        specificationFile = DEFAULT_SPEC_FILES_DIR + "/"
            + options["file-type"].as<std::string>() + ".xml";
    }

    return new ApertiumLatticeReader(specificationFile);
}

boost::program_options::options_description ApertiumLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("file-type", boost::program_options::value<std::string>()
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
        input += line;
    }
    INFO(input);
}
