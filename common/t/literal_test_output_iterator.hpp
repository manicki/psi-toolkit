#ifndef LITERAL_TEST_OUTPUT_ITERATOR_HDR
#define LITERAL_TEST_OUTPUT_ITERATOR_HDR

#include <iostream>
#include <sstream>
#include <string>

#include "lattice_writer_output_iterator.hpp"

/**
 * Writes literal output to the stream.
 */
class LiteralTestOutputIterator : public LatticeWriterOutputIterator {
public:
    LiteralTestOutputIterator(
        std::ostream& outputStream
    );

    virtual ~LiteralTestOutputIterator();

private:
    std::ostream& outputStream_;

    virtual void doPutElement(const std::string& element);

    virtual void doOpenAlternative();

    virtual void doCloseAlternative();

    virtual void doOpenGroup(const std::string& group);

    virtual void doCloseGroup(const std::string& group);

    virtual void doFlush();

};

#endif
