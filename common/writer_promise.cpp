#include "writer_promise.hpp"

Processor* WriterPromise::doCreateProcessor() {
    return writerFactory_->createProcessor(options_);
}

std::list<std::list<std::string> > WriterPromise::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > WriterPromise::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> WriterPromise::doProvidedLayerTags() {
    return std::list<std::string>();
}

AnnotatorFactory::LanguagesHandling WriterPromise::doLanguagesHandling() const {
    return AnnotatorFactory::LANGUAGE_INDEPENDENT;
}

std::list<std::string> WriterPromise::doLanguagesHandled() const {
    return std::list<std::string>();
}

bool WriterPromise::doIsAnnotator() const {
    return false;
}

bool WriterPromise::doIsReader() const {
    return false;
}

bool WriterPromise::doIsWriter() const {
    return true;
}
