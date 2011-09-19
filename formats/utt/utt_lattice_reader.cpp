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

                Lattice::VertexDescriptor from = lattice_.getLastVertex();

                lattice_.appendStringWithSymbols(item.form);

                Lattice::VertexDescriptor to = lattice_.getLastVertex();

                LayerTagMask rawMask = lattice_.getLayerTagManager().getMask("symbol");

                Lattice::EdgeSequence::Builder seqBuilder;

                Lattice::VertexDescriptor currentVertex = from;
                while (currentVertex != to) {
                    Lattice::EdgeDescriptor currentEdge
                        = lattice_.firstOutEdge(currentVertex, rawMask);
                    seqBuilder.addEdge(currentEdge);
                    currentVertex = lattice_.getEdgeTarget(currentEdge);
                }

                lattice_.addEdge(
                    from,
                    to,
                    AnnotationItem(item.form),
                    lattice_.getLayerTagManager().createSingletonTagCollection("token"),
                    seqBuilder.build()
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

                Lattice::EdgeSequence::Builder sentenceBuilder;
                for (int i = beginningOfSentencePosition; i < item.position + item.length; ++i) {
                    try {
                        sentenceBuilder.addEdge(
                            lattice_.firstOutEdge(lattice_.getVertexForRawCharIndex(i), tokenMask)
                        );
                    } catch (NoEdgeException) { }
                }

                lattice_.addEdge(
                    lattice_.getVertexForRawCharIndex(beginningOfSentencePosition),
                    lattice_.getVertexForRawCharIndex(item.position + item.length),
                    AnnotationItem(sentenceForm),
                    lattice_.getLayerTagManager().createSingletonTagCollection("sentence"),
                    sentenceBuilder.build()
                );

            }

        }
    }
}

UTTLatticeReader::~UTTLatticeReader() {
}
