#include "annotator_factory.hpp"

Annotator* AnnotatorFactory::createAnnotator(
    boost::program_options::variables_map options) {
    return dynamic_cast<Annotator*>(createProcessor(options));
}

Annotator* AnnotatorFactory::doCreateProcessor(
    boost::program_options::variables_map options) {
    return doCreateAnnotator(options);
}

std::list<LayerTagCollection> AnnotatorFactory::requiredLayerTags() {
    return doRequiredLayerTags();
}

std::list<LayerTagCollection> AnnotatorFactory::optionalLayerTags() {
    return doOptionalLayerTags();
}

LayerTagCollection AnnotatorFactory::providedLayerTags() {
    return doProvidedLayerTags();
}




