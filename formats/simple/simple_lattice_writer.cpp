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
        tagsSeparators
    );
}

boost::program_options::options_description SimpleLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("linear", "skips cross-edges")
        ("no-alts", "skips alternative edges")
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

    //TODO

}

SimpleLatticeWriter::Worker::~Worker() {
}
