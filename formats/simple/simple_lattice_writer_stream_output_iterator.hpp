#ifndef SIMPLE_LATTICE_WRITER_STREAM_OUTPUT_ITERATOR_HDR
#define SIMPLE_LATTICE_WRITER_STREAM_OUTPUT_ITERATOR_HDR

#include <string>

#include "lattice_writer_output_iterator.hpp"

/**
 * Writes simple lattice writer's output to the stream.
 */
class SimpleLatticeWriterStreamOutputIterator : public LatticeWriterOutputIterator {
public:
    virtual ~SimpleLatticeWriterStreamOutputIterator();

private:
    virtual void doPutElement(const std::string& element);

    virtual void doOpenAlternative();

    virtual void doCloseAlternative();

    virtual void doOpenGroup(const std::string& group);

    virtual void doCloseGroup(const std::string& group);

};

#endif
