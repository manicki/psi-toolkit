#include "txt_lattice_reader.hpp"


std::string TxtLatticeReader::getFormatName() {
    return "TXT";
}

std::string TxtLatticeReader::doInfo() {
    return "plain text reader";
}

void TxtLatticeReader::doReadIntoLattice(std::istream& inputStream, Lattice& lattice) {

    std::string line;
    while (getline(inputStream, line)) {
        appendParagraphToLattice_(line, lattice);
    }
}

void TxtLatticeReader::appendParagraphToLattice_(std::string paragraph, Lattice& lattice) {
    lattice.appendString(paragraph);
}
