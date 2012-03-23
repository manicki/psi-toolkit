#include "perl_simple_lattice_writer.hpp"

#if HAVE_PERL_BINDINGS

#include "logging.hpp"

std::string PerlSimpleLatticeWriter::getFormatName() {
    return "simple";
}

LatticeWriter<Sink> * PerlSimpleLatticeWriter::Factory::doCreateLatticeWriter(
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
        higherOrderTags,
        options.count("with-args")
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
        ("with-args",
         "if set, then returns text with annotation as a hash element")
        ;

    return optionsDescription;
}

std::string PerlSimpleLatticeWriter::Factory::doGetName() {
    return "perl-simple-writer";
}

boost::filesystem::path PerlSimpleLatticeWriter::Factory::doGetFile() {
    return __FILE__;
}

LatticeWriter<AbstractSimpleDataLatticeWriterOutput>* PerlSimpleLatticeWriter::Factory::createLatticeWriter(
           bool linear,
           bool noAlts,
           bool withBlank,
           std::string basicTag,
           std::set<std::string> higherOrderTags,
           bool withArgs ) {

    return new PerlSimpleLatticeWriter(
           linear,
           noAlts,
           withBlank,
           basicTag,
           higherOrderTags,
           withArgs );
}

PerlSimpleLatticeWriter::Worker::Worker(PerlSimpleLatticeWriter & processor,
                                 Sink & output,
                                 Lattice& lattice):
    WriterWorker<Sink>(output, lattice),
    processor_(processor) {
}

std::string PerlSimpleLatticeWriter::doInfo() {
    return "Perl simple writer";
}

PerlSimpleLatticeWriter::Worker::~Worker() {
}

#endif
