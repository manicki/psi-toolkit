#include "annotator_factory.hpp"

Annotator* AnnotatorFactory::createAnnotator(
    const boost::program_options::variables_map& options) {
    return dynamic_cast<Annotator*>(createProcessor(options));
}

Annotator* AnnotatorFactory::doCreateProcessor(
    const boost::program_options::variables_map& options) {
    return doCreateAnnotator(options);
}

std::list<std::list<std::string> > AnnotatorFactory::requiredLayerTags() {
    return doRequiredLayerTags();
}

std::list<std::list<std::string> > AnnotatorFactory::optionalLayerTags() {
    return doOptionalLayerTags();
}

std::list<std::string> AnnotatorFactory::providedLayerTags() {
    return doProvidedLayerTags();
}




