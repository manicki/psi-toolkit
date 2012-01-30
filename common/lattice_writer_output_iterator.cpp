#include "lattice_writer_output_iterator.hpp"
#include "logging.hpp"

void LatticeWriterOutputIterator::putElement(const AnnotationItem& element) {
    doPutElement(element);
}

void LatticeWriterOutputIterator::openAlternative() {
    doOpenAlternative();
}

void LatticeWriterOutputIterator::closeAlternative() {
    doCloseAlternative();
}

void LatticeWriterOutputIterator::openGroup(const std::string& group) {
    doOpenGroup(group);
}

void LatticeWriterOutputIterator::closeGroup(const std::string& group) {
    doCloseGroup(group);
}

void LatticeWriterOutputIterator::flush() {
    doFlush();
}

LatticeWriterOutputIterator::~LatticeWriterOutputIterator() {
}

std::string LatticeWriterOutputIterator::getElementAnnotationItemStringToPut(const AnnotationItem & element) const {
    if (element.getText().empty()) {
        return element.getCategory();
    } else {
        return element.getText();
    }

}
