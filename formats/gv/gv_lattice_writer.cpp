#include "gv_lattice_writer.hpp"

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <locale>
#include <fstream>

#include <gvc.h>

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
        filter,
        options["format"].as<std::string>(),
        options.count("tree")
    );
}

boost::program_options::options_description GVLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("color",
            "edges with different tags have different colors")
        ("filter", boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "filters edges by specified tags")
        ("format", boost::program_options::value<std::string>()->default_value("dot"),
            "output format")
        ("show-tags",
            "prints layer tags")
        ("tree",
            "shows dependencies between edges instead of the content of the lattice")
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

    std::map<Lattice::EdgeDescriptor, int> edgeOrdinalMap;
    int ordinal = 0;

    char * tmpFile;

    GVC_t * gvc = gvContext();
    std::string mainFormat
        = processor_.getOutputFormat().substr(0, processor_.getOutputFormat().find(":"));
    if (
        processor_.getOutputFormat() != "canon" &&
        processor_.getOutputFormat() != "dot" &&
        processor_.getOutputFormat() != "eps" &&
        processor_.getOutputFormat() != "fig" &&
        processor_.getOutputFormat() != "gd" &&
        processor_.getOutputFormat() != "gd:cairo" &&
        processor_.getOutputFormat() != "gd:gd" &&
        processor_.getOutputFormat() != "gd2" &&
        processor_.getOutputFormat() != "gd2:cairo" &&
        processor_.getOutputFormat() != "gd2:gd" &&
        processor_.getOutputFormat() != "gif" &&
        processor_.getOutputFormat() != "gif:cairo" &&
        processor_.getOutputFormat() != "gif:gd" &&
        processor_.getOutputFormat() != "gv" &&
        processor_.getOutputFormat() != "jpe" &&
        processor_.getOutputFormat() != "jpe:cairo" &&
        processor_.getOutputFormat() != "jpe:gd" &&
        processor_.getOutputFormat() != "jpeg" &&
        processor_.getOutputFormat() != "jpeg:cairo" &&
        processor_.getOutputFormat() != "jpeg:gd" &&
        processor_.getOutputFormat() != "jpg" &&
        processor_.getOutputFormat() != "jpg:cairo" &&
        processor_.getOutputFormat() != "jpg:gd" &&
        processor_.getOutputFormat() != "pdf" &&
        processor_.getOutputFormat() != "plain" &&
        processor_.getOutputFormat() != "plain-ext" &&
        processor_.getOutputFormat() != "png" &&
        processor_.getOutputFormat() != "png:cairo" &&
        processor_.getOutputFormat() != "png:gd" &&
        processor_.getOutputFormat() != "ps" &&
        processor_.getOutputFormat() != "ps:cairo" &&
        processor_.getOutputFormat() != "ps:ps" &&
        processor_.getOutputFormat() != "ps2" &&
        processor_.getOutputFormat() != "svg" &&
        processor_.getOutputFormat() != "svg:cairo" &&
        processor_.getOutputFormat() != "svg:svg" &&
        processor_.getOutputFormat() != "svgz" &&
        processor_.getOutputFormat() != "tk" &&
        processor_.getOutputFormat() != "vml" &&
        processor_.getOutputFormat() != "vmlz" &&
        processor_.getOutputFormat() != "wbmp" &&
        processor_.getOutputFormat() != "wbmp:cairo" &&
        processor_.getOutputFormat() != "wbmp:gd" &&
        processor_.getOutputFormat() != "xdot"
    ) {
        throw PsiException("Format \"" + processor_.getOutputFormat() +
            "\" not recognized. Use one of the following formats: " +
            "canon dot eps fig gd(:cairo,:gd) gd2(:cairo,:gd) gif(:cairo,:gd) gv " +
            "jpe(:cairo,:gd) jpeg(:cairo,:gd) jpg(:cairo,:gd) pdf plain plain-ext " +
            "png(:cairo,:gd) ps(:cairo,:ps) ps2 svg(:cairo,:svg) svgz tk vml vmlz " +
            "wbmp(:cairo,:gd) xdot");
    }
    std::string arg1("-T" + processor_.getOutputFormat());
    std::string arg2("");
    tmpFile = tempnam(NULL, "gv_");
    arg2 += "-o";
    arg2 += tmpFile;
    const char * const args[] = {
        "dot",
        arg1.c_str(),
        arg2.c_str()
    };
    gvParseArgs(gvc, sizeof(args)/sizeof(char*), (char**)args);
    Agraph_t * g = agopen((char*)"g", AGDIGRAPH);
    Agnode_t * n;
    Agnode_t * m;
    Agedge_t * e;

    Lattice::EdgesSortedByTargetIterator ei
        = lattice_.edgesSortedByTarget(lattice_.getLayerTagManager().anyTag());

    if (processor_.isTree()) {
        agsafeset(g, (char*)"rankdir", (char*)"TB", (char*)"");
    } else {
        agsafeset(g, (char*)"rankdir", (char*)"LR", (char*)"");
    }

    while (ei.hasNext()) {

        Lattice::EdgeDescriptor edge = ei.next();

        std::list<std::string> tagNames
            = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));

        if (!processor_.areSomeInFilter(tagNames)) continue;

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

            n = agnode(g, (char*)(edgeIdSs.str().c_str()));
            agsafeset(n, (char*)"label", (char*)(edgeLabelSs.str().c_str()), (char*)"");

            if (processor_.isColor()) {
                agsafeset(n, (char*)"color", (char*)(colorSs.str().c_str()), (char*)"");
            }

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
                        m = agnode(g, (char*)(edSs.str().c_str()));
                        e = agedge(g, n, m);
                        if (partitions.size() > 1) {
                            agsafeset(e, (char*)"label", (char*)(partSs.str().c_str()), (char*)"");
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
            n = agnode(g, (char*)(nSs.str().c_str()));

            std::stringstream mSs;
            if (lattice_.isLooseVertex(target)) {
                mSs << "L" << lattice_.getLooseVertexIndex(target);
            } else {
                mSs << lattice_.getVertexRawCharIndex(target);
            }
            m = agnode(g, (char*)(mSs.str().c_str()));

            e = agedge(g, n, m);

            agsafeset(e, (char*)"label", (char*)(edgeLabelSs.str().c_str()), (char*)"");

            if (processor_.isColor()) {
                agsafeset(e, (char*)"color", (char*)(colorSs.str().c_str()), (char*)"");
            }

        }

    }

    gvLayoutJobs(gvc, g);
    gvRenderJobs(gvc, g);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);

    try {
        std::string line;
        std::string contents;
        std::ifstream s(tmpFile);
        while (getline(s, line)) {
            contents += line;
            contents += "\n";
        }
        alignOutput_(contents);
        std::remove(tmpFile);
    } catch (...) {
        std::remove(tmpFile);
    }

    DEBUG("WRITING");
}

GVLatticeWriter::Worker::~Worker() {
}
