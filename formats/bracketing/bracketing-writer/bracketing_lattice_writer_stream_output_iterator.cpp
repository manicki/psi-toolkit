#include "bracketing_lattice_writer_stream_output_iterator.hpp"


BracketingLatticeWriterStreamOutputIterator::BracketingLatticeWriterStreamOutputIterator(
    std::ostream& outputStream,
    std::string altSep,
    std::string sep
) :
    outputStream_(outputStream),
    altSep_(altSep),
    sep_(sep),
    alternativeOpened_(false)
{ }

BracketingLatticeWriterStreamOutputIterator::~BracketingLatticeWriterStreamOutputIterator() {
}

void BracketingLatticeWriterStreamOutputIterator::setSeparator(std::string group, std::string sep) {
    sepMap_[group] = sep;
    groupOpened_[group] = false;
}

void BracketingLatticeWriterStreamOutputIterator::doPutElement(const AnnotationItem& element) {
    std::string elementText =
        getElementAnnotationItemStringToPut(element);

    if (alternativeOpened_) {
        if (alternativeSs_.str() != "") {
            alternativeSs_ << altSep_;
        }
        alternativeSs_ << elementText;
    } else {
        if (blockSs_.str() != "") {
            blockSs_ << sep_;
        }
        blockSs_ << elementText;
    }
}

void BracketingLatticeWriterStreamOutputIterator::doOpenAlternative() {
    if (alternativeOpened_) {
        doCloseAlternative();
    }
    alternativeSs_.str("");
    alternativeOpened_ = true;
}

void BracketingLatticeWriterStreamOutputIterator::doCloseAlternative() {
    if (alternativeSs_.str() != "") {
        if (blockSs_.str() != "") {
            blockSs_ << sep_;
        }
        blockSs_ << alternativeSs_.str();
        alternativeSs_.str("");
    }
    alternativeOpened_ = false;
}

void BracketingLatticeWriterStreamOutputIterator::doOpenGroup(const std::string& group) {
    std::map<std::string, std::string>::iterator smi = sepMap_.find(group);
    if (smi != sepMap_.end()) {
        if (groupOpened_[group]) {
            doCloseGroup(group);
        }
        if (mainSs_.str() != "") {
            if (sepSs_.str().find((*smi).second) == std::string::npos) {
                sepSs_ << (*smi).second;
            }
        }
        groupOpened_[group] = true;
    }
}

void BracketingLatticeWriterStreamOutputIterator::doCloseGroup(const std::string& group) {
    std::map<std::string, std::string>::iterator smi = sepMap_.find(group);
    if (smi != sepMap_.end()) {
        if (blockSs_.str() != "") {
            mainSs_ << sepSs_.str();
            sepSs_.str("");
            mainSs_ << blockSs_.str();
        }
        blockSs_.str("");
        groupOpened_[group] = false;
    }
}

void BracketingLatticeWriterStreamOutputIterator::doFlush() {
    blockSs_ << alternativeSs_.str();
    mainSs_ << blockSs_.str();
    outputStream_ << mainSs_.str();
}
