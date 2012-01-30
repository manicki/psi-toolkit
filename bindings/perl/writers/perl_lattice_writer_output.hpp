#ifndef PERL_LATTICE_WRITER_OUTPUT_HDR
#define PERL_LATTICE_WRITER_OUTPUT_HDR


#include <string>
#include "config.hpp"
#include  <stack>
#include "annotation_item.hpp"
#include "annotation_item_manager.hpp"

#if HAVE_PERL_BINDINGS

#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

typedef HV* PerlHashPointer;
typedef AV* PerlArrayPointer;
typedef SV* PerlReference;

class PerlLatticeWriterOutput {
public:
    PerlLatticeWriterOutput(AV * arrayPointer);

    void push(const std::string & textElement);
    void push(const AnnotationItem & element,
              AnnotationItemManager * latticeAnnotationItemManager);

    void openNewSubArray();
    void closeSubArray();
    void closeSubArrayWithFlattenOneElement();

private:

    PerlReference getCurrentArrayReference_();
    void closeSubArray_(bool flattenOneElement);

    bool isCurrentArrayEmpty_();
    long getCurrentArrayLength_();
    PerlReference tryToFlattenOneElementCurrentArray();

    PerlArrayPointer currentArrayPointer_;
    std::stack<PerlArrayPointer> arraysStack_;

    AnnotationItemManager * latticeAnnotationItemManager_;
};

#endif

#endif
