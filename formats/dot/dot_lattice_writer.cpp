#include "dot_lattice_writer.hpp"

#include <iomanip>
#include <iostream>
#include <locale>

#include "lattice.hpp"
#include "logging.hpp"

std::string DotLatticeWriter::getFormatName() {
    return "DOT";
}

LatticeWriter<std::ostream>* DotLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options) {

    std::set<std::string> filter;
    if (options.count("filter")) {
        std::vector<std::string> filterVector = options["filter"].as< std::vector<std::string> >();
        filter.insert(filterVector.begin(), filterVector.end());
    }

    return new DotLatticeWriter(
        options.count("show-tags"),
        options.count("color"),
        filter,
        options.count("tree")
    );
}

boost::program_options::options_description DotLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("color",
            "edges with different tags have different colors")
        ("filter", boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "filters edges by specified tags")
        ("show-tags",
            "prints layer tags")
        ("tree",
            "shows dependencies between edges instead of the content of the lattice")
        ;

    return optionsDescription;
}


std::string DotLatticeWriter::Factory::doGetName() {
    return "dot-writer";
}

boost::filesystem::path DotLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}


std::string DotLatticeWriter::doInfo() {
    return "DOT writer";
}

DotLatticeWriter::Worker::Worker(DotLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}

void DotLatticeWriter::Worker::doRun() {

    DEBUG("starting writer...");

    PsiQuoter quoter;

    std::map<Lattice::EdgeDescriptor, int> edgeOrdinalMap;
    int ordinal = 0;

    Lattice::EdgesSortedByTargetIterator ei
        = lattice_.edgesSortedByTarget(lattice_.getLayerTagManager().anyTag());

    alignOutput_("digraph G {");
    alignOutputNewline_();

    if (processor_.isTree()) {
        alignOutput_("rankdir=TB");
    } else {
        alignOutput_("rankdir=LR");
    }
    alignOutputNewline_();

    while (ei.hasNext()) {

        Lattice::EdgeDescriptor edge = ei.next();

        std::list<std::string> tagNames
            = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));

        if (!processor_.areSomeInFilter(tagNames)) continue;

        std::stringstream edgeSs;

        Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);

        std::stringstream edgeIdSs;
        std::stringstream edgeLabelSs;

        const AnnotationItem& annotationItem = lattice_.getEdgeAnnotationItem(edge);
        if (lattice_.isLooseVertex(source) || lattice_.isLooseVertex(target)) {
            edgeLabelSs << quoter.escape(annotationItem.getText());
        } else {
            edgeLabelSs << quoter.escape(lattice_.getEdgeText(edge));
        }

        std::string tagStr("");
        std::stringstream colorSs;
        colorSs << std::setbase(16);

        if (processor_.isShowTags() || processor_.isColor()) {
            BOOST_FOREACH(std::string tagName, tagNames) {
                if (!processor_.isInFilter(tagName)) continue;
                if (!tagStr.empty()) {
                    tagStr += ",";
                    colorSs << ":";
                }
                tagStr += tagName;
                if (processor_.isColor()) {
                    const std::collate<char>& coll
                        = std::use_facet<std::collate<char> >(std::locale());
                    unsigned int color
                        = coll.hash(tagName.data(), tagName.data() + tagName.length()) & 0xffffff;
                    if ((color & 0xe0e0e0) == 0xe0e0e0) color &= 0x7f7f7f; // darken if too bright
                    colorSs << "#" << std::setbase(16) << color;
                }
            }
        }

        if (processor_.isShowTags()) {
            edgeLabelSs << " (" << tagStr << ")";
        }

        edgeLabelSs << " " << annotationItem.getCategory();

        if (processor_.isTree()) {

            ++ordinal;
            edgeOrdinalMap[edge] = ordinal;
            edgeIdSs << ordinal;

            edgeSs << edgeIdSs.str() << " [label=\"" << edgeLabelSs.str() << "\"";
            if (processor_.isColor()) {
                edgeSs << "color=\"" << colorSs.str() << "\"";
            }
            edgeSs << "]";

            int partitionNumber = 0;
            std::list<Lattice::Partition> partitions = lattice_.getEdgePartitions(edge);
            BOOST_FOREACH(Lattice::Partition partition, partitions) {
                std::stringstream partSs;
                ++partitionNumber;
                partSs << partitionNumber;
                Lattice::Partition::Iterator ei(lattice_, partition);
                while (ei.hasNext()) {
                    Lattice::EdgeDescriptor ed = ei.next();
                    std::map<Lattice::EdgeDescriptor, int>::iterator
                        moi = edgeOrdinalMap.find(ed);
                    if (moi != edgeOrdinalMap.end()) {
                        std::stringstream edSs;
                        edSs << moi->second;
                        edgeSs << "\n" << edgeIdSs.str() << " -> " << edSs.str();
                        if (partitions.size() > 1) {
                            edgeSs << " [label=\"" << partSs.str() << "]";
                        }
                    }
                }
            }

        } else {

            std::stringstream nSs;
            if (lattice_.isLooseVertex(source)) {
                nSs << "L" << lattice_.getLooseVertexIndex(source);
            } else {
                nSs << lattice_.getVertexRawCharIndex(source);
            }

            std::stringstream mSs;
            if (lattice_.isLooseVertex(target)) {
                mSs << "L" << lattice_.getLooseVertexIndex(target);
            } else {
                mSs << lattice_.getVertexRawCharIndex(target);
            }

            edgeSs << nSs.str() << " -> " << mSs.str();
            edgeSs << " [label=\"" << edgeLabelSs.str() << "\"";
            if (processor_.isColor()) {
                edgeSs << ",color=\"" << colorSs.str() << "\"";
            }
            edgeSs << "]";

        }

        alignOutput_(edgeSs.str());
        alignOutputNewline_();

    }

    alignOutput_("}");
    alignOutputNewline_();

    DEBUG("WRITING");
}

DotLatticeWriter::Worker::~Worker() {
}
