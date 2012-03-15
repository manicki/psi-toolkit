#include "nkjp_lattice_reader.hpp"

#include "logging.hpp"


std::string NKJPLatticeReader::getFormatName() {
    return "NKJP";
}

std::string NKJPLatticeReader::doInfo() {
    return "NKJP reader";
}

NKJPLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* NKJPLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& /*options*/
) {
    return new NKJPLatticeReader();
}

boost::program_options::options_description NKJPLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string NKJPLatticeReader::Factory::doGetName() {
    return "nkjp-reader";
}

boost::filesystem::path NKJPLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

NKJPLatticeReader::Worker::Worker(
    NKJPLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor)
{ }

void NKJPLatticeReader::Worker::doRun() {
    SET_LOGGING_LEVEL("DEBUG");
    std::string line;
    std::string input;
    while (std::getline(inputStream_, line)) {
        input += line + "\n";
    }
    DEBUG(input);
}
