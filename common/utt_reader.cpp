#include "utt_reader.hpp"


void UTTReader::readIntoLattice(std::istream& inputStream, Lattice& lattice) {
    UTTReaderItem item;
    UTTReaderGrammar grammar;
    std::string line;
    while (std::getline(inputStream, line)) {
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {
            LayerTagCollection 
                rawTag = lattice.getLayerTagManager().createSingletonTagCollection("raw");
            
            std::list<Lattice::EdgeDescriptor> partition;
            for (int i = item.position; i < item.position + item.length; ++i) {
                partition.push_back(
                    lattice.firstOutEdge(lattice.getVertexForRawCharIndex(i), rawTag)
                );
            }
            
            lattice.appendString(item.form.substr(0, item.length));
            lattice.addEdge(
                lattice.getVertexForRawCharIndex(item.position),
                lattice.getVertexForRawCharIndex(item.position + item.length),
                AnnotationItem(item.form),
                lattice.getLayerTagManager().createSingletonTagCollection("token"),
                0.0,
                partition
            );
        }
    }
}

std::string UTTReader::getFormatName() {
    return "UTT";
}