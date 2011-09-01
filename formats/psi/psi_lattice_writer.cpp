#include "psi_lattice_writer.hpp"

#include <iomanip>
#include <iostream>

#include "logging.hpp"

std::string PsiLatticeWriter::getFormatName() {
    return "PSI";
}

LatticeWriter* PsiLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map&) {
    return new PsiLatticeWriter();
}

boost::program_options::options_description PsiLatticeWriter::Factory::doOptionsHandled() {
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


std::string PsiLatticeWriter::Factory::doGetName() {
    return "psi-writer";
}


std::string PsiLatticeWriter::doInfo() {
    return "PSI writer";
}

PsiLatticeWriter::Worker::Worker(PsiLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    WriterWorker(outputStream, lattice), processor_(processor) {
}

void PsiLatticeWriter::Worker::doRun() {

    DEBUG("starting writer...");

    PsiQuoter quoter;

    std::map<Lattice::EdgeDescriptor, int> edgeOrdinalMap;

    Lattice::EdgesSortedByTargetIterator ei
        = lattice_.edgesSortedByTarget(lattice_.getLayerTagManager().anyTag());

    int ordinal = 0;

    int alignments[] = { 2, 7, 13, 26, 48, 60 };

    while(ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();

        if (lattice_.isEdgeHidden(edge)) continue;

        ++ordinal;

        edgeOrdinalMap[edge] = ordinal;

        std::stringstream ordinalSs;
        ordinalSs << std::right << std::setfill('0') << std::setw(2);
        ordinalSs << ordinal;
        alignOutput_(ordinalSs.str(), alignments[0]);
        alignOutput_(" ");

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

        const AnnotationItem& annotationItem = lattice_.getEdgeAnnotationItem(edge);
        std::string edgeText;
        if (lattice_.isLooseVertex(source) || lattice_.isLooseVertex(target)) {
            edgeText = quoter.escape(annotationItem.getText());
        } else {
            edgeText = quoter.escape(lattice_.getEdgeText(edge));
        }
        alignOutput_(edgeText, alignments[3]);
        alignOutput_(" ");

        std::string tagStr = "";
        std::list<std::string> tagNames
            = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
        for (
            std::list<std::string>::iterator ti = tagNames.begin();
            ti != tagNames.end();
            ++ti
        ) {
            if (!tagStr.empty()) {
                tagStr += ",";
            }
            tagStr += *ti;
        }
        alignOutput_(quoter.escape(tagStr), alignments[4]);
        alignOutput_(" ");

        alignOutput_(quoter.escape(annotationItem.getText()), alignments[5]);
        alignOutput_(" ");

        std::stringstream aiSs;

        aiSs << quoter.escape(annotationItem.getCategory());

        Lattice::Score score = lattice_.getEdgeScore(edge);
        if (score != 0.0) {
            aiSs << "<" << score << ">";
        }

        std::string avStr = "";
        std::list< std::pair<std::string, std::string> > avPairs
            = lattice_.getAnnotationItemManager().getValues(annotationItem);
        for (
            std::list< std::pair<std::string, std::string> >::iterator avi = avPairs.begin();
            avi != avPairs.end();
            ++avi
        ) {
            avStr += ",";
            avStr += (*avi).first;
            avStr += "=";
            avStr += (*avi).second;
        }
        aiSs << quoter.escape(avStr);

        bool isDefaultPartition = (
            lattice_.getEdgeLayerTags(edge)
                == lattice_.getLayerTagManager().createSingletonTagCollection("symbol")
        );
        bool partitionBeginning = false;
        std::list<Lattice::Partition> partitions = lattice_.getEdgePartitions(edge);
        std::string partStr = "";
        for (
            std::list<Lattice::Partition>::iterator pi = partitions.begin();
            pi != partitions.end();
            ++pi
        ) {
            if (partStr != "") {
                partStr += ",";
            }
            std::stringstream linkSs;
            partitionBeginning = true;
            for (
                Lattice::Partition::Iterator ei = (*pi).begin();
                ei != (*pi).end();
                ++ei
            ) {
                if (partitionBeginning) {
                    if (
                        lattice_.isEdgeHidden(*ei)
                    ) {
                        isDefaultPartition = true;
                    }
                    partitionBeginning = false;
                } else {
                    if (
                        !lattice_.isEdgeHidden(*ei)
                    ) {
                        isDefaultPartition = false;
                    }
                    linkSs << "-";
                }
                std::map<Lattice::EdgeDescriptor, int>::iterator mi = edgeOrdinalMap.find(*ei);
                if (mi != edgeOrdinalMap.end()) {
                    linkSs << (*mi).second;
                }
            }
            partStr += linkSs.str();
        }
        if (!isDefaultPartition) {
            aiSs << "[" << partStr << "]";
        }

        alignOutput_(aiSs.str());
        alignOutputNewline_();

    }

    DEBUG("WRITING");
}

PsiLatticeWriter::Worker::~Worker() {
}
