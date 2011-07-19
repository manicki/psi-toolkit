#include "psi_lattice_reader.hpp"


void PsiLatticeReader::readIntoLattice(std::istream& inputStream, Lattice& lattice) {
    PsiLRGrammar grammar;
    std::string line;
    while (std::getline(inputStream, line)) {
        PsiLRItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {

            item.unescape();

std::cerr << item.ordinal << " " << item.text << std::endl;

        }
    }
}

std::string PsiLatticeReader::getFormatName() {
    return "Psi";
}
