#include "utt_lattice_reader.hpp"


std::string UTTLatticeReader::getFormatName() {
    return "UTT";
}

std::string UTTLatticeReader::doInfo() {
    return "UTT reader";
}


UTTLatticeReader::Factory::~Factory() {
}

LatticeReader* UTTLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map&) {
    return new UTTLatticeReader();
}

boost::program_options::options_description UTTLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("line-by-line", "processes line by line")
        ("whole-text",   "read the whole text")
        ("paragraphs",   "paragraphs are delimited with double newlines")
        ("discard-comments", "discards comments")
        ("pass-through-comments", "marks comments as single markup")
        ;

    return optionsDescription;
}

std::string UTTLatticeReader::Factory::doGetName() {
    return "utt-reader";
}


UTTLatticeReader::Worker::Worker(UTTLatticeReader& processor,
                                 std::istream& inputStream,
                                 Lattice& lattice):
    ReaderWorker(inputStream, lattice), processor_(processor) {
}

void UTTLatticeReader::Worker::doRun() {
    UTTQuoter quoter;
    UTTLRGrammar grammar;
    PosConverter conv;
    std::string line;
    std::string sentenceForm = "";
    int beginningOfSentencePosition = -1;
    while (std::getline(inputStream_, line)) {
        UTTLRItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {

            item.unescape(quoter);

            if (item.position < 0) {
                item.position = conv.utt(lattice_.getVertexRawCharIndex(lattice_.getLastVertex()));
            }

            if (item.length < 0) {
                item.length = utf8::distance(item.form.begin(), item.form.end());
            }

            if (item.length > 0) {

                if ((unsigned int)item.length == item.form.length()) {
                    lattice_.appendStringWithSymbols(item.form.substr(
                        conv.utt(lattice_.getVertexRawCharIndex(lattice_.getLastVertex()))
                            - item.position
                    ));
                } else {
                    std::string::iterator iter = item.form.begin();
                    std::string::iterator end = item.form.end();
                    for (
                        int i = item.position;
                        i < conv.utt(lattice_.getVertexRawCharIndex(lattice_.getLastVertex()));
                        ++i
                    ) {
                        utf8::next(iter, end);
                    }
                    for (
                        int i = conv.utt(lattice_.getVertexRawCharIndex(lattice_.getLastVertex()));
                        i < item.position + item.length;
                        ++i
                    ) {
                        conv.add(i, lattice_.getVertexRawCharIndex(lattice_.getLastVertex()));
                        std::string symbol;
                        utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
                        lattice_.appendStringWithSymbols(symbol);
                    }
                    conv.add(
                        item.position + item.length,
                        lattice_.getVertexRawCharIndex(lattice_.getLastVertex())
                    );
                }

                Lattice::VertexDescriptor from
                    = lattice_.getVertexForRawCharIndex(conv.psi(item.position));
                Lattice::VertexDescriptor to
                    = lattice_.getVertexForRawCharIndex(conv.psi(item.position + item.length));

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
                            lattice_.firstOutEdge(
                                lattice_.getVertexForRawCharIndex(conv.psi(i)),
                                tokenMask
                            )
                        );
                    } catch (NoEdgeException) { }
                }

                lattice_.addEdge(
                    lattice_.getVertexForRawCharIndex(conv.psi(beginningOfSentencePosition)),
                    lattice_.getVertexForRawCharIndex(conv.psi(item.position + item.length)),
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
