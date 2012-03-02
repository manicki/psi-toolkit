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
        options["file"].as<std::string>(),
        options.count("allow-tmp"),
        options.count("tree")
    );
}

boost::program_options::options_description GVLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("allow-tmp",
            "allows to use temporary files")
        ("color",
            "edges with different tags have different colors")
        ("file", boost::program_options::value<std::string>()->default_value(""),
            "output file name")
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

    std::map<Lattice::EdgeDescriptor, std::string> edgeDescriptionMap;

    char * tmpFile;

    GVC_t * gvc = gvContext();
    std::string arg1("-T" + processor_.getOutputFormat());
    std::string arg2("");
    if (processor_.isUseOutputStream()) {
        tmpFile = tempnam(NULL, "gv_");
        arg2 += "-o";
        arg2 += tmpFile;
    } else {
        if (!processor_.getOutputFile().empty()) {
            arg2 += "-o";
        }
        arg2 += processor_.getOutputFile();
    }
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

            edgeDescriptionMap[edge] = edgeLabelSs.str();

            n = agnode(g, (char*)(edgeLabelSs.str().c_str()));

            if (processor_.isColor()) {
                agsafeset(n, (char*)"color", (char*)(colorSs.str().c_str()), (char*)"");
            }

            std::list<Lattice::Partition> partitions = lattice_.getEdgePartitions(edge);
            BOOST_FOREACH(Lattice::Partition partition, partitions) {
                Lattice::Partition::Iterator ei(lattice_, partition);
                while (ei.hasNext()) {
                    Lattice::EdgeDescriptor ed = ei.next();
                    std::map<Lattice::EdgeDescriptor, std::string>::iterator mi = edgeDescriptionMap.find(ed);
                    if (mi != edgeDescriptionMap.end()) {
                        m = agnode(g, (char*)((*mi).second.c_str()));
                        e = agedge(g, n, m);
                    }
                }
                break;
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

    if (processor_.isUseOutputStream()) {
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
    }

    DEBUG("WRITING");
}

GVLatticeWriter::Worker::~Worker() {
}
