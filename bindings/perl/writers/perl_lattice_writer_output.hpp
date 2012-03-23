#ifndef PERL_LATTICE_WRITER_OUTPUT_HDR
#define PERL_LATTICE_WRITER_OUTPUT_HDR

#include "abstract_simple_data_lattice_writer_output.hpp"

#if HAVE_PERL_BINDINGS

#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

typedef AV * Sink;

class PerlLatticeWriterOutput : public AbstractSimpleDataLatticeWriterOutput{
public:
    PerlLatticeWriterOutput(Sink & arrayPointer);

    void push(const std::string & textElement);
    void push(const AnnotationItem & element,
              AnnotationItemManager * latticeAnnotationItemManager);
    virtual void pushToArrayPointer_(ArrayPointer arrayPointer, ReferencePointer what);

    virtual long getCurrentArrayLength_() {
        long arrayLength = av_len(getPerlCurrentArrayPointer_()) + 1;
        return arrayLength;
    }

    SV * getCurrentArrayReference_();
    void closeSubArray_(bool flattenOneElement);

    bool isCurrentArrayEmpty_();
    long getCurrentArrayLength_();
    SV * tryToFlattenOneElementCurrentArray();

    AV * currentArrayPointer_;
    std::stack<AV *> arraysStack_;

    AnnotationItemManager * latticeAnnotationItemManager_;
};

#endif

#endif
