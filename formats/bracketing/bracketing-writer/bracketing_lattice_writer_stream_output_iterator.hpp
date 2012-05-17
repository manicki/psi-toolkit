#ifndef BRACKETING_LATTICE_WRITER_STREAM_OUTPUT_ITERATOR_HDR
#define BRACKETING_LATTICE_WRITER_STREAM_OUTPUT_ITERATOR_HDR

#include <iostream>
#include <sstream>
#include <string>

#include "lattice_writer_output_iterator.hpp"
#include "annotation_item.hpp"
/**
 * Writes bracketing lattice writer's output to the stream.
 */
class BracketingLatticeWriterStreamOutputIterator : public LatticeWriterOutputIterator {
public:
    BracketingLatticeWriterStreamOutputIterator(
        std::ostream& outputStream,
        std::string altSep,
        std::string sep
    );

    void setSeparator(std::string group, std::string sep);

    virtual ~BracketingLatticeWriterStreamOutputIterator();

private:
    std::ostream& outputStream_;

    std::string altSep_;
    std::string sep_;

    std::map<std::string, std::string> sepMap_;

    std::stringstream alternativeSs_;
    std::stringstream blockSs_;
    std::stringstream sepSs_;
    std::stringstream mainSs_;

    bool alternativeOpened_;
    std::map<std::string, bool> groupOpened_;

    virtual void doPutElement(const AnnotationItem& element);

    virtual void doOpenAlternative();

    virtual void doCloseAlternative();

    virtual void doOpenGroup(const std::string& group);

    virtual void doCloseGroup(const std::string& group);

    virtual void doFlush();

};

#endif
