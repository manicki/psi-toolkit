#ifndef PERL_LATTICE_WRITER_OUTPUT_HDR
#define PERL_LATTICE_WRITER_OUTPUT_HDR

#include <string>
#include "config.h"

#if HAVE_PERL_BINDINGS
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#endif

class PerlLatticeWriterOutput {
public:
    PerlLatticeWriterOutput(AV * arrayPointer);

    void push(const std::string & textElement);
private:
    AV * arrayPointer_;
};

#endif
