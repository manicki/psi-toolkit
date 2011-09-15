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

    SimpleLatticeWriterStreamOutputIterator outputIterator(
        getOutputStream(),
        processor_.getAltSeparator(),
        processor_.getBasicTagSeparator()
    );

    std::vector<std::string> handledTags;

    for (
        std::map<std::string, std::string>::iterator mi = processor_.getTagsSeparatorsMap().begin();
        mi != processor_.getTagsSeparatorsMap().end();
        ++mi
    ) {
        outputIterator.setSeparator((*mi).first, (*mi).second);
        handledTags.push_back((*mi).first);
    }

    LatticeIterWriter writer(
        lattice_,
        outputIterator,
        processor_.isLinear(),
        processor_.getBasicTag(),
        handledTags
    );

    writer.run();

/*
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

    std::stringstream allSs;
    std::stringstream blockSs;

    while (processor_.isLinear() ? true : vi.hasNext()) {
        if (!processor_.isLinear()) {
            vd = vi.next();
        }
        bool areEdgesToWrite = false;
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
                    areEdgesToWrite = true;
                    if (vertexSs.str() != "") {
                        vertexSs << processor_.getAltSeparator();
                    }
                    if (lattice_.getAnnotationText(edge) == "") {
                        vertexSs << lattice_.getAnnotationCategory(edge);
                    } else {
                        vertexSs << lattice_.getAnnotationText(edge);
                    }
                }
                if (processor_.isHandledTag(*ti) && targets[*ti] == vd) {
                    areEdgesToWrite = true;
                    targets[*ti] = lattice_.getEdgeTarget(edge);
                    sepSs << processor_.getTagSeparator(*ti);
                }
            }
        }

        if (sepSs.str() != "") {
            allSs << blockSs.str();
            if (allSs.str() != "") {
                allSs << sepSs.str();
            }
            blockSs.str("");
        }
        if (blockSs.str() != "" && areEdgesToWrite) {
            blockSs << processor_.getBasicTagSeparator();
        }
        blockSs << vertexSs.str();
        if (processor_.isLinear()) {
            try {
                vd = lattice_.getEdgeTarget(
                    lattice_.firstOutEdge(vd, lattice_.getLayerTagManager().anyTag())
                );
            } catch (NoEdgeException) {
                break;
            }
        }
    }
    alignOutput_(allSs.str());
    alignOutput_(blockSs.str());
    alignOutputNewline_();
*/
}

SimpleLatticeWriter::Worker::~Worker() {
}
