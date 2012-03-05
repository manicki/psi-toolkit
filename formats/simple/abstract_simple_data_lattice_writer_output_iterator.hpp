#ifndef ABSTRACT_SIMPLE_DATA_LATTICE_WRITER_OUTPUT_ITERATOR_HDR
#define ABSTRACT_SIMPLE_DATA_LATTICE_WRITER_OUTPUT_ITERATOR_HDR


#include <string>

#include "lattice_writer_output_iterator.hpp"
#include "abstract_simple_data_lattice_writer_output.hpp"
#include "annotation_item.hpp"

class AbstractSimpleDataLatticeWriterOutputIterator : public LatticeWriterOutputIterator {
public:
    AbstractSimpleDataLatticeWriterOutputIterator(
                                          AbstractSimpleDataLatticeWriterOutput & output,
                                          bool withArgs
    );

    virtual ~AbstractSimpleDataLatticeWriterOutputIterator();

private:
    AbstractSimpleDataLatticeWriterOutput & output_;

    virtual void doPutElement(const AnnotationItem& element);

    virtual void doOpenAlternative();

    virtual void doCloseAlternative();

    virtual void doOpenGroup(const std::string& group);

    virtual void doCloseGroup(const std::string& group);

    virtual void doFlush();

    bool withArgs_;
};

#endif
