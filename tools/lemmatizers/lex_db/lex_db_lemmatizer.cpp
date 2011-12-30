#include "lex_db_lemmatizer.hpp"

#include <pqxx/transaction>
#include <pqxx/tuple>
#include <boost/program_options.hpp>

#include "logging.hpp"

LexDbLemmatizer::LexDbLemmatizer(const boost::program_options::variables_map& options)
    :connection_(std::string("dbname=")+options["db-name"].as<std::string>()) {
    connection_.set_client_encoding("UTF8");
}

void LexDbLemmatizer::lemmatize(const std::string& token,
                               AnnotationItemManager& annotationItemManager,
                               LemmatizerOutputIterator& outputIterator) {
    pqxx::work transaction(connection_);

    pqxx::result tuples =
        transaction.exec(
            "SELECT canon,name,sinflection,M.morphology"
            " FROM forms F, lexemes L, morphologies M"
            " WHERE F.lexeme_sn = L.lexeme_sn AND M.morphology_no = F.morphology_no"
            " AND (discard is null OR discard = 0)"
            " AND form = '" + pqxx::to_string(token) + "'"
            " ORDER BY canon, name, priority");

    std::string currentLemma;
    std::string currentLexeme;

    static pqxx::tuple::size_type CANON_INDEX = 0;
    static pqxx::tuple::size_type NAME_INDEX = 1;
    static pqxx::tuple::size_type SINFLECTION_INDEX = 2;
    static pqxx::tuple::size_type MORPHOLOGY_INDEX = 3;

    for (pqxx::result::const_iterator iter = tuples.begin();
         iter != tuples.end();
         ++iter) {

        const pqxx::tuple& tuple = *iter;

        if (tuple[CANON_INDEX].c_str() != currentLemma) {
            currentLemma = tuple[CANON_INDEX].c_str();
            outputIterator.addLemma(currentLemma);
        }

        if (tuple[NAME_INDEX].c_str() != currentLexeme) {
            currentLexeme = tuple[NAME_INDEX].c_str();

            std::string partOfSpeech;
            std::string flags;

            parseSinflection_(tuple[SINFLECTION_INDEX].c_str(),
                              partOfSpeech,
                              flags);

            AnnotationItem lexeme(
                partOfSpeech,
                StringFrag(currentLexeme));

            annotationItemManager.setValue(lexeme, "flags", flags);

            outputIterator.addLexeme(lexeme);
        }

        {
            std::string partOfSpeech;
            std::string flags;

            parseSinflection_(tuple[SINFLECTION_INDEX].c_str(),
                              partOfSpeech,
                              flags);

            AnnotationItem form(
                partOfSpeech,
                StringFrag(currentLexeme));

            annotationItemManager.setValue(form, "flags", flags);

            annotationItemManager.setValue(form, "morpho", tuple[MORPHOLOGY_INDEX].c_str());

            outputIterator.addForm(form);
        }
    }
}

boost::program_options::options_description LexDbLemmatizer::optionsHandled() {
    boost::program_options::options_description desc;

    desc.add_options()
        ("db-name", boost::program_options::value<std::string>(), "database name")
        ;

    return desc;
}

std::string LexDbLemmatizer::getName() {
    return "lex-db";
}

boost::filesystem::path LexDbLemmatizer::getFile() {
    return __FILE__;
}

std::string LexDbLemmatizer::getLanguage() const {
    return "pl";
}

std::list<std::string> LexDbLemmatizer::getLayerTags() {
    std::list<std::string> layerTags;

    layerTags.push_back("lex-db");
    layerTags.push_back("lexdb-tagset");

    return layerTags;
}

void LexDbLemmatizer::parseSinflection_(const std::string& sinflection,
                                        std::string& partOfSpeech,
                                        std::string& flags) {
    size_t i = sinflection.rfind(':');
    if(i != std::string::npos)
    {
        partOfSpeech = sinflection.substr(0,i);
        flags = sinflection.substr(i+1);
    }
    else {
        partOfSpeech = sinflection;
        flags = "";
    }

}

