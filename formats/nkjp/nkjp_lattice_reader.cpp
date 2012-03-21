#include "nkjp_lattice_reader.hpp"

#include <cstring>
#include <locale>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

#include "xml_property_tree.hpp"

#include "logging.hpp"


std::string NKJPLatticeReader::getFormatName() {
    return "NKJP";
}

std::string NKJPLatticeReader::doInfo() {
    return "NKJP reader";
}

NKJPLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* NKJPLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& options
) {
    return new NKJPLatticeReader(
        options["layer"].as<std::string>()
    );
}

boost::program_options::options_description NKJPLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("layer", boost::program_options::value<std::string>()->default_value("morphosyntax"),
            "specifies the annotation layer represented by the given NKJP file")
        ;

    return optionsDescription;
}

std::string NKJPLatticeReader::Factory::doGetName() {
    return "nkjp-reader";
}

boost::filesystem::path NKJPLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

NKJPLatticeReader::Worker::Worker(
    NKJPLatticeReader& processor,
    std::istream& inputStream,
    Lattice& lattice
) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("token")("nkjp-reader")
    ))
{ }

void NKJPLatticeReader::Worker::doRun() {
    // SET_LOGGING_LEVEL("DEBUG");
    XmlPropertyTree xpt(inputStream_);
    BOOST_FOREACH(
        boost::property_tree::ptree::value_type &vP,
        xpt.get_child("teiCorpus.TEI.text.body")
    ) {
        if (strcmp(vP.first.data(), "p") == 0) {
            BOOST_FOREACH(
                boost::property_tree::ptree::value_type &vS,
                vP.second.get_child("")
            ) {
                if (strcmp(vS.first.data(), "s") == 0) {
                    BOOST_FOREACH(
                        boost::property_tree::ptree::value_type &vSeg,
                        vS.second.get_child("")
                    ) {
                        if (strcmp(vSeg.first.data(), "seg") == 0) {
                            try {
                                std::string segment(vSeg.second.get<std::string>("fs.f.string"));
                                appendSegmentToLattice_(segment);
                            } catch (boost::property_tree::ptree_error) {
                                throw PsiException("nkjp-reader: input file error");
                            }
                        }
                    }
                }
            }
        }
    }
}

void NKJPLatticeReader::Worker::appendSegmentToLattice_(std::string segment) {

    //temporary solution for inserting spaces
    std::locale loc;
    if (isalpha(segment.at(0), loc)) {
        Lattice::VertexDescriptor sFrom = lattice_.getLastVertex();
        lattice_.appendStringWithSymbols(" ");
        Lattice::VertexDescriptor sTo = lattice_.getLastVertex();
        std::string blank(" ");
        AnnotationItem sItem("B", blank);
        Lattice::EdgeSequence::Builder sBuilder(lattice_);
        sBuilder.addEdge(lattice_.firstOutEdge(
            sFrom,
            lattice_.getLayerTagManager().getMask("symbol")
        ));
        lattice_.addEdge(sFrom, sTo, sItem, textTags_, sBuilder.build());
    }

    Lattice::VertexDescriptor from = lattice_.getLastVertex();
    lattice_.appendStringWithSymbols(segment);
    Lattice::VertexDescriptor to = lattice_.getLastVertex();
    AnnotationItem item("seg", segment);
    Lattice::EdgeSequence::Builder seqBuilder(lattice_);
    Lattice::VertexDescriptor currentVertex = from;
    Lattice::EdgeDescriptor currentEdge;
    while (currentVertex != to) {
        currentEdge = lattice_.firstOutEdge(
            currentVertex,
            lattice_.getLayerTagManager().getMask("symbol")
        );
        seqBuilder.addEdge(currentEdge);
        currentVertex = lattice_.getEdgeTarget(currentEdge);
    }
    lattice_.addEdge(from, to, item, textTags_, seqBuilder.build());
}
