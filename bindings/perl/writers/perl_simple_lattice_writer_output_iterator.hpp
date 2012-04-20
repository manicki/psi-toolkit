#ifndef PERL_SIMPLE_LATTICE_WRITER_OUTPUT_ITERATOR_HDR
#define PERL_SIMPLE_LATTICE_WRITER_OUTPUT_ITERATOR_HDR

#include "config.hpp"

#if HAVE_PERL_BINDINGS

#include <string>
#include "lattice_writer_output_iterator.hpp"
#include "annotation_item.hpp"
#include "perl_lattice_writer_output.hpp"

#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

/**
 * Writes simple lattice writer's output to the stream.
 */
class PerlSimpleLatticeWriterOutputIterator : public LatticeWriterOutputIterator {
public:
    PerlSimpleLatticeWriterOutputIterator(
                                          Sink & output,
                                          bool withArgs
    );

    virtual ~PerlSimpleLatticeWriterOutputIterator();

private:
    PerlLatticeWriterOutput outputWrapper_;

    virtual void doPutElement(const AnnotationItem& element);

    virtual void doOpenAlternative();

    virtual void doCloseAlternative();

    virtual void doOpenGroup(const std::string& group);

    virtual void doCloseGroup(const std::string& group);

    virtual void doFlush();

    bool withArgs_;
};
#endif

#endif
