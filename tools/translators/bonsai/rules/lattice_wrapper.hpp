#ifndef LATTICE_WRAPPER_HDR
#define LATTICE_WRAPPER_HDR

#include "lattice.hpp" 

bool isNonTerminal(Lattice::EdgeDescriptor edge, Lattice& lattice);

Lattice::EdgeSequence getWordTokenSequence(
    Lattice&,
    Lattice::VertexDescriptor,
    Lattice::VertexDescriptor);

Lattice::EdgeSequence getTopParseSequence(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end);

std::map<int, int> getCharWordTokenMap(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end);

Lattice::EdgeSequence getSubTreeSymbols(Lattice::EdgeDescriptor node,
                                        Lattice& lattice);

Lattice::EdgeSequence getChildSymbols(Lattice::EdgeDescriptor node,
                                      Lattice& lattice);

Lattice::EdgeSequence getTreeSymbols(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end);

#endif
