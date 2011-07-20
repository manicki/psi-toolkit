#include "txt_lattice_reader.hpp"

std::string TxtLatticeReader::getFormatName() {
    return "TXT";
}

std::string TxtLatticeReader::doInfo() {
    return "plain text reader";
}

void TxtLatticeReader::doReadIntoLattice(std::istream& inputStream, Lattice& lattice) {
    ;
}
