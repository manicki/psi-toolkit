#include "perl_simple_lattice_writer.hpp"

#include "logging.hpp"

std::string PerlSimpleLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter<PerlLatticeWriterOutput>* PerlSimpleLatticeWriter::Factory::doCreateLatticeWriter(
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

    return new PerlSimpleLatticeWriter(
        options.count("linear"),
        options.count("no-alts"),
        options.count("with-blank"),
        quoter.unescape(options["tag"].as<std::string>()),
        higherOrderTags
        );
}

boost::program_options::options_description PerlSimpleLatticeWriter::Factory::doOptionsHandled() {
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
        ;

    return optionsDescription;
}


std::string PerlSimpleLatticeWriter::Factory::doGetName() {
    return "perl-simple-writer";
}


std::string PerlSimpleLatticeWriter::doInfo() {
    return "Perl simple writer";
}

PerlSimpleLatticeWriter::Worker::Worker(PerlSimpleLatticeWriter & processor,
                                 PerlLatticeWriterOutput & output,
                                 Lattice& lattice):
    WriterWorker<PerlLatticeWriterOutput>(output, lattice),
    processor_(processor) {
}

void PerlSimpleLatticeWriter::Worker::doRun() {

    PerlSimpleLatticeWriterOutputIterator outputIterator(
        getOutputStream()
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

PerlSimpleLatticeWriter::Worker::~Worker() {
}
