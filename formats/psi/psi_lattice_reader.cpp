#include "psi_lattice_reader.hpp"


std::string PsiLatticeReader::getFormatName() {
    return "Psi";
}

std::string PsiLatticeReader::doInfo() {
    return "Psi reader";
}

void PsiLatticeReader::readIntoLattice(std::istream& inputStream, Lattice& lattice) {
    //TODO
}
