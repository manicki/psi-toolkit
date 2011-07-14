#include "utt_reader.hpp"


void UTTReader::readIntoLattice(std::istream& inputStream, Lattice& lattice) {
    UTTReaderGrammar grammar;
    std::string line;
    int beginningOfSentencePosition = -1;
    while (std::getline(inputStream, line)) {
        UTTReaderItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {
            
            if (item.length > 0) {

                lattice.appendString(item.form.substr(0, item.length));

                LayerTagCollection rawTag 
                    = lattice.getLayerTagManager().createSingletonTagCollection("raw");
                
                std::list<Lattice::EdgeDescriptor> partition;
                for (int i = item.position; i < item.position + item.length; ++i) {
                    partition.push_back(
                        lattice.firstOutEdge(lattice.getVertexForRawCharIndex(i), rawTag)
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
            
            } else if (item.segmentType == "BOS") {
                
                beginningOfSentencePosition = item.position;
                
            } else if (item.segmentType == "EOS") {
                
                if (beginningOfSentencePosition == -1) {
                    throw FileFormatException("UTT reader: EOS tag does not match any BOS tag.");
                }

                LayerTagCollection sentenceTag 
                    = lattice.getLayerTagManager().createSingletonTagCollection("sentence");
                
                std::list<Lattice::EdgeDescriptor> sentencePartition;
                for (int i = beginningOfSentencePosition; i < item.position + item.length; ++i) {
                    sentencePartition.push_back(
                        lattice.firstOutEdge(lattice.getVertexForRawCharIndex(i), sentenceTag)
                    );
                }
                            
            }
            
        }
    }
}

std::string UTTReader::getFormatName() {
    return "UTT";
}