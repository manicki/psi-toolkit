#include "psi_lattice_reader.hpp"


std::string PsiLatticeReader::getFormatName() {
    return "Psi";
}

std::string PsiLatticeReader::doInfo() {
    return "Psi reader";
}

PsiLatticeReader::Worker::Worker(PsiLatticeReader& processor,
                                 std::istream& inputStream,
                                 Lattice& lattice):
    ReaderWorker(inputStream, lattice), processor_(processor) {
}

void PsiLatticeReader::Worker::doRun() {
    PsiQuoter quoter;
    PsiLRGrammar grammar;
    PsiLRAVGrammar avGrammar;
    PsiLRAVPairGrammar avPairGrammar;
    PsiLRPartitionsGrammar partsGrammar;
    PsiLRPartitionGrammar partGrammar;
    std::map<int, Lattice::VertexDescriptor> looseVertices;
    std::map<int, Lattice::EdgeDescriptor> edgeOrdinalMap;
    std::string line;
    while (std::getline(inputStream_, line)) {
        if (
            boost::algorithm::trim_copy(line).empty()
            || boost::algorithm::trim_copy(line).at(0) == '#'
        ) {
            continue;
        }
        PsiLRItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {

            item.unescape(quoter);

            LayerTagCollection tags = lattice_.getLayerTagManager().createTagCollection(item.tags);
            LayerTagMask tagsMask = lattice_.getLayerTagManager().getMask(tags);

            std::stringstream formSs;
            formSs << std::setw(item.length) << item.text;
            std::string form = formSs.str();


            // Appending lattice.

            if (!item.beginningLoose) {
                try {
                    if (lattice_.getLayerTagManager().match(tagsMask, "symbol")) {
                        lattice_.appendString(form.substr(
                            lattice_.getVertexRawCharIndex(lattice_.getLastVertex())
                                - item.beginning
                        ));
                    } else {
                        lattice_.appendStringWithSymbols(form.substr(
                            lattice_.getVertexRawCharIndex(lattice_.getLastVertex())
                                - item.beginning
                        ));
                    }
                } catch (std::out_of_range) {
                    // Don't need to append lattice.
                }
            }


            // Defining source vertex.

            Lattice::VertexDescriptor from;
            if (item.beginningLoose) {
                std::map<int, Lattice::VertexDescriptor>::iterator lvi
                    = looseVertices.find(item.beginning);
                if (lvi == looseVertices.end()) {
                    looseVertices[item.beginning] = lattice_.addLooseVertex();
                }
                from = looseVertices[item.beginning];
            } else {
                from = lattice_.getVertexForRawCharIndex(item.beginning);
            }


            // Defining target vertex.

            Lattice::VertexDescriptor to;
            if (item.lengthPoint) {
                if (item.lengthLoose) {
                    std::map<int, Lattice::VertexDescriptor>::iterator lvi
                        = looseVertices.find(item.length);
                    if (lvi == looseVertices.end()) {
                        looseVertices[item.length] = lattice_.addLooseVertex();
                    }
                    to = looseVertices[item.length];
                } else {
                    to = lattice_.getVertexForRawCharIndex(item.length);
                }
            } else {
                if (item.lengthLoose) {
                    throw FileFormatException(
                        "PSI reader: Edge length cannot be a loose vertex. Missed point marker(*)?"
                    );
                } else {
                    to = lattice_.getVertexForRawCharIndex(item.beginning + item.length);
                }
            }


            // Creating annotation item.

            AnnotationItem annotationItem(item.annotationItem.category, item.annotationText);

            std::vector<std::string> avItem;
            std::string::const_iterator avBegin = item.annotationItem.avVector.begin();
            std::string::const_iterator avEnd = item.annotationItem.avVector.end();
            if (parse(avBegin, avEnd, avGrammar, avItem)) {
                BOOST_FOREACH(std::string av, avItem) {
                    PsiLRAVPairItem avPairItem;
                    std::string::const_iterator avPairBegin = av.begin();
                    std::string::const_iterator avPairEnd = av.end();
                    if (parse(avPairBegin, avPairEnd, avPairGrammar, avPairItem)) {
                        lattice_.getAnnotationItemManager().setValue(
                            annotationItem,
                            avPairItem.arg,
                            avPairItem.val
                        );
                    }
                }
            }


            // Defining partitions.

            LayerTagMask rawMask = lattice_.getLayerTagManager().getMask("symbol");
            Lattice::EdgeSequence::Builder seqBuilder;

            if (item.annotationItem.partitions.empty()) {

                if (!lattice_.getLayerTagManager().match(tagsMask, "symbol")) {
                    Lattice::VertexDescriptor currentVertex = from;
                    while (currentVertex != to) {
                        Lattice::EdgeDescriptor currentEdge
                            = lattice_.firstOutEdge(currentVertex, rawMask);
                        seqBuilder.addEdge(currentEdge);
                        currentVertex = lattice_.getEdgeTarget(currentEdge);
                    }
                }

            } else {

                std::vector<std::string> partsItem;
                std::string::const_iterator partsBegin = item.annotationItem.partitions.begin();
                std::string::const_iterator partsEnd = item.annotationItem.partitions.end();
                if (parse(partsBegin, partsEnd, partsGrammar, partsItem)) {

                    BOOST_FOREACH(std::string part, partsItem) {
                        std::vector<int> partItem;
                        std::string::const_iterator partBegin = part.begin();
                        std::string::const_iterator partEnd = part.end();
                        if (parse(partBegin, partEnd, partGrammar, partItem)) {
                            // BOOST_FOREACH(int edgeNumber, partItem) {

                                //TODO

                            // }
                        }
                    }

                }

            }


            // Adding edge.

            edgeOrdinalMap[item.ordinal] = lattice_.addEdge(
                from,
                to,
                annotationItem,
                tags,
                seqBuilder.build(),
                item.annotationItem.score
            );

        } else {

            throw FileFormatException("PSI reader: Wrong line.");

        }
    }

}

