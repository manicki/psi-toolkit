#include "txt_lattice_reader.hpp"


std::string TxtLatticeReader::getFormatName() {
    return "TXT";
}

std::string TxtLatticeReader::doInfo() {
    return "plain text reader";
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
