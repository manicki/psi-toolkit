#include "psi_lattice_writer.hpp"

#include <iomanip>
#include <iostream>

#include "logging.hpp"

std::string PsiLatticeWriter::getFormatName() {
    return "PSI";
}

LatticeWriter<std::ostream>* PsiLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map&) {
    return new PsiLatticeWriter();
}

boost::program_options::options_description PsiLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}


std::string PsiLatticeWriter::Factory::doGetName() {
    return "psi-writer";
}

boost::filesystem::path PsiLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}


std::string PsiLatticeWriter::doInfo() {
    return "PSI writer";
}

PsiLatticeWriter::Worker::Worker(PsiLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}

void PsiLatticeWriter::Worker::doRun() {

    DEBUG("starting writer...");

    PsiQuoter quoter;

    std::map<Lattice::EdgeDescriptor, int> edgeOrdinalMap;

    Lattice::EdgesSortedByTargetIterator ei
        = lattice_.edgesSortedByTarget(lattice_.getLayerTagManager().anyTag());

    int ordinal = 0;

    int alignments[] = { 2, 7, 13, 26, 48, 60 };

    while (ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();

        if (lattice_.isEdgeHidden(edge)) continue;

        ++ordinal;

        edgeOrdinalMap[edge] = ordinal;

        // ordinal:

        std::stringstream ordinalSs;
        ordinalSs << std::right << std::setfill('0') << std::setw(2);
        ordinalSs << ordinal;
        alignOutput_(ordinalSs.str(), alignments[0]);
        alignOutput_(" ");

        // beginning:

        std::stringstream beginningSs;
        Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
        if (lattice_.isLooseVertex(source)) {
            beginningSs << "@" << lattice_.getLooseVertexIndex(source);
        } else {
            beginningSs << std::right << std::setfill('0') << std::setw(4);
            beginningSs << lattice_.getVertexRawCharIndex(source);
        }
        alignOutput_(beginningSs.str(), alignments[1]);
        alignOutput_(" ");

        // length:

        std::stringstream lengthSs;
        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);
        if (lattice_.isLooseVertex(target)) {
            lengthSs << "*@" << lattice_.getLooseVertexIndex(target);
        } else if (lattice_.isLooseVertex(source)) {
            lengthSs << "*";
            lengthSs << std::right << std::setfill('0') << std::setw(4);
            lengthSs << lattice_.getVertexRawCharIndex(target);
        } else {
            lengthSs << std::right << std::setfill('0') << std::setw(2);
            lengthSs << lattice_.getEdgeLength(edge);
        }
        alignOutput_(lengthSs.str(), alignments[2]);
        alignOutput_(" ");

        // (some preparations):

        const AnnotationItem& annotationItem = lattice_.getEdgeAnnotationItem(edge);
        std::string edgeText;
        if (lattice_.isLooseVertex(source) || lattice_.isLooseVertex(target)) {
            edgeText = quoter.escape(annotationItem.getText());
        } else {
            edgeText = quoter.escape(lattice_.getEdgeText(edge));
        }

        std::list<Lattice::Partition> partitions = lattice_.getEdgePartitions(edge);
        bool writeWholeText = false;
        BOOST_FOREACH(Lattice::Partition partition, partitions) {
            Lattice::Partition::Iterator ei(lattice_, partition);
            while (ei.hasNext()) {
                if (
                    lattice_.getEdgeLayerTags(ei.next())
                        == lattice_.getLayerTagManager().createSingletonTagCollection("symbol")
                ) {
                    writeWholeText = true;
                    break;
                }
            }
        }

        // edge text:

        int edgeTextLength = utf8::distance(edgeText.begin(), edgeText.end());
        if (edgeTextLength == 0) {
            alignOutput_("∅", alignments[3]);
        } else if (edgeTextLength > alignments[3] - alignments[2] && !writeWholeText) {
            std::string::const_iterator bIter = edgeText.begin();
            utf8::unchecked::advance(bIter, alignments[3] - alignments[2] - 7);
            alignOutput_(edgeText.substr(0, bIter - edgeText.begin()));
            alignOutput_("...");

            std::string::const_iterator eIter = edgeText.end();
            for (int i = 0; i < 3; ++i)
                utf8::unchecked::prior(eIter);

            alignOutput_(edgeText.substr(eIter - edgeText.begin()),
                         alignments[3]);
        } else {
            alignOutput_(edgeText, alignments[3]);
        }
        alignOutput_(" ");

        // tags:

        std::string tagStr = "";
        std::list<std::string> tagNames
            = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
        BOOST_FOREACH(std::string tagName, tagNames) {
            if (!tagStr.empty()) {
                tagStr += ",";
            }
            tagStr += tagName;
        }
        alignOutput_(quoter.escape(tagStr), alignments[4]);
        alignOutput_(" ");

        // annotation text:

        alignOutput_(quoter.escape(annotationItem.getText()), alignments[5]);
        alignOutput_(" ");

        // annotations:

        std::stringstream aiSs;

        aiSs << quoter.escape(annotationItem.getCategory());

        Lattice::Score score = lattice_.getEdgeScore(edge);
        if (score != 0.0) {
            aiSs << "<" << score << ">";
        }

        std::string avStr = "";
        typedef std::pair<std::string, std::string> StrStrPair;
        std::list<StrStrPair> avPairs
            = lattice_.getAnnotationItemManager().getValues(annotationItem);
        BOOST_FOREACH(StrStrPair av, avPairs) {
            avStr += ",";
            avStr += av.first;
            avStr += "=";
            avStr += av.second;
        }
        aiSs << quoter.escape(avStr);

        bool isDefaultPartition = (
            lattice_.getEdgeLayerTags(edge)
                == lattice_.getLayerTagManager().createSingletonTagCollection("symbol")
        );
        bool firstPartition = true;
        bool partitionBeginning = false;
        std::stringstream partSs;
        BOOST_FOREACH(Lattice::Partition partition, partitions) {
            if (firstPartition) {
                firstPartition = false;
            } else {
                partSs << ",";
            }
            std::stringstream linkSs;
            partitionBeginning = true;
            Lattice::Partition::Iterator ei(lattice_, partition);
            while (ei.hasNext()) {
                Lattice::EdgeDescriptor ed = ei.next();
                if (partitionBeginning) {
                    if (
                        lattice_.isEdgeHidden(ed)
                    ) {
                        isDefaultPartition = true;
                    }
                    partitionBeginning = false;
                } else {
                    if (
                        !lattice_.isEdgeHidden(ed)
                    ) {
                        isDefaultPartition = false;
                    }
                    linkSs << "-";
                }
                std::map<Lattice::EdgeDescriptor, int>::iterator mi = edgeOrdinalMap.find(ed);
                if (mi != edgeOrdinalMap.end()) {
                    linkSs << (*mi).second;
                }
            }
            partSs << linkSs.str();
            std::list<std::string> partitionTagNames
                = lattice_.getLayerTagManager().getTagNames(partition.getTagList());
            if (partitionTagNames != tagNames) {
                std::string partitionTagStr = "";
                BOOST_FOREACH(std::string partitionTagName, partitionTagNames) {
                    if (!partitionTagStr.empty()) {
                        partitionTagStr += ",";
                    }
                    partitionTagStr += partitionTagName;
                }
                partSs << "(" << partitionTagStr << ")";
            }
            Lattice::Score partitionScore = partition.getScore();
            if (partitionScore != score) {
                partSs << "<" << partitionScore << ">";
            }
        }
        if (!isDefaultPartition) {
            aiSs << "[" << partSs.str() << "]";
        }

        alignOutput_(aiSs.str());
        alignOutputNewline_();

    }

    DEBUG("WRITING");
}

PsiLatticeWriter::Worker::~Worker() {
}
