#include "bonsai.hpp"
#include <sstream>

Annotator* Bonsai::Factory::doCreateAnnotator(
        const boost::program_options::variables_map& options) {
    std::string lang = options["lang"].as<std::string>();

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    Bonsai* bonsai = new Bonsai(lang);
    return bonsai;
}

void Bonsai::Factory::doAddLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription) {}

std::string Bonsai::Factory::doGetName() const {
    return "bonsai";
}

boost::filesystem::path Bonsai::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > Bonsai::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > Bonsai::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Bonsai::Factory::doProvidedLayerTags() {
    return std::list<std::string>();
}

LatticeWorker* Bonsai::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

Bonsai::Worker::Worker(Bonsai& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
}

void Bonsai::Worker::doRun() {
    processor_.tag(lattice_);
}

std::string Bonsai::doInfo() {
    return "Bonsai translator";
}

Bonsai::Bonsai(const std::string& langCode) :
    langCode_(langCode)
{ }

void Bonsai::tag(Lattice &lattice) {
    LayerTagMask segmentMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "segment", langCode_);
    Lattice::EdgesSortedBySourceIterator segmentIt =
        lattice.edgesSortedBySource(segmentMask);
    if (!segmentIt.hasNext()) {
        Lattice::VertexDescriptor start = lattice.getFirstVertex();
        Lattice::VertexDescriptor end   = lattice.getLastVertex();
        
        AnnotationItem ai("TRANS", StringFrag("lala lala lala"));
        LayerTagCollection tags
            = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
                "trans", langCode_);
        
        lattice.addEdge(start, end, ai, tags);
    }
    while (segmentIt.hasNext()) {

        Lattice::EdgeDescriptor segment = segmentIt.next();
        Lattice::VertexDescriptor start = lattice.getEdgeSource(segment);
        Lattice::VertexDescriptor end   = lattice.getEdgeTarget(segment);
        
        AnnotationItem ai("TRANS", StringFrag("lala lala lala"));
        LayerTagCollection tags
            = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
                "trans", langCode_);
        
        lattice.addEdge(start, end, ai, tags);
   }
}

