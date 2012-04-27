#include "reader_promise.hpp"

Processor* ReaderPromise::doCreateProcessor() {
    return readerFactory_->createProcessor(options_);
}

std::list<std::list<std::string> > ReaderPromise::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > ReaderPromise::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> ReaderPromise::doProvidedLayerTags() {
    return std::list<std::string>();
}

AnnotatorFactory::LanguagesHandling ReaderPromise::doLanguagesHandling() const {
    return AnnotatorFactory::LANGUAGE_INDEPENDENT;
}

std::list<std::string> ReaderPromise::doLanguagesHandled() const {
    return std::list<std::string>();
}

bool ReaderPromise::doIsAnnotator() const {
    return false;
}

bool ReaderPromise::doIsReader() const {
    return true;
}

bool ReaderPromise::doIsWriter() const {
    return false;
}
