#include "literal_test_output_iterator.hpp"


LiteralTestOutputIterator::LiteralTestOutputIterator(
    std::ostream& outputStream
) :
    outputStream_(outputStream)
{ }

LiteralTestOutputIterator::~LiteralTestOutputIterator() {
}

void LiteralTestOutputIterator::doPutElement(const AnnotationItem& element) {
    outputStream_ << element.getText();
}

void LiteralTestOutputIterator::doOpenAlternative() {
    outputStream_ << " [ ";
}

void LiteralTestOutputIterator::doCloseAlternative() {
    outputStream_ << " ] \n";
}

void LiteralTestOutputIterator::doOpenGroup(const std::string& group) {
    outputStream_ << " (" << group << "( \n";
}

void LiteralTestOutputIterator::doCloseGroup(const std::string& group) {
    outputStream_ << " )" << group << ") \n";
}

void LiteralTestOutputIterator::doFlush() {
    outputStream_ << " ! \n";
}
