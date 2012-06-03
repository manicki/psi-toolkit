#include "config.hpp"
#include "lexicon_base.hpp"
#include "logging.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

void LexiconBase::readPlainText(const boost::filesystem::path& plainTextLexicon) {

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

void LexiconBase::saveBinary(const boost::filesystem::path& binaryLexiconPath) {
    WARN("saving lexicon to `" << binaryLexiconPath.string() << "`");

    store_->save(binaryLexiconPath.string());

    WARN("lexicon saved");
}

void LexiconBase::loadBinary(const boost::filesystem::path& binaryLexiconPath) {
    store_.reset(new KeyValueStore());
    store_->load(binaryLexiconPath.string());
}

bool LexiconBase::isEmpty() {
    return !store_;
}

std::vector<std::string> LexiconBase::getRecords(const std::string& text) {
    boost::optional<std::string> entryFound = store_->get(text);
    std::vector<std::string> records;

    if (entryFound) {
        boost::split(records, entryFound.get(), boost::is_any_of(";"));
    }

    return records;
}

void LexiconBase::removeComment_(std::string& s) {
    size_t hashPos = s.find_first_of('#');

    if (hashPos != std::string::npos)
        s = s.substr(0, hashPos);
}

bool LexiconBase::isEmptyLine_(const std::string& s) {
    return s.find_first_not_of(" \t");
}
