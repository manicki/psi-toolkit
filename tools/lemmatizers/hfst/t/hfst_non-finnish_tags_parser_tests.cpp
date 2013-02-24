#include "tests.hpp"

#include <boost/program_options/parsers.hpp>
#include <map>
#include <vector>
#include <boost/assign.hpp>
#include "config.hpp"
#include "hfst_english_tags_parser.hpp"
#include "hfst_french_tags_parser.hpp"
#include "hfst_german_tags_parser.hpp"
#include "hfst_italian_tags_parser.hpp"
#include "hfst_swedish_tags_parser.hpp"
#include "hfst_turkish_tags_parser.hpp"

BOOST_AUTO_TEST_SUITE( hfst_nonfinnish_tags_parser )

void testTagsParser(HfstTagsParser* parser, HfstRawTags tags,
        HfstTags expectedLexemeAttributes,
        HfstTags expectedFormAttributes);

BOOST_AUTO_TEST_CASE ( english_tags_parser ) {
    boost::program_options::variables_map noOptions;
    HfstTagsParser* parser = dynamic_cast<HfstEnglishTagsParser*>(new HfstEnglishTagsParser(noOptions));
    HfstTags expectedLexemeAttribsGreenV =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "v"));
    HfstTags expectedFormAttribsGreenV =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "v")
             ("tense", "inf"));
    HfstRawTags tagsGreenV = boost::assign::list_of
        ("V")
        ("V")
        ("INF");
    testTagsParser(parser, tagsGreenV, expectedLexemeAttribsGreenV,
            expectedFormAttribsGreenV);
    HfstTags expectedLexemeAttribsGreenN =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "n"));
    HfstTags expectedFormAttribsGreenN =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "n"));
    HfstRawTags tagsGreenN = boost::assign::list_of
        ("N")
        ("N");
    testTagsParser(parser, tagsGreenN, expectedLexemeAttribsGreenN,
            expectedFormAttribsGreenN);
    HfstTags expectedLexemeAttribsGreenAdj =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adj"));
    HfstTags expectedFormAttribsGreenAdj =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adj"));
    HfstRawTags tagsGreenAdj = boost::assign::list_of
        ("ADJ")
        ("ADJ");
    testTagsParser(parser, tagsGreenAdj, expectedLexemeAttribsGreenAdj,
            expectedFormAttribsGreenAdj);
    HfstTags expectedLexemeAttribsGreener =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adj"));
    HfstTags expectedFormAttribsGreener =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("degree", "comp")
             ("pos", "adj"));
    HfstRawTags tagsGreener = boost::assign::list_of
        ("ADJ")
        ("ADJ")
        ("COMP");
    testTagsParser(parser, tagsGreener, expectedLexemeAttribsGreener,
            expectedFormAttribsGreener);
    delete parser;
}

BOOST_AUTO_TEST_CASE ( german_tags_parser ) {
    boost::program_options::variables_map noOptions;
    HfstTagsParser* parser = dynamic_cast<HfstGermanTagsParser*>(new HfstGermanTagsParser(noOptions));
    HfstTags expectedLexemeAttribsGemuseNom =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "neut")
             ("pos", "nn"));
    HfstTags expectedFormAttribsGemuseNom =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "nom")
             ("gender", "neut")
             ("number", "sg")
             ("pos", "nn"));
    HfstRawTags tagsGemuseNom = boost::assign::list_of
        ("<+NN>")
        ("<Neut>")
        ("<Nom>")
        ("<Sg>");
    testTagsParser(parser, tagsGemuseNom, expectedLexemeAttribsGemuseNom,
            expectedFormAttribsGemuseNom);
    HfstTags expectedLexemeAttribsGemuseDat =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "neut")
             ("pos", "nn"));
    HfstTags expectedFormAttribsGemuseDat =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "dat")
             ("gender", "neut")
             ("number", "sg")
             ("pos", "nn"));
    HfstRawTags tagsGemuseDat = boost::assign::list_of
        ("<+NN>")
        ("<Neut>")
        ("<Dat>")
        ("<Sg>");
    testTagsParser(parser, tagsGemuseDat, expectedLexemeAttribsGemuseDat,
            expectedFormAttribsGemuseDat);
    HfstTags expectedLexemeAttribsGemuseAkk =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "neut")
             ("pos", "nn"));
    HfstTags expectedFormAttribsGemuseAkk =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "akk")
             ("gender", "neut")
             ("number", "sg")
             ("pos", "nn"));
    HfstRawTags tagsGemuseAkk = boost::assign::list_of
        ("<+NN>")
        ("<Neut>")
        ("<Akk>")
        ("<Sg>");
    testTagsParser(parser, tagsGemuseAkk, expectedLexemeAttribsGemuseAkk,
            expectedFormAttribsGemuseAkk);
    delete parser;
}

