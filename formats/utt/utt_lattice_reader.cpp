#include "utt_lattice_reader.hpp"


std::string UTTLatticeReader::getFormatName() {
    return "UTT";
}

std::string UTTLatticeReader::doInfo() {
    return "UTT reader";
}

UTTLatticeReader::Worker::Worker(UTTLatticeReader& processor,
                                 std::istream& inputStream,
                                 Lattice& lattice):
    ReaderWorker(inputStream, lattice), processor_(processor) {
}

void UTTLatticeReader::Worker::doRun() {
    UTTQuoter quoter;
    UTTLRGrammar grammar;
    std::string line;
    std::string sentenceForm = "";
    int beginningOfSentencePosition = -1;
    while (std::getline(inputStream_, line)) {
        UTTLRItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {

            item.unescape(quoter);

            if (item.length > 0) {

                lattice_.appendString(item.form);

                LayerTagMask rawMask = lattice_.getLayerTagManager().getMask("raw");

                Lattice::Partition partition;
                for (int i = item.position; i < item.position + item.length; ++i) {
                    partition.links.push_back(
                        lattice_.firstOutEdge(lattice_.getVertexForRawCharIndex(i), rawMask)
                    );
                }

                lattice_.addEdge(
                    lattice_.getVertexForRawCharIndex(item.position),
                    lattice_.getVertexForRawCharIndex(item.position + item.length),
                    AnnotationItem(item.form),
                    lattice_.getLayerTagManager().createSingletonTagCollection("token"),
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

                LayerTagMask tokenMask = lattice_.getLayerTagManager().getMask("token");

                Lattice::Partition sentencePartition;
                for (int i = beginningOfSentencePosition; i < item.position + item.length; ++i) {
                    try {
                        sentencePartition.links.push_back(
                            lattice_.firstOutEdge(lattice_.getVertexForRawCharIndex(i), tokenMask)
                        );
                    } catch (NoEdgeException) { }
                }

                lattice_.addEdge(
                    lattice_.getVertexForRawCharIndex(beginningOfSentencePosition),
                    lattice_.getVertexForRawCharIndex(item.position + item.length),
                    AnnotationItem(sentenceForm),
                    lattice_.getLayerTagManager().createSingletonTagCollection("sentence"),
                    0.0,
                    sentencePartition
                );

            }

        }
    }
}

UTTLatticeReader::~UTTLatticeReader() {
}
