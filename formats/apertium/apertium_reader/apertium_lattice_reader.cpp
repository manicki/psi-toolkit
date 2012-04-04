#include <boost/algorithm/string/trim.hpp>
#include <boost/assign/list_of.hpp>

#include "apertium_lattice_reader.hpp"
#include "processor_file_fetcher.cpp"


std::string ApertiumLatticeReader::getFormatName() {
    return "Apertium";
}

std::string ApertiumLatticeReader::doInfo() {
    return "Apertium reader";
}

std::string ApertiumLatticeReader::processReplacementRules(const std::string& input) {
    return apertiumDeformatter_.processReplacementRules(input);
}

std::vector<DeformatIndex> ApertiumLatticeReader::processFormatRules(const std::string& input) {
    return apertiumDeformatter_.processFormatRules(input);
}

const std::string ApertiumLatticeReader::Factory::DEFAULT_SPEC_FILES_DIR = "%ITSDATA%/";
const std::string ApertiumLatticeReader::Factory::DEFAULT_SPEC_FILE_ENDING = "-format.xml";

ApertiumLatticeReader::ApertiumLatticeReader(const boost::filesystem::path& specificationFile)
    : apertiumDeformatter_(specificationFile) { }

ApertiumLatticeReader::Factory::~Factory() { }

LatticeReader<std::istream>* ApertiumLatticeReader::Factory::doCreateLatticeReader(
    const boost::program_options::variables_map& options) {

    std::string specFilePath;

    if (options.count("specification-file")) {
        specFilePath = options["specification-file"].as<std::string>();
    }
    else {
        specFilePath = DEFAULT_SPEC_FILES_DIR + "/"
            + options["format"].as<std::string>() + DEFAULT_SPEC_FILE_ENDING;
    }

    ProcessorFileFetcher fileFetcher(__FILE__);
    boost::filesystem::path specificationFile = fileFetcher.getOneFile(specFilePath);

    return new ApertiumLatticeReader(specificationFile);
}

boost::program_options::options_description ApertiumLatticeReader::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("format", boost::program_options::value<std::string>()
            ->default_value("html"),
            "type of file to deformatting")
        ("specification-file", boost::program_options::value<std::string>(),
            "specification file path");

    return optionsDescription;
}

std::string ApertiumLatticeReader::Factory::doGetName() {
    return "apertium-reader";
}

boost::filesystem::path ApertiumLatticeReader::Factory::doGetFile() {
    return __FILE__;
}

ApertiumLatticeReader::Worker::Worker(ApertiumLatticeReader& processor,
                                      std::istream& inputStream,
                                      Lattice& lattice) :
    ReaderWorker<std::istream>(inputStream, lattice),
    processor_(processor),
    fragTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("frag")("apertium-reader"))),
    tagTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("tag")("apertium-reader"))) {
}

void ApertiumLatticeReader::Worker::doRun() {
    DEBUG("starting apertium reader...");

    std::string input;
    std::string line;

    while (std::getline(inputStream_, line)) {
        if (boost::algorithm::trim_copy(line).empty()) {
            continue;
        }
        input += line;// + "\n";
    }

    input = processor_.processReplacementRules(input);
    std::vector<DeformatIndex> indexes = processor_.processFormatRules(input);

    if (indexes.size() != 0) {
        std::string text;
        unsigned int length;

        if (indexes[0].begin != 0) {
            text = input.substr(0, indexes[0].begin);
            DEBUG("frag: " << text);
            appendFragmentToLattice_(text);
        }

        for (unsigned int i = 0; i < indexes.size(); i++) {
            text = input.substr(indexes[i].begin, indexes[i].length());
            DEBUG("tag: " << text << " [" << indexes[i].type << ", " << indexes[i].eos << "]");
            appendTagToLattice_(text, indexes[i].type, indexes[i].eos);

            length = (i == indexes.size() - 1) ? input.length() : indexes[i+1].begin;
            length -= indexes[i].end;

            if (length != 0) {
                text = input.substr(indexes[i].end, length);
                DEBUG("frag: " << text);
                appendFragmentToLattice_(text);
            }
        }

    }

    std::string output = "";
    DEBUG(output);
}

void ApertiumLatticeReader::Worker::appendFragmentToLattice_(std::string fragment) {
    Lattice::VertexDescriptor prevEnd = lattice_.getLastVertex();
    lattice_.appendStringWithSymbols(fragment);
    Lattice::VertexDescriptor nowEnd = lattice_.getLastVertex();
    AnnotationItem item("FRAG", fragment);
    lattice_.addEdge(prevEnd, nowEnd, item, fragTags_);
}

void ApertiumLatticeReader::Worker::appendTagToLattice_(
    std::string tag, std::string type, bool eos) {

    Lattice::VertexDescriptor prevEnd = lattice_.getLastVertex();
    lattice_.appendStringWithSymbols(tag);
    Lattice::VertexDescriptor nowEnd = lattice_.getLastVertex();

    AnnotationItem item(type + "-tag", StringFrag(tag));
    AnnotationItemManager& manager = lattice_.getAnnotationItemManager();
    manager.setValue(item, "eos", eos ? "yes" : "no");

    lattice_.addEdge(prevEnd, nowEnd, item, tagTags_);
}
