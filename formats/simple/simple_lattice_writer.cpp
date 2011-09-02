#include "simple_lattice_writer.hpp"

#include <iomanip>
#include <iostream>

#include "logging.hpp"

std::string SimpleLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter* SimpleLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& vm
) {
    std::map<std::string, std::string> tagsSeparators;
    if (vm.count("tag")) {
        tagsSeparators[vm["tag"].as<std::string>()] = "\n";
    }

    return new SimpleLatticeWriter(
        vm.count("linear"),
        vm.count("no-alts"),
        vm.count("tag") ? (vm["tag"].as<std::string>()) : "token",
        vm.count("sep") ? (vm["sep"].as<std::string>()) : " ",
        vm.count("alt-sep") ? (vm["alt-sep"].as<std::string>()) : "|",
        tagsSeparators
    );
}

boost::program_options::options_description SimpleLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("alt-sep", "alternative edges separator")
        ("linear", "skips cross-edges")
        ("no-alts", "skips alternative edges")
        ("sep", "basic tag separator")
        ("tag", "basic tag")
        ;

    return optionsDescription;
}


std::string SimpleLatticeWriter::Factory::doGetName() {
    return "simple-writer";
}


std::string SimpleLatticeWriter::doInfo() {
    return "Simple writer";
}

SimpleLatticeWriter::Worker::Worker(SimpleLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    WriterWorker(outputStream, lattice), processor_(processor) {
}

void SimpleLatticeWriter::Worker::doRun() {

    Lattice::VertexIterator vi(lattice_);
    Lattice::VertexDescriptor vd = lattice_.getFirstVertex();
    Lattice::EdgeDescriptor edge;

    std::map<std::string, Lattice::VertexDescriptor> targets;
    std::map<std::string, std::string> tagsSeparators;
    for (
        std::map<std::string, std::string>::iterator mi = tagsSeparators.begin();
        mi != tagsSeparators.end();
        ++mi
    ) {
        targets[(*mi).first] = vd;
    }

    while (processor_.isLinear() ? (vd != lattice_.getLastVertex()) : vi.hasNext()) {
        if (!processor_.isLinear()) {
            vd = vi.next();
        }
        std::stringstream vertexSs;
        std::stringstream sepSs;
        Lattice::InOutEdgesIterator oei
            = lattice_.outEdges(vd, lattice_.getLayerTagManager().anyTag());
        while(oei.hasNext()) {
            edge = oei.next();
            std::list<std::string> tags
                = lattice_.getLayerTagManager().getTagNames(lattice_.getEdgeLayerTags(edge));
            for (
                std::list<std::string>::iterator ti = tags.begin();
                ti != tags.end();
                ++ti
            ) {
                if (processor_.getBasicTag() == *ti) {
                    if (vertexSs.str() != "") {
                        vertexSs << processor_.getAltSeparator();
                    }
                    vertexSs << lattice_.getAnnotationCategory(edge);
                }
                if (processor_.isHandledTag(*ti) && targets[*ti] == vd) {
                    targets[*ti] = lattice_.getEdgeTarget(edge);
                    sepSs << processor_.getTagSeparator(*ti);
                }
            }
        }
        alignOutput_(sepSs.str());
        alignOutput_(vertexSs.str());
        alignOutput_(processor_.getBasicTagSeparator());
        if (processor_.isLinear()) {
            vd = lattice_.getEdgeTarget(edge);
        }
    }

}

SimpleLatticeWriter::Worker::~Worker() {
}
