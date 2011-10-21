#include "utt_lattice_reader.hpp"


std::string UTTLatticeReader::getFormatName() {
    return "UTT";
}

std::string UTTLatticeReader::doInfo() {
    return "UTT reader";
}


UTTLatticeReader::Factory::~Factory() {
}

LatticeReader<std::istream>* UTTLatticeReader::Factory::doCreateLatticeReader(
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
    UTTLRAnnotationsGrammar aGrammar;
    UTTLRAVGrammar avGrammar;
    PosConverter conv;
    std::string line;
    std::string sentenceForm = "";
    int beginningOfSentencePosition = -1;
    int endPosition = -1;
    while (std::getline(inputStream_, line)) {
        UTTLRItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {

            if (item.form == "*") {
                item.form = "";
            }

            if (item.position < 0) {
                if (endPosition < 0) {
                    throw FileFormatException("UTT reader: No beginning position.");
                }
                item.position = endPosition;
            }

            if (item.length < 0) {
                item.length = utf8::distance(item.form.begin(), item.form.end());
            }

            item.unescape(quoter);
            endPosition = item.position + item.length;

            if (item.length > 0) {

                if ((unsigned int)item.length == item.form.length()) {
                    try {
                        lattice_.appendStringWithSymbols(item.form.substr(
                            conv.utt(lattice_.getVertexRawCharIndex(lattice_.getLastVertex()))
                                - item.position
                        ));
                    } catch (std::out_of_range) {
                        // Don't need to append lattice.
                    }
                } else {
                    std::string::iterator iter = item.form.begin();
                    std::string::iterator end = item.form.end();
                    for (
                        int i = item.position;
                        i < conv.utt(lattice_.getVertexRawCharIndex(lattice_.getLastVertex()));
                        ++i
                    ) {
                        try {
                            utf8::next(iter, end);
                        } catch (utf8::not_enough_room) {
                            break; // Don't need to iterate further.
                        }
                    }
                    for (
                        int i = conv.utt(lattice_.getVertexRawCharIndex(lattice_.getLastVertex()));
                        i < endPosition;
                        ++i
                    ) {
                        conv.add(i, lattice_.getVertexRawCharIndex(lattice_.getLastVertex()));
                        try {
                            std::string symbol;
                            utf8::append(utf8::next(iter, end), std::back_inserter(symbol));
                            lattice_.appendStringWithSymbols(symbol);
                        } catch (utf8::not_enough_room) {
                            lattice_.appendStringWithSymbols(" ");
                                // Appending lattice with undefined content.
                        }
                    }
                    conv.add(
                        endPosition,
                        lattice_.getVertexRawCharIndex(lattice_.getLastVertex())
                    );
                }

                Lattice::VertexDescriptor from
                    = lattice_.getVertexForRawCharIndex(conv.psi(item.position));
                Lattice::VertexDescriptor to
                    = lattice_.getVertexForRawCharIndex(conv.psi(endPosition));

                LayerTagMask rawMask = lattice_.getLayerTagManager().getMask("symbol");

                Lattice::EdgeSequence::Builder seqBuilder(lattice_);

                Lattice::VertexDescriptor currentVertex = from;
                while (currentVertex != to) {
                    Lattice::EdgeDescriptor currentEdge
                        = lattice_.firstOutEdge(currentVertex, rawMask);
                    seqBuilder.addEdge(currentEdge);
                    currentVertex = lattice_.getEdgeTarget(currentEdge);
                }

                AnnotationItem annotationItem("'" + item.form + "'", item.form);

                std::vector<std::string> aItem;
                std::string::const_iterator aBegin = item.annotations.begin();
                std::string::const_iterator aEnd = item.annotations.end();
                if (parse(aBegin, aEnd, aGrammar, aItem)) {
                    BOOST_FOREACH(std::string annotation, aItem) {
                        UTTLRAVItem avItem;
                        std::string::const_iterator avBegin = annotation.begin();
                        std::string::const_iterator avEnd = annotation.end();
                        if (parse(avBegin, avEnd, avGrammar, avItem)) {
                            lattice_.getAnnotationItemManager().setValue(
                                annotationItem,
                                avItem.arg,
                                avItem.val
                            );
                        }
                    }
                }

                // if (!item.annotations.empty()) {
                    // lattice_.getAnnotationItemManager().setValue(
                        // annotationItem,
                        // "utt-annotations",
                        // item.annotations
                    // );
                // }

                lattice_.addEdge(
                    from,
                    to,
                    annotationItem,
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

                Lattice::EdgeSequence::Builder sentenceBuilder(lattice_);
                for (int i = beginningOfSentencePosition; i < endPosition; ++i) {
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
                    lattice_.getVertexForRawCharIndex(conv.psi(endPosition)),
                    AnnotationItem("sen", sentenceForm),
                    lattice_.getLayerTagManager().createSingletonTagCollection("sentence"),
                    sentenceBuilder.build()
                );

            }

        }
    }
}

UTTLatticeReader::~UTTLatticeReader() {
}
