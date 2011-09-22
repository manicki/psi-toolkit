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
    LayerTagManager ltm = lattice_.getLayerTagManager();
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

            LayerTagCollection tags = ltm.createTagCollection(item.tags);
            LayerTagMask tagsMask = ltm.getMask(tags);

            if (!item.beginningLoose) {
                try {
                    if (ltm.match(tagsMask, "symbol")) {
                        lattice_.appendString(item.text.substr(
                            lattice_.getVertexRawCharIndex(lattice_.getLastVertex()) - item.beginning
                        ));
                    } else {
                        lattice_.appendStringWithSymbols(item.text.substr(
                            lattice_.getVertexRawCharIndex(lattice_.getLastVertex()) - item.beginning
                        ));
                    }
                } catch (std::out_of_range) {
                    // Don't need to append lattice.
                }
            }

            std::vector<std::string> avItem;
            std::string::const_iterator avBegin = item.annotationItem.avVector.begin();
            std::string::const_iterator avEnd = item.annotationItem.avVector.end();
            if (parse(avBegin, avEnd, avGrammar, avItem)) {

                BOOST_FOREACH(std::string av, avItem) {
                    PsiLRAVPairItem avPairItem;
                    std::string::const_iterator avPairBegin = av.begin();
                    std::string::const_iterator avPairEnd = av.end();
                    if (parse(avPairBegin, avPairEnd, avPairGrammar, avPairItem)) {

                        //TODO

                    }
                }

            }

            std::vector<std::string> partsItem;
            std::string::const_iterator partsBegin = item.annotationItem.partitions.begin();
            std::string::const_iterator partsEnd = item.annotationItem.partitions.end();
            if (parse(partsBegin, partsEnd, partsGrammar, partsItem)) {

                BOOST_FOREACH(std::string part, partsItem) {
                    std::vector<int> partItem;
                    std::string::const_iterator partBegin = part.begin();
                    std::string::const_iterator partEnd = part.end();
                    if (parse(partBegin, partEnd, partGrammar, partItem)) {
                        // BOOST_FOREACH(int edge, partItem) {

                            //TODO

                        // }
                    }
                }

            }

        } else {

            throw FileFormatException("PSI reader: Wrong line.");

        }
    }

DEBUG("Σ: " << lattice_.getAllText());

}

