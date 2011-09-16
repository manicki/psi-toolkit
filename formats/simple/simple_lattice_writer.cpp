#include "simple_lattice_writer.hpp"


std::string SimpleLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter* SimpleLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    std::map<std::string, std::string> tagsSeparators;

    return new SimpleLatticeWriter(
        options.count("linear"),
        options.count("no-alts"),
        options.count("tag") ? (options["tag"].as<std::string>()) : "token",
        options.count("sep") ? (options["sep"].as<std::string>()) : "\n",
        options.count("alt-sep") ? (options["alt-sep"].as<std::string>()) : "|",
        tagsSeparators
    );
}

boost::program_options::options_description SimpleLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("alt-sep", boost::program_options::value<std::string>(), "alternative edges separator")
        ("linear", "skips cross-edges")
        ("no-alts", "skips alternative edges")
        ("sep", boost::program_options::value<std::string>(), "basic tag separator")
        ("tag", boost::program_options::value<std::string>(), "basic tag")
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
