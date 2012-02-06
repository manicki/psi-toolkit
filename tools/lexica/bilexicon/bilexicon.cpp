#include "bilexicon.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

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

void BiLexicon::processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge) {
    std::string edgeText = lattice.getAnnotationText(edge);

    boost::optional<std::string> entryFound = store_->get(edgeText);

    if (entryFound) {
        std::vector<std::string> records;
        boost::split(records, entryFound.get(), boost::is_any_of(";"));

        BOOST_FOREACH(std::string& record, records) {
            addEntry_(lattice, edge, record);
        }
    }
}

void BiLexicon::addEntry_(
    Lattice& lattice, Lattice::EdgeDescriptor edge, const std::string& record) {

    LayerTagCollection tags = lattice.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("lexeme")("bilexicon"));

    lattice.addEdge(
        lattice.getEdgeSource(edge),
        lattice.getEdgeTarget(edge),
        parseRecord_(record),
        tags);

}

AnnotationItem BiLexicon::parseRecord_(const std::string& record) {
    return AnnotationItem(record, StringFrag(record));
}

void BiLexicon::readPlainText_(const boost::filesystem::path& plainTextLexicon) {
    KeyValueStore::Builder storeBuilder;

    boost::filesystem::ifstream plainTextStream(plainTextLexicon);

    std::string line;
    std::string prevKey;
    std::string stringSoFar;
    bool firstEntry = true;

    while (std::getline(plainTextStream, line)) {
        std::vector<std::string> fields;
        boost::split(fields, line, boost::is_any_of("\t "));

        if (fields.size() != 2)
            throw Exception(
                std::string("two fields expected in plain text bilexicon, was: `")
                + line + "`");

        if (firstEntry) {
            prevKey = fields[0];
            stringSoFar = fields[1];
            firstEntry = false;
        } else {
            if (fields[0] == prevKey) {
                stringSoFar += ";";
                stringSoFar += fields[1];
            }
            else {
                storeBuilder.add(prevKey, stringSoFar);
                prevKey = fields[0];
                stringSoFar = fields[1];
            }
        }
    }

    if (!firstEntry)
        storeBuilder.add(prevKey, stringSoFar);

    store_.reset(storeBuilder.build());
}
