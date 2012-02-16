#ifndef LATTICE_WRITER_OUTPUT_ITERATOR_HDR
#define LATTICE_WRITER_OUTPUT_ITERATOR_HDR

#include <string>

#include "lattice.hpp"
#include "annotation_item.hpp"

/**
 * Lattice writers write their results using
 * lattice writer output iterators.
 */
class LatticeWriterOutputIterator {
public:
    LatticeWriterOutputIterator() : latticeAnnotationItemManager_(NULL) { }

    /**
     * Puts an element into the iterator.
     */
    void putElement(const AnnotationItem & element);

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

    void setLatticeAnnotationItemManagerPointer(
         AnnotationItemManager * latticeAnnotationItemManager) {
        latticeAnnotationItemManager_ = latticeAnnotationItemManager;
    }

    virtual ~LatticeWriterOutputIterator();

protected:
    std::string getElementAnnotationItemStringToPut(const AnnotationItem & element) const;

    AnnotationItemManager * getLatticeAnnotationItemManagerPointer() {
        return latticeAnnotationItemManager_;
    }

private:
    virtual void doPutElement(const AnnotationItem & element) = 0;

    virtual void doOpenAlternative() = 0;

    virtual void doCloseAlternative() = 0;

    virtual void doOpenGroup(const std::string& group) = 0;

    virtual void doCloseGroup(const std::string& group) = 0;

    virtual void doFlush() = 0;

    AnnotationItemManager * latticeAnnotationItemManager_;
};

#endif
