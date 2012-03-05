#ifndef ABSTRACT_SIMPLE_DATA_LATTICE_WRITER_OUTPUT_HDR
#define ABSTRACT_SIMPLE_DATA_LATTICE_WRITER_OUTPUT_HDR

#include <string>
#include "config.hpp"
#include  <stack>
#include "annotation_item.hpp"
#include "annotation_item_manager.hpp"

typedef void * ArrayPointer;
typedef void * ReferencePointer;

class AbstractSimpleDataLatticeWriterOutput {
public:
    void push(const std::string & textElement);
    void push(const AnnotationItem & element,
              AnnotationItemManager * latticeAnnotationItemManager);

    void openNewSubArray();
    void closeSubArray();
    void closeSubArrayWithFlattenOneElement();

    AbstractSimpleDataLatticeWriterOutput();
    virtual ~AbstractSimpleDataLatticeWriterOutput() {
    }

protected:

    void closeSubArray_(bool flattenOneElement);

    bool isCurrentArrayEmpty_();
    ReferencePointer tryToFlattenOneElementCurrentArray();

    void pushCurrentArrayPointerOnStack_() {
        arraysStack_.push(currentArrayPointer_);
    }

    void newCurrentArrayPointer_() {
        currentArrayPointer_ = createNewArrayPointer_();
    }

    virtual void pushToCurrentArrayPointer_(const std::string & textElement) = 0;
    virtual void pushToCurrentArrayPointer_(const AnnotationItem & element,
              AnnotationItemManager * latticeAnnotationItemManager) = 0;
    virtual void pushToArrayPointer_(ArrayPointer arrayPointer, ReferencePointer what) = 0;

    virtual long getCurrentArrayLength_() = 0;
    virtual ReferencePointer getCurrentArrayReference_() = 0;
    virtual ArrayPointer createNewArrayPointer_() = 0;
    virtual ReferencePointer doFlattenOneElementCurrentArray() = 0;

    ArrayPointer currentArrayPointer_;
    std::stack<ArrayPointer> arraysStack_;
    AnnotationItemManager * latticeAnnotationItemManager_;
};

#endif
