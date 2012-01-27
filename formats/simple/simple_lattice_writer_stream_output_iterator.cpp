#include "simple_lattice_writer_stream_output_iterator.hpp"


SimpleLatticeWriterStreamOutputIterator::SimpleLatticeWriterStreamOutputIterator(
    std::ostream& outputStream,
    std::string altSep,
    std::string sep
) :
    outputStream_(outputStream),
    altSep_(altSep),
    sep_(sep),
    alternativeOpened_(false)
{ }

SimpleLatticeWriterStreamOutputIterator::~SimpleLatticeWriterStreamOutputIterator() {
}

void SimpleLatticeWriterStreamOutputIterator::setSeparator(std::string group, std::string sep) {
    sepMap_[group] = sep;
    groupOpened_[group] = false;
}

void SimpleLatticeWriterStreamOutputIterator::doPutElement(const AnnotationItem& element) {
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

void SimpleLatticeWriterStreamOutputIterator::doOpenAlternative() {
    if (alternativeOpened_) {
        doCloseAlternative();
    }
    alternativeSs_.str("");
    alternativeOpened_ = true;
}

void SimpleLatticeWriterStreamOutputIterator::doCloseAlternative() {
    if (alternativeSs_.str() != "") {
        if (blockSs_.str() != "") {
            blockSs_ << sep_;
        }
        blockSs_ << alternativeSs_.str();
        alternativeSs_.str("");
    }
    alternativeOpened_ = false;
}

void SimpleLatticeWriterStreamOutputIterator::doOpenGroup(const std::string& group) {
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

void SimpleLatticeWriterStreamOutputIterator::doCloseGroup(const std::string& group) {
    std::map<std::string, std::string>::iterator smi = sepMap_.find(group);
    if (smi != sepMap_.end()) {
        if (blockSs_.str() != "") {
            mainSs_ << sepSs_.str();
            mainSs_ << blockSs_.str();
        }
        blockSs_.str("");
        groupOpened_[group] = false;
    }
}

void SimpleLatticeWriterStreamOutputIterator::doFlush() {
    blockSs_ << alternativeSs_.str();
    mainSs_ << blockSs_.str();
    outputStream_ << mainSs_.str();
}
