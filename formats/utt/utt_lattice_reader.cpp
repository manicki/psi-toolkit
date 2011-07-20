#include "utt_lattice_reader.hpp"


void UTTLatticeReader::readIntoLattice(std::istream& inputStream, Lattice& lattice) {
    UTTLRGrammar grammar;
    std::string line;
    std::string sentenceForm = "";
    int beginningOfSentencePosition = -1;
    while (std::getline(inputStream, line)) {
        UTTLRItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {

            item.unescape();

            if (item.length > 0) {

                lattice.appendString(UTF8String(item.form).substr(0, item.length));

                LayerTagMask rawMask = lattice.getLayerTagManager().getMask("raw");

                std::list<Lattice::EdgeDescriptor> partition;
                for (int i = item.position; i < item.position + item.length; ++i) {
                    partition.push_back(
                        lattice.firstOutEdge(lattice.getVertexForRawCharIndex(i), rawMask)
                    );
                }

                lattice.addEdge(
                    lattice.getVertexForRawCharIndex(item.position),
                    lattice.getVertexForRawCharIndex(item.position + item.length),
                    AnnotationItem(item.form),
                    lattice.getLayerTagManager().createSingletonTagCollection("token"),
                    0.0,
                    partition
                );

                sentenceForm += item.form;

            } else if (item.segmentType == "BOS") {

                beginningOfSentencePosition = item.position;
                sentenceForm = "";

            } else if (item.segmentType == "EOS") {

                if (beginningOfSentencePosition == -1) {
                    throw FileFormatException("UTT reader: EOS tag does not match any BOS tag.");
                }

                LayerTagMask tokenMask = lattice.getLayerTagManager().getMask("token");

                std::list<Lattice::EdgeDescriptor> sentencePartition;
                for (int i = beginningOfSentencePosition; i < item.position + item.length; ++i) {
                    try {
                        sentencePartition.push_back(
                            lattice.firstOutEdge(lattice.getVertexForRawCharIndex(i), tokenMask)
                        );
                    } catch (NoEdgeException) { }
                }

                lattice.addEdge(
                    lattice.getVertexForRawCharIndex(beginningOfSentencePosition),
                    lattice.getVertexForRawCharIndex(item.position + item.length),
                    AnnotationItem(sentenceForm),
                    lattice.getLayerTagManager().createSingletonTagCollection("sentence"),
                    0.0,
                    sentencePartition
                );

            }

        }
    }
}
