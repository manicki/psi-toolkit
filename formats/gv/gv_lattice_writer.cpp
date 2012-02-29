#include "gv_lattice_writer.hpp"

#include <iomanip>
#include <iostream>
#include <locale>

#include "lattice.hpp"
#include "logging.hpp"

std::string GVLatticeWriter::getFormatName() {
    return "GraphViz";
}

LatticeWriter<std::ostream>* GVLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options) {

    std::set<std::string> filter;
    if (options.count("filter")) {
        std::vector<std::string> filterVector = options["filter"].as< std::vector<std::string> >();
        filter.insert(filterVector.begin(), filterVector.end());
    }

    return new GVLatticeWriter(
        options.count("show-tags"),
        options.count("color"),
        filter
    );
}

boost::program_options::options_description GVLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("color",
            "edges with different tags have different colors")
        ("filter", boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "filters edges by specified tags")
        ("show-tags",
            "prints layer tags")
        ;

    return optionsDescription;
}


std::string GVLatticeWriter::Factory::doGetName() {
    return "gv-writer";
}

boost::filesystem::path GVLatticeWriter::Factory::doGetFile() {
    return __FILE__;
}


std::string GVLatticeWriter::doInfo() {
    return "GraphViz writer";
}

GVLatticeWriter::Worker::Worker(GVLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}

void GVLatticeWriter::Worker::doRun() {

    DEBUG("starting writer...");

    PsiQuoter quoter;

    Lattice::EdgesSortedByTargetIterator ei
        = lattice_.edgesSortedByTarget(lattice_.getLayerTagManager().anyTag());

    alignOutput_("digraph G {");
    alignOutputNewline_();

    alignOutput_("rankdir=LR");
    alignOutputNewline_();

    while (ei.hasNext()) {

        Lattice::EdgeDescriptor edge = ei.next();

        // if (lattice_.isEdgeHidden(edge)) continue;

        std::list<std::string> tagNames
            = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));

        if (!processor_.areSomeInFilter(tagNames)) continue;

        std::stringstream edgeSs;

        Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
        if (lattice_.isLooseVertex(source)) {
            edgeSs << "L" << lattice_.getLooseVertexIndex(source);
        } else {
            edgeSs << lattice_.getVertexRawCharIndex(source);
        }

        edgeSs << " -> ";

        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);
        if (lattice_.isLooseVertex(target)) {
            edgeSs << "L" << lattice_.getLooseVertexIndex(target);
        } else {
            edgeSs << lattice_.getVertexRawCharIndex(target);
        }

        const AnnotationItem& annotationItem = lattice_.getEdgeAnnotationItem(edge);
        std::string edgeText;
        if (lattice_.isLooseVertex(source) || lattice_.isLooseVertex(target)) {
            edgeText = quoter.escape(annotationItem.getText());
        } else {
            edgeText = quoter.escape(lattice_.getEdgeText(edge));
        }

        std::string tagStr = "";
        std::stringstream colorSs;

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

        edgeSs << " [label=\"" << edgeText;
        if (processor_.isShowTags()) {
            edgeSs << " (" << tagStr << ")";
        }
        edgeSs << " " << annotationItem.getCategory() << "\"";
        if (processor_.isColor()) {
            edgeSs << ",color=\"" << std::setbase(16) << colorSs.str() << "\"";
        }
        edgeSs << "]";

        alignOutput_(edgeSs.str());
        alignOutputNewline_();

    }

    alignOutput_("}");
    alignOutputNewline_();

    DEBUG("WRITING");
}

GVLatticeWriter::Worker::~Worker() {
}
