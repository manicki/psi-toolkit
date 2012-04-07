#ifndef JSON_LATTICE_WRITER_OUTPUT_ITERATOR_HDR
#define JSON_LATTICE_WRITER_OUTPUT_ITERATOR_HDR

#include "config.hpp"

#include <string>
#include "lattice_writer_output_iterator.hpp"
#include "annotation_item.hpp"
#include "json_lattice_writer_output.hpp"

class JSONLatticeWriterOutputIterator : public LatticeWriterOutputIterator {
public:
    JSONLatticeWriterOutputIterator(
                                    std::ostream & output,
                                    bool withArgs
    );

    virtual ~JSONLatticeWriterOutputIterator();

private:
    JSONLatticeWriterOutput outputWrapper_;

    virtual void doPutElement(const AnnotationItem& element);

    virtual void doOpenAlternative();

    virtual void doCloseAlternative();

    virtual void doOpenGroup(const std::string& group);

    virtual void doCloseGroup(const std::string& group);

    virtual void doFlush();

    bool withArgs_;
};

#endif
