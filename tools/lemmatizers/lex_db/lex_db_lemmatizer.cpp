#include "lex_db_lemmatizer.hpp"

#include <pqxx/transaction>

LexDbLemmatizer::LexDbLemmatizer():connection_("dbname=upl-copy") {
    connection_.set_client_encoding("UTF8");
}

void LexDbLemmatizer::lemmatize(const std::string token,
                               AnnotationItemManager& annotationItemManager,
                               LemmatizerOutputIterator& outputIterator) {
    pqxx::work transaction(connection_);

    pqxx::result tuples =
        transaction.exec(
            "SELECT canon,name,getposprefix(sinflection),M.morphology"
            " FROM forms F, lexemes L, morphologies M"
            " WHERE F.lexeme_sn = L.lexeme_sn AND M.morphology_no = F.morphology_no"
            " AND (discard is null OR discard = 0)"
            " AND form = '" + pqxx::to_string(token) + "'"
            " ORDER BY canon, name, priority");

    std::string currentLemma;
    std::string currentLexeme;

    static pqxx::result::tuple::size_type CANON_INDEX = 0;
    static pqxx::result::tuple::size_type NAME_INDEX = 1;
    static pqxx::result::tuple::size_type PART_OF_SPEECH_INDEX = 2;
    static pqxx::result::tuple::size_type MORPHOLOGY_INDEX = 3;

    for (pqxx::result::const_iterator iter = tuples.begin();
         iter != tuples.end();
         ++iter) {

        const pqxx::result::tuple& tuple = *iter;

        if (tuple[CANON_INDEX].c_str() != currentLemma) {
            currentLemma = tuple[CANON_INDEX].c_str();
            outputIterator.addLemma(currentLemma);
        }

        if (tuple[NAME_INDEX].c_str() != currentLexeme) {
            currentLexeme = tuple[NAME_INDEX].c_str();

            AnnotationItem lexeme(
                tuple[PART_OF_SPEECH_INDEX].c_str(),
                currentLexeme);

            outputIterator.addLexeme(lexeme);
        }

        AnnotationItem form(
            tuple[PART_OF_SPEECH_INDEX].c_str(),
            currentLexeme);

        annotationItemManager.setValue(form, "morpho", tuple[MORPHOLOGY_INDEX].c_str());

        outputIterator.addForm(form);

    }
}

std::string LexDbLemmatizer::getName() {
    return "lex-db";
}

std::list<std::string> LexDbLemmatizer::getLayerTags() {
    std::list<std::string> layerTags;

    layerTags.push_back("lex-db");
    layerTags.push_back("lexdb-tagset");

    return layerTags;
}
