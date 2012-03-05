#include "abstract_simple_data_lattice_writer.hpp"

std::string AbstractSimpleDataLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter<AbstractSimpleDataLatticeWriterOutput>* AbstractSimpleDataLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    PsiQuoter quoter;

    std::set<std::string> higherOrderTags;
    if (options.count("spec")) {
        std::vector<std::string> spec = options["spec"].as< std::vector<std::string> >();
        std::vector<std::string>::iterator si = spec.begin();
        while (si != spec.end()) {
            std::string tag = quoter.unescape(*si);
            higherOrderTags.insert(tag);

            ++si;
        }
    }

    return createLatticeWriter(
                               options.count("linear"),
                               options.count("no-alts"),
                               options.count("with-blank"),
                               quoter.unescape(options["tag"].as<std::string>()),
                               higherOrderTags,
                               options.count("with-args")
                               );
}

boost::program_options::options_description AbstractSimpleDataLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("linear",
            "skips cross-edges")
        ("no-alts",
            "skips alternative edges")
        ("with-blank",
            "does not skip edges with whitespace text")
        ("tag", boost::program_options::value<std::string>()->default_value("token"),
            "basic tag")
        ("spec", boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "specification of higher-order tags")
        ("with-args",
         "if set, then returns text with annotation as a hash element")
        ;

    return optionsDescription;
}


AbstractSimpleDataLatticeWriter::Worker::Worker(AbstractSimpleDataLatticeWriter & processor,
                                 AbstractSimpleDataLatticeWriterOutput & output,
                                 Lattice& lattice):
    WriterWorker<AbstractSimpleDataLatticeWriterOutput>(output, lattice),
    processor_(processor) {
}

void AbstractSimpleDataLatticeWriter::Worker::doRun() {

    AbstractSimpleDataLatticeWriterOutputIterator outputIterator(
                                                  getOutputStream(),
                                                  processor_.isWithArgs()
        );

    std::vector<std::string> handledTags;
    BOOST_FOREACH(std::string higherOrderTag, processor_.getHigherOrderTags()) {
        handledTags.push_back(higherOrderTag);
    }

    LatticeIterWriter writer(
        lattice_,
        outputIterator,
        processor_.isLinear(),
        processor_.isNoAlts(),
        processor_.isWithBlank(),
        processor_.getBasicTag(),
        handledTags
    );

    writer.run();
}

AbstractSimpleDataLatticeWriter::Worker::~Worker() {
}

