#include "bracketing_lattice_writer.hpp"


std::string BracketingLatticeWriter::getFormatName() {
    return "bracketing";
}

LatticeWriter<std::ostream>* BracketingLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    PsiQuoter quoter;

    std::map<std::string, std::string> tagsSeparators;
    if (options.count("spec")) {
        std::vector<std::string> spec = options["spec"].as< std::vector<std::string> >();
        std::vector<std::string>::iterator si = spec.begin();
        while (si != spec.end()) {
            std::string tag = quoter.unescape(*si);
            ++si;
            if (si == spec.end()) {
                break;
            }
            tagsSeparators[tag] = quoter.unescape(*si);
            ++si;
        }
    }

    return new BracketingLatticeWriter(
        options.count("linear"),
        options.count("no-alts"),
        options.count("with-blank"),
        quoter.unescape(options["tag"].as<std::string>()),
        quoter.unescape(options["sep"].as<std::string>()),
        quoter.unescape(options["alt-sep"].as<std::string>()),
        tagsSeparators
    );
}

boost::program_options::options_description BracketingLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("alt-sep", boost::program_options::value<std::string>()->default_value("|"),
            "alternative edges separator")
        ("linear",
            "skips cross-edges")
        ("no-alts",
            "skips alternative edges")
        ("with-blank",
            "does not skip edges with whitespace text")
        ("sep", boost::program_options::value<std::string>()->default_value("\n"),
            "basic tag separator")
        ("spec", boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "specification of higher-order tags and their separators")
        ("tag", boost::program_options::value<std::string>()->default_value("token"),
            "basic tag")
        ;

    return optionsDescription;
}


std::string BracketingLatticeWriter::Factory::doGetName() const {
    return "bracketing-writer";
}

boost::filesystem::path BracketingLatticeWriter::Factory::doGetFile() const {
    return __FILE__;
}

std::string BracketingLatticeWriter::doInfo() {
    return "Bracketing writer";
}

BracketingLatticeWriter::Worker::Worker(BracketingLatticeWriter& processor,
                                 std::ostream& outputStream,
                                 Lattice& lattice):
    AligningWriterWorker(outputStream, lattice), processor_(processor) {
}

void BracketingLatticeWriter::Worker::doRun() {

    BracketingLatticeWriterStreamOutputIterator outputIterator(
        getOutputStream(),
        processor_.getAltSeparator(),
        processor_.getBasicTagSeparator()
    );

    std::vector<std::string> handledTags;

    typedef std::pair<std::string, std::string> TagSeparatorPair;
    BOOST_FOREACH(TagSeparatorPair tagSeparatorPair, processor_.getTagsSeparatorsMap()) {
        outputIterator.setSeparator(tagSeparatorPair.first, tagSeparatorPair.second);
        handledTags.push_back(tagSeparatorPair.first);
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

    alignOutputNewline_();

}

BracketingLatticeWriter::Worker::~Worker() {
}
