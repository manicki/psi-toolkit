#include "bilexicon.hpp"
#include "config.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>

#include "processor_file_fetcher.hpp"

BiLexicon::BiLexicon(const boost::program_options::variables_map& options) {
    ProcessorFileFetcher fileFetcher(__FILE__);

    if (options.count("plain-text-lexicon") > 0) {
        if (options.count("binary-lexicon") > 0)
            throw new Exception(
                "either --plain-text-lexicon or --binary-lexicon expected, not both");


        boost::filesystem::path plainTextLexiconPath =
            fileFetcher.getOneFile(
                options["plain-text-lexicon"].as<std::string>());

        readPlainText_(plainTextLexiconPath);
    } else if (options.count("binary-lexicon") > 0) {
        boost::filesystem::path binaryLexiconPath =
            fileFetcher.getOneFile(
                options["binary-lexicon"].as<std::string>());

        loadBinary_(binaryLexiconPath);
    }

    if (options.count("save-binary-lexicon") > 0) {
        if (!store_)
            throw new Exception("no data to save");

        boost::filesystem::path binaryLexiconPath(
            options["save-binary-lexicon"].as<std::string>());

        saveBinary_(binaryLexiconPath);
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

std::list<std::string> BiLexicon::providedLayerTags() {
    return boost::assign::list_of
        (std::string("lexeme"))
        (std::string("bilexicon"))
        (std::string("!translation"));
}

std::list<std::string> BiLexicon::tagsToOperateOn() {
    return boost::assign::list_of
        (std::string("lexeme"));
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
        providedLayerTags());

    Lattice::EdgeSequence::Builder builder(lattice);
    builder.addEdge(edge);

    lattice.addEdge(
        lattice.getEdgeSource(edge),
        lattice.getEdgeTarget(edge),
        parseRecord_(record),
        tags,
        builder.build());
}

AnnotationItem BiLexicon::parseRecord_(const std::string& record) {
    size_t separatorPos = record.find_first_of(LEMMA_CATEGORY_SEPARATOR);

    std::string category =
        (separatorPos == std::string::npos
         ? record
         : record.substr(separatorPos + 1));

    return AnnotationItem(category, StringFrag(record));
}

void BiLexicon::readPlainText_(const boost::filesystem::path& plainTextLexicon) {

    KeyValueStore::Builder storeBuilder;

    boost::filesystem::ifstream plainTextStream(plainTextLexicon);

    std::string line;
    std::string prevKey;
    std::string stringSoFar;
    bool firstEntry = true;

    while (std::getline(plainTextStream, line)) {
        removeComment_(line);

        if (isEmptyLine_(line))
            continue;

        std::vector<std::string> fields;
        boost::split(fields, line, boost::is_any_of("\t "));
        fields.erase(std::remove_if(
                         fields.begin(), fields.end(),
                         boost::bind( &std::string::empty, _1 )), fields.end());

        BOOST_FOREACH(std::string& field, fields) {
            field = quoter_.unescape(field);
        }

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

void BiLexicon::saveBinary_(const boost::filesystem::path& binaryLexiconPath) {
    WARN("saving lexicon to `" << binaryLexiconPath.string() << "`");

    store_->save(binaryLexiconPath.string());

    WARN("lexicon saved");
}

void BiLexicon::loadBinary_(const boost::filesystem::path& binaryLexiconPath) {
    store_.reset(new KeyValueStore());
    store_->load(binaryLexiconPath.string());
}

void BiLexicon::removeComment_(std::string& s) {
    size_t hashPos = s.find_first_of('#');

    if (hashPos != std::string::npos)
        s = s.substr(0, hashPos);
}

bool BiLexicon::isEmptyLine_(const std::string& s) {
    return s.find_first_not_of(" \t");
}
