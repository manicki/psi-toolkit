#include "apertium_lattice_reader.hpp"

std::string ApertiumLatticeReader::getFormatName() {
    return "Apertium";
}
std::string ApertiumLatticeReader::doInfo() {
    return "Apertium reader";
}

ApertiumLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* ApertiumLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map&) {

    return new ApertiumLatticeReader();
}

boost::program_options::options_description ApertiumLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}

std::string ApertiumLatticeReader::Factory::doGetName() {
    return "apertium-reader";
}

boost::filesystem::path ApertiumLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

ApertiumLatticeReader::Worker::Worker(ApertiumLatticeReader& processor, std::istream& inputStream,
    Lattice& lattice) : ReaderWorker<std::istream>(inputStream, lattice), processor_(processor) {
}

void ApertiumLatticeReader::Worker::doRun() {
    SET_LOGGING_LEVEL("DEBUG");
    DEBUG("ApertiumLatticeReader is working!");
}
