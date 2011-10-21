#include <iostream>

#include "perl_lattice_writer_output.hpp"

PerlLatticeWriterOutput::PerlLatticeWriterOutput(AV * arrayPointer)
    : arrayPointer_(arrayPointer) {
}

void PerlLatticeWriterOutput::push(const std::string & textElement) {
    std::cerr << "Add element: " << textElement << std::endl;

    SV * element_sv = newSVpv(textElement.c_str(), 0);
    av_push( arrayPointer_, element_sv);

}

