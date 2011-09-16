#include "simple_lattice_writer.hpp"


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
        vm.count("sep") ? (vm["sep"].as<std::string>()) : "\n",
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

    std::map<std::string, std::string> tagsSeparatorsMap = processor_.getTagsSeparatorsMap();
    for (
        std::map<std::string, std::string>::iterator mi = tagsSeparatorsMap.begin();
        mi != tagsSeparatorsMap.end();
        ++mi
    ) {
        outputIterator.setSeparator((*mi).first, (*mi).second);
        handledTags.push_back((*mi).first);
    }

    LatticeIterWriter writer(
        lattice_,
        outputIterator,
        processor_.isLinear(),
        processor_.isNoAlts(),
        processor_.getBasicTag(),
        handledTags
    );

    writer.run();

    alignOutputNewline_();

}

SimpleLatticeWriter::Worker::~Worker() {
}
