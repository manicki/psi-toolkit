#include "bilexicon.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#include "processor_file_fetcher.hpp"

BiLexicon::BiLexicon(const boost::program_options::variables_map& options) {
    if (options.count("plain-text-lexicon") > 0) {
        ProcessorFileFetcher fileFetcher(__FILE__);

        boost::filesystem::path plainTextLexiconPath =
            fileFetcher.getOneFile(
                options["plain-text-lexicon"].as<std::string>());

        readPlainText_(plainTextLexiconPath);
    }
}

std::string BiLexicon::getName() {
    return "bilexicon";
}

boost::filesystem::path BiLexicon::getFile() {
    return __FILE__;
}

boost::program_options::options_description BiLexicon::optionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    desc.add_options()
        ("binary-lexicon",
         boost::program_options::value<std::string>(),
         "path to the lexicon in the binary format")
        ("plain-text-lexicon",
         boost::program_options::value<std::string>(),
         "path to the lexicon in the plain text format")
        ("save-binary-lexicon",
         boost::program_options::value<std::string>(),
         "as a side effect the lexicon in the binary format is generated");

    return desc;
}

void processEdge(Lattice& /*lattice*/, Lattice::EdgeDescriptor /*edge*/) {

}

void BiLexicon::readPlainText_(const boost::filesystem::path& plainTextLexicon) {
    KeyValueStore::Builder storeBuilder;

    boost::filesystem::ifstream plainTextStream(plainTextLexicon);

    std::string line;
    while (std::getline(plainTextStream, line)) {
        std::vector<std::string> fields;
        boost::split(fields, line, boost::is_any_of("\t "));

        if (fields.size() != 2)
            throw Exception(
                std::string("two fields expected in plain text bilexicon, was: `")
                + line + "`");

        storeBuilder.add(fields[0], fields[1]);
    }

    store_.reset(storeBuilder.build());
}
