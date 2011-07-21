#include "psi_lattice_reader.hpp"


std::string PsiLatticeReader::getFormatName() {
    return "Psi";
}

std::string PsiLatticeReader::doInfo() {
    return "Psi reader";
}

void PsiLatticeReader::doReadIntoLattice(std::istream& inputStream, Lattice& lattice) {
    //TODO
}
