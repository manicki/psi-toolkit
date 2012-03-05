#ifndef PERL_LATTICE_WRITER_OUTPUT_HDR
#define PERL_LATTICE_WRITER_OUTPUT_HDR

#include "abstract_simple_data_lattice_writer_output.hpp"

#if HAVE_PERL_BINDINGS

#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

typedef HV* PerlHashPointer;
typedef AV* PerlArrayPointer;
typedef SV* PerlReference;

class PerlLatticeWriterOutput : public AbstractSimpleDataLatticeWriterOutput{
public:
    PerlLatticeWriterOutput(AV * arrayPointer);
protected:
    virtual void pushToCurrentArrayPointer_(const std::string & textElement);
    virtual void pushToCurrentArrayPointer_(const AnnotationItem & element,
              AnnotationItemManager * latticeAnnotationItemManager);
    virtual void pushToArrayPointer_(ArrayPointer arrayPointer, ReferencePointer what);

    virtual long getCurrentArrayLength_() {
        long arrayLength = av_len(getPerlCurrentArrayPointer_()) + 1;
        return arrayLength;
    }

    virtual ReferencePointer getCurrentArrayReference_() {
        return newRV_inc((PerlReference) getPerlCurrentArrayPointer_());
        
    }

    virtual ArrayPointer createNewArrayPointer_();
    virtual ReferencePointer doFlattenOneElementCurrentArray();
private:
    PerlArrayPointer getPerlCurrentArrayPointer_() {
        return (PerlArrayPointer) currentArrayPointer_;
    }
};

#endif

#endif