BOOST_AUTO_TEST_CASE ( french_tags_parser ) {
    boost::program_options::variables_map noOptions;
    HfstTagsParser* parser = dynamic_cast<HfstFrenchTagsParser*>(new HfstFrenchTagsParser(noOptions));
    HfstTags expectedLexemeAttribsChat =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "masculine")
             ("pos", "commonNoun"));
    HfstTags expectedFormAttribsChat =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "masculine")
             ("number", "singular")
             ("pos", "commonNoun"));
    HfstRawTags tagsChat = boost::assign::list_of
        ("+commonNoun")
        ("+masculine")
        ("+singular");
    testTagsParser(parser, tagsChat, expectedLexemeAttribsChat,
            expectedFormAttribsChat);
    HfstTags expectedLexemeAttribsBlancN =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "masculine")
             ("pos", "commonNoun"));
    HfstTags expectedFormAttribsBlancN =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "masculine")
             ("number", "singular")
             ("pos", "commonNoun"));
    HfstRawTags tagsBlancN = boost::assign::list_of
        ("+commonNoun")
        ("+masculine")
        ("+singular");
    testTagsParser(parser, tagsBlancN, expectedLexemeAttribsBlancN,
            expectedFormAttribsBlancN);
    HfstTags expectedLexemeAttribsBlancAdj =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adjective"));
    HfstTags expectedFormAttribsBlancAdj =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "masculine")
             ("number", "singular")
             ("pos", "adjective"));
    HfstRawTags tagsBlancAdj = boost::assign::list_of
        ("+adjective")
        ("+masculine")
        ("+singular");
    testTagsParser(parser, tagsBlancAdj, expectedLexemeAttribsBlancAdj,
            expectedFormAttribsBlancAdj);
    delete parser;
}

BOOST_AUTO_TEST_CASE ( italian_tags_parser ) {
    boost::program_options::variables_map noOptions;
    HfstTagsParser* parser = dynamic_cast<HfstItalianTagsParser*>(new HfstItalianTagsParser(noOptions));
    HfstTags expectedLexemeAttribsGatta =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "f")
             ("pos", "noun"));
    HfstTags expectedFormAttribsGatta =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "f")
             ("number", "s")
             ("pos", "noun"));
    HfstRawTags tagsGatta = boost::assign::list_of
        ("NOUN")
        ("-F")
        ("s");
    testTagsParser(parser, tagsGatta, expectedLexemeAttribsGatta,
            expectedFormAttribsGatta);
    HfstTags expectedLexemeAttribsBiancaN =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "f")
             ("pos", "noun"));
    HfstTags expectedFormAttribsBiancaN =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "f")
             ("number", "s")
             ("pos", "noun"));
    HfstRawTags tagsBiancaN = boost::assign::list_of
        ("NOUN")
        ("-F")
        ("s");
    testTagsParser(parser, tagsBiancaN, expectedLexemeAttribsBiancaN,
            expectedFormAttribsBiancaN);
    HfstTags expectedLexemeAttribsBiancaAdj =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adj"));
    HfstTags expectedFormAttribsBiancaAdj =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("degree", "pos")
             ("gender", "f")
             ("number", "s")
             ("pos", "adj"));
    HfstRawTags tagsBiancaAdj = boost::assign::list_of
        ("ADJ")
        ("pos")
        ("f")
        ("s");
    testTagsParser(parser, tagsBiancaAdj, expectedLexemeAttribsBiancaAdj,
            expectedFormAttribsBiancaAdj);
    delete parser;
}

