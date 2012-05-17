#include "bracketing_lattice_writer.hpp"


std::string BracketingLatticeWriter::getFormatName() {
    return "bracketing";
}


LatticeWriter<std::ostream>* BracketingLatticeWriter::Factory::doCreateLatticeWriter(
    const boost::program_options::variables_map& options
) {
    std::vector<std::string> filter;
    std::vector<std::string> filterAttributes;

    if (options.count("show-only-tags")) {
        filter = options["show-only-tags"].as< std::vector<std::string> >();
    } else if (options.count("filter")) {
        filter = options["filter"].as< std::vector<std::string> >();
    }

    if (options.count("filter-attributes")) {
        filterAttributes = options["filter-attributes"].as< std::vector<std::string> >();
    }

    return new BracketingLatticeWriter(
        options["opening-bracket"].as<std::string>(),
        options["closing-bracket"].as<std::string>(),
        options["tag-separator"].as<std::string>(),
        filter,
        options["av-pairs-separator"].as<std::string>(),
        options["av-separator"].as<std::string>(),
        filterAttributes
    );
}


boost::program_options::options_description BracketingLatticeWriter::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("opening-bracket",
            boost::program_options::value<std::string>()->default_value("["),
            "the actual format of opening brackets")
        ("closing-bracket",
            boost::program_options::value<std::string>()->default_value("]"),
            "the actual format of closing brackets")
        ("tag-separator",
            boost::program_options::value<std::string>()->default_value(","),
            "separates tags")
        ("show-only-tags",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "limits the tag names that will appear in `%T` substitions")
        ("filter",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "limits the tag names that will appear in `%T` substitions")
        ("av-pairs-separator",
            boost::program_options::value<std::string>()->default_value(","),
            "separates the attribute-value pairs")
        ("av-separator",
            boost::program_options::value<std::string>()->default_value("="),
            "separates the attribute and its value")
        ("filter-attributes",
            boost::program_options::value< std::vector<std::string> >()->multitoken(),
            "the attributes to be shown")
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
    alignOutput_("bracketing-writer: TODO");
    alignOutputNewline_();
}


BracketingLatticeWriter::Worker::~Worker() {
}
