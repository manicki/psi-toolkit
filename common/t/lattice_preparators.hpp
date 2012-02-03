#ifndef LATTICE_PREPARATORS_HDR
#define LATTICE_PREPARATORS_HDR

#include "lattice.hpp"


namespace lattice_preparators {

    void prepareSimpleLattice(Lattice & lattice);
    void prepareAdvancedLattice(Lattice & lattice);
    void prepareRegularLattice(Lattice & lattice);
    void prepareLatticeWithOneSymbolTokens(Lattice & lattice, std::string text);

}

#endif