BOOST_AUTO_TEST_CASE ( swedish_tags_parser ) {
    boost::program_options::variables_map noOptions;
    HfstTagsParser* parser = dynamic_cast<HfstSwedishTagsParser*>(new HfstSwedishTagsParser(noOptions));
    HfstTags expectedLexemeAttribsKvinnorna =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gender", "utrum")
             ("pos", "substantiv"));
    HfstTags expectedFormAttribsKvinnorna =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "nom")
             ("definiteness", "best")
             ("gender", "utrum")
             ("number", "pl")
             ("pos", "substantiv"));
    HfstRawTags tagsKvinnorna = boost::assign::list_of
        ("<substantiv>")
        ("<utrum>")
        ("<pl>")
        ("<best>")
        ("<nom>");
    testTagsParser(parser, tagsKvinnorna, expectedLexemeAttribsKvinnorna,
            expectedFormAttribsKvinnorna);
    HfstTags expectedLexemeAttribsDopas =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsDopas =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb")
             ("tense", "infinitiv")
             ("voice", "passiv"));
    HfstRawTags tagsDopas = boost::assign::list_of
        ("<verb>")
        ("<infinitiv>")
        ("<passiv>");
    testTagsParser(parser, tagsDopas, expectedLexemeAttribsDopas,
            expectedFormAttribsDopas);
    HfstTags expectedLexemeAttribsVaraImp =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsVaraImp =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("mood", "imperativ")
             ("pos", "verb")
             ("voice", "aktiv"));
    HfstRawTags tagsVaraImp = boost::assign::list_of
        ("<verb>")
        ("<imperativ>")
        ("<aktiv>");
    testTagsParser(parser, tagsVaraImp, expectedLexemeAttribsVaraImp,
            expectedFormAttribsVaraImp);
    delete parser;
}

BOOST_AUTO_TEST_CASE ( turkish_tags_parser ) {
    boost::program_options::variables_map noOptions;
    HfstTagsParser* parser = dynamic_cast<HfstTurkishTagsParser*>(new HfstTurkishTagsParser(noOptions));
    HfstTags expectedLexemeAttribsBilgisayarlarin =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "n"));
    HfstTags expectedFormAttribsBilgisayarlarin =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "gen")
             ("number", "pl")
             ("pos", "n"));
    HfstRawTags tagsBilgisayarlarin = boost::assign::list_of
        ("<n>")
        ("<pl>")
        ("<gen>");
    testTagsParser(parser, tagsBilgisayarlarin, expectedLexemeAttribsBilgisayarlarin,
            expectedFormAttribsBilgisayarlarin);
    HfstTags expectedLexemeAttribsYemiyorsun =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "v"));
    HfstTags expectedFormAttribsYemiyorsun =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "v")
             ("neg", "neg")
             ("person", "2s")
             ("tense", "t_cont"));
    HfstRawTags tagsYemiyorsun = boost::assign::list_of
        ("<v>")
        ("<neg>")
        ("<t_cont>")
        ("<2s>");
    testTagsParser(parser, tagsYemiyorsun, expectedLexemeAttribsYemiyorsun,
            expectedFormAttribsYemiyorsun);
    delete parser;
}


void testTagsParser(HfstTagsParser* parser, HfstRawTags tags,
        HfstTags expectedLexemeAttributes,
        HfstTags expectedFormAttributes) {
    HfstTags resultLexemeAttributes = 
        parser->getLexemeAttributes(tags);
    BOOST_CHECK_EQUAL(resultLexemeAttributes.size(), expectedLexemeAttributes.size());
    HfstTags::iterator resultIt =
        resultLexemeAttributes.begin();
    HfstTags::iterator expectedIt =
        expectedLexemeAttributes.begin();
    while (resultIt != resultLexemeAttributes.end() && expectedIt != expectedLexemeAttributes.end()) {
        std::map<std::string, std::string>::iterator resultTagsIt = resultIt->begin();
        std::map<std::string, std::string>::iterator expectedTagsIt = expectedIt->begin();
        while (resultTagsIt != resultIt->end() && expectedTagsIt != expectedIt->end()) {
            BOOST_CHECK_EQUAL(resultTagsIt->first, expectedTagsIt->first);
            BOOST_CHECK_EQUAL(resultTagsIt->second, expectedTagsIt->second);
            ++ resultTagsIt;
            ++ expectedTagsIt;
        }
        ++ resultIt;
        ++ expectedIt;
    }
    HfstTags resultFormAttributes = 
        parser->getFormAttributes(tags);
    BOOST_CHECK_EQUAL(resultFormAttributes.size(), expectedFormAttributes.size());
    resultIt = resultFormAttributes.begin();
    expectedIt = expectedFormAttributes.begin();
    while (resultIt != resultFormAttributes.end() && expectedIt != expectedFormAttributes.end()) {
        std::map<std::string, std::string>::iterator resultTagsIt = resultIt->begin();
        std::map<std::string, std::string>::iterator expectedTagsIt = expectedIt->begin();
        while (resultTagsIt != resultIt->end() && expectedTagsIt != expectedIt->end()) {
            BOOST_CHECK_EQUAL(resultTagsIt->first, expectedTagsIt->first);
            BOOST_CHECK_EQUAL(resultTagsIt->second, expectedTagsIt->second);
            ++ resultTagsIt;
            ++ expectedTagsIt;
        }
        ++ resultIt;
        ++ expectedIt;
    }
}

BOOST_AUTO_TEST_SUITE_END()
