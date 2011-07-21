#include "txt_lattice_reader.hpp"


std::string TxtLatticeReader::getFormatName() {
    return "TXT";
}

std::string TxtLatticeReader::doInfo() {
    return "plain text reader";
}

LatticeReader* TxtLatticeReader::Factory::doCreateLatticeReader(
    boost::program_options::variables_map options) {
    return new TxtLatticeReader();
}

boost::program_options::options_description doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    return optionsDescription;
}

std::string TxtLatticeReader::Factory::doGetName() {
    return "txt-reader";
}


TxtLatticeReader::Worker::Worker(TxtLatticeReader& processor,
                                 std::istream& inputStream,
                                 Lattice& lattice):
    ReaderWorker(inputStream, lattice), processor_(processor) {
}

void TxtLatticeReader::Worker::doRun() {
    std::string line;
    while (getline(inputStream_, line)) {
        appendParagraphToLattice_(line);
    }
}

void TxtLatticeReader::Worker::appendParagraphToLattice_(std::string paragraph) {
    lattice_.appendString(paragraph);
}
