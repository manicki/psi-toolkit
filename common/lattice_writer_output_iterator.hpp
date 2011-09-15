#ifndef LATTICE_WRITER_OUTPUT_ITERATOR_HDR
#define LATTICE_WRITER_OUTPUT_ITERATOR_HDR

#include <string>

#include "lattice.hpp"

/**
 * Lattice writers write their results using
 * lattice writer output iterators.
 */
class LatticeWriterOutputIterator {
public:
    /**
     * Puts an element into the iterator.
     */
    void putElement(const std::string& element);

    /**
     * Opens a group of alternative elements.
     */
    void openAlternative();

    /**
     * Closes a group of alternative elements.
     */
    void closeAlternative();

    /**
     * Opens a group.
     */
    void openGroup(const std::string& group);

    /**
     * Closes a group.
     */
    void closeGroup(const std::string& group);

    /**
     * Writes out the remaining content of the iterator.
     */
    void flush();

    virtual ~LatticeWriterOutputIterator();

private:
    virtual void doPutElement(const std::string& element) = 0;

    virtual void doOpenAlternative() = 0;

    virtual void doCloseAlternative() = 0;

    virtual void doOpenGroup(const std::string& group) = 0;

    virtual void doCloseGroup(const std::string& group) = 0;

    virtual void doFlush() = 0;

};

#endif
