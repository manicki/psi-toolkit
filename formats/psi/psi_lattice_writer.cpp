#include "psi_lattice_writer.hpp"

#include <iomanip>
#include <iostream>

#include "logging.hpp"

std::string PsiLatticeWriter::getFormatName() {
    return "PSI";
}

LatticeWriter* PsiLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options) {
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

    while(ei.hasNext()) {
        Lattice::EdgeDescriptor edge = ei.next();

        if (lattice_.isEdgeHidden(edge)) continue;

        ++ordinal;

        edgeOrdinalMap[edge] = ordinal;

        outputStream_ << std::right << std::setfill('0');
        outputStream_ << std::setw(2) << ordinal;
        outputStream_ << " ";
        outputStream_ << std::setw(4) << lattice_.getEdgeBeginIndex(edge);
        outputStream_ << " ";
        outputStream_ << std::setw(2) << lattice_.getEdgeLength(edge);
        outputStream_ << " ";

        outputStream_ << std::left << std::setfill(' ');
        const AnnotationItem& annotationItem = lattice_.getEdgeAnnotationItem(edge);
        outputStream_ << std::setw(12) << quoter.escape(lattice_.getEdgeText(edge));
        outputStream_ << " ";

        std::string tagStr = "";
        std::list<std::string> tagNames
            = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
        for (
            std::list<std::string>::iterator ti = tagNames.begin();
            ti != tagNames.end();
            ++ti
        ) {
            if (tagStr != "") {
                tagStr += ",";
            }
            tagStr += *ti;
        }
        outputStream_ << std::setw(20) << quoter.escape(tagStr);
        outputStream_ << " ";

        outputStream_ << quoter.escape(annotationItem.getCategory());

        Lattice::Score score = lattice_.getEdgeScore(edge);
        if (score != 0.0) {
            outputStream_ << "<" << score << ">";
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
        outputStream_ << quoter.escape(avStr);

        std::list<Lattice::Partition> partitions = lattice_.getEdgePartitions(edge);
        std::string partStr = "";
        for (
            std::list<Lattice::Partition>::iterator pi = partitions.begin();
            pi != partitions.end();
            ++pi
        ) {
            std::stringstream linkSs;
            for (
                Lattice::Partition::Iterator ei = (*pi).begin();
                ei != (*pi).end();
                ++ei
            ) {
                std::map<Lattice::EdgeDescriptor, int>::iterator mi = edgeOrdinalMap.find(*ei);
                if (mi != edgeOrdinalMap.end()) {
                    if (linkSs.str() != "") {
                        linkSs << "-";
                    }
                    linkSs << (*mi).second;
                }
            }
            partStr += linkSs.str();
        }
        if (partStr != "") {
            outputStream_ << "[" << partStr << "]";
        }

        outputStream_ << std::endl;
    }

    DEBUG("WRITING");
}

PsiLatticeWriter::Worker::~Worker() {
}
