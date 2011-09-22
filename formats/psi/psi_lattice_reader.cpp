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
    std::string line;
    while (std::getline(inputStream_, line)) {
        PsiLRItem item;
        std::string::const_iterator begin = line.begin();
        std::string::const_iterator end = line.end();
        if (parse(begin, end, grammar, item)) {

            item.unescape(quoter);

std::stringstream tagsSs;
BOOST_FOREACH(std::string tag, item.tags) {
if (!tagsSs.str().empty()) {
tagsSs << "+";
}
tagsSs << tag;
}

DEBUG("+|" << ((item.ordinal < 10)?" ":"") << item.ordinal
    << "|" << item.annotationItem.category << "|\t"
    << "|" << item.annotationItem.score << "|\t"
    << "|" << item.annotationItem.avVector << "|\t"
    << "|" << item.annotationItem.partitions
    << "|");

            std::vector<std::string> avItem;
            std::string::const_iterator avBegin = item.annotationItem.avVector.begin();
            std::string::const_iterator avEnd = item.annotationItem.avVector.end();
            if (parse(avBegin, avEnd, avGrammar, avItem)) {

                BOOST_FOREACH(std::string av, avItem) {
                    PsiLRAVPairItem avPairItem;
                    std::string::const_iterator avPairBegin = av.begin();
                    std::string::const_iterator avPairEnd = av.end();
                    if (parse(avPairBegin, avPairEnd, avPairGrammar, avPairItem)) {

// DEBUG("\t" << avPairItem.arg);
// DEBUG("\t\t" << avPairItem.val);

                    }
                }

            // } else {
// DEBUG("AV FAIL " << item.annotationItem.avVector);

            }

DEBUG("\t" << (item.annotationItem.partitions.empty()?"default":"[]"));

            std::vector<std::string> partsItem;
            std::string::const_iterator partsBegin = item.annotationItem.partitions.begin();
            std::string::const_iterator partsEnd = item.annotationItem.partitions.end();
            if (parse(partsBegin, partsEnd, partsGrammar, partsItem)) {

// DEBUG("\t" << ((partsItem.open && partsItem.close)?"[]":"default"));

                BOOST_FOREACH(std::string part, partsItem) {
                    std::vector<int> partItem;
                    std::string::const_iterator partBegin = part.begin();
                    std::string::const_iterator partEnd = part.end();
                    if (parse(partBegin, partEnd, partGrammar, partItem)) {
                        BOOST_FOREACH(int edge, partItem) {

DEBUG("\t\t" << edge);
// DEBUG("\t" << avPairItem.arg);
// DEBUG("\t\t" << avPairItem.val);

                        }
                    }
                }

            }

        } else {
if (!line.empty() && line[0] != '#')
DEBUG("-|" << line.substr(0,2) << "|--------");

        }
    }
}

