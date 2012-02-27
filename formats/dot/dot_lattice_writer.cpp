#include "dot_lattice_writer.hpp"

#include <iostream>

#include "lattice.hpp"
#include "logging.hpp"

std::string DotLatticeWriter::getFormatName() {
    return "DOT";
}

LatticeWriter<std::ostream>* DotLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map&) {
    return new DotLatticeWriter();
}

boost::program_options::options_description DotLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ;

    return optionsDescription;
}


std::string DotLatticeWriter::Factory::doGetName() {
    return "dot-writer";
}

boost::filesystem::path DotLatticeWriter::Factory::doGetFile() {
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

    Lattice::EdgesSortedByTargetIterator ei
        = lattice_.edgesSortedByTarget(lattice_.getLayerTagManager().anyTag());

    alignOutput_("digraph G {");
    alignOutputNewline_();

    alignOutput_("rankdir=LR");
    alignOutputNewline_();

    while (ei.hasNext()) {

        Lattice::EdgeDescriptor edge = ei.next();

        if (lattice_.isEdgeHidden(edge)) continue;

        std::stringstream edgeSs;

        Lattice::VertexDescriptor source = lattice_.getEdgeSource(edge);
        if (lattice_.isLooseVertex(source)) {
            edgeSs << "@" << lattice_.getLooseVertexIndex(source);
        } else {
            edgeSs << lattice_.getVertexRawCharIndex(source);
        }

        edgeSs << " -> ";

        Lattice::VertexDescriptor target = lattice_.getEdgeTarget(edge);
        if (lattice_.isLooseVertex(target)) {
            edgeSs << "@" << lattice_.getLooseVertexIndex(target);
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
        edgeSs << " [label=\"" << edgeText << "\"]";

        alignOutput_(edgeSs.str());
        alignOutputNewline_();

    }

    alignOutput_("}");
    alignOutputNewline_();

    DEBUG("WRITING");
}

DotLatticeWriter::Worker::~Worker() {
}
