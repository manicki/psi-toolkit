#include "literal_test_output_iterator.hpp"


LiteralTestOutputIterator::LiteralTestOutputIterator(
    std::ostream& outputStream
) :
    outputStream_(outputStream)
{ }

LiteralTestOutputIterator::~LiteralTestOutputIterator() {
}

void LiteralTestOutputIterator::doPutElement(const AnnotationItem& element) {
    outputStream_ << "[" << element.getText() << "]\n";
}

void LiteralTestOutputIterator::doOpenAlternative() {
    outputStream_ << "openalt\n";
}

void LiteralTestOutputIterator::doCloseAlternative() {
    outputStream_ << "closealt\n";
}

void LiteralTestOutputIterator::doOpenGroup(const std::string& group) {
    outputStream_ << "open(" << group << ")\n";
}

void LiteralTestOutputIterator::doCloseGroup(const std::string& group) {
    outputStream_ << "close(" << group << ")\n";
}

void LiteralTestOutputIterator::doFlush() {
    outputStream_ << "flush\n";
}
