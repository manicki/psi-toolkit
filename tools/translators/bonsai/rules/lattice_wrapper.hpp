#ifndef LATTICE_WRAPPER_HDR
#define LATTICE_WRAPPER_HDR

#include "lattice.hpp" 

bool isNonTerminal(Lattice::EdgeDescriptor edge, Lattice& lattice);

Lattice::EdgeSequence getWordTokenSequence(
    Lattice&,
    Lattice::VertexDescriptor,
    Lattice::VertexDescriptor,
    std::string);

Lattice::EdgeSequence getTopParseSequence(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end,
    std::string langCode);

std::map<int, int> getCharWordTokenMap(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end,
    std::string langCode);

Lattice::EdgeSequence getSubTreeSymbols(Lattice::EdgeDescriptor node,
                                        Lattice& lattice,
                                        std::string langCode);

Lattice::EdgeSequence getChildSymbols(Lattice::EdgeDescriptor node,
                                      Lattice& lattice,
                                      std::string langCode);

Lattice::EdgeSequence getTreeSymbols(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end,
    std::string langCode);

#endif
