#include "tests.hpp"

#include <boost/program_options/parsers.hpp>
#include <fstream>
#include <map>
#include <vector>
#include <boost/assign.hpp>
#include "config.hpp"
#include "hfst_finnish_tags_parser.hpp"

BOOST_AUTO_TEST_SUITE( hfst_finnish_tags_parser )

void testTagsParser(HfstTagsParser* parser, HfstRawTags tags,
        HfstTags expectedLexemeAttributes,
        HfstTags expectedFormAttributes);

BOOST_AUTO_TEST_CASE ( finnish_tags_parser_default ) {
    boost::program_options::variables_map noOptions;
    HfstTagsParser* parser = dynamic_cast<HfstFinnishTagsParser*>(new HfstFinnishTagsParser(noOptions));
    HfstTags expectedLexemeAttribsLampimampi =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adjective"));
    HfstTags expectedFormAttribsLampimampi =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "nom")
             ("cmp", "cmp")
             ("num", "sg")
             ("pos", "adjective"));
    HfstRawTags tagsLampimampi = boost::assign::list_of
        ("@U.NEED.NOMINAL@")
        ("@C.AGREE@")
        ("@C.PLURALE@")
        ("[POS=ADJECTIVE]")
        ("[KTN=35]")
        ("[KAV=H]")
        ("[CMP=CMP]")
        ("@U.AGREE.SGfNOM@")
        ("@D.ALLOW.SGfPAR@")
        ("[NUM=SG]")
        ("[CASE=NOM]");
    testTagsParser(parser, tagsLampimampi, expectedLexemeAttribsLampimampi,
            expectedFormAttribsLampimampi);
    HfstTags expectedLexemeAttribsTietaa3sg =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsTietaa3sg =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("mood", "indv")
             ("num", "sg")
             ("person", "ter")
             ("pos", "verb")
             ("tense", "pres"));
    HfstRawTags tagsTietaa3sg = boost::assign::list_of
        ("@D.NEED@")
        ("@P.NEED.REST@")
        ("[POS=VERB]")
        ("[KTN=76]")
        ("[KAV=F]")
        ("[GEN=ACT]")
        ("[MOOD=INDV]")
        ("[TENSE=PRES]")
        ("[PRS=SG3]");
    testTagsParser(parser, tagsTietaa3sg, expectedLexemeAttribsTietaa3sg,
            expectedFormAttribsTietaa3sg);
    HfstTags expectedLexemeAttribsTietaaInf =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsTietaaInf =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("inf", "a")
             ("num", "sg")
             ("pos", "verb"));
    HfstRawTags tagsTietaaInf = boost::assign::list_of
        ("@D.NEED@")
        ("@P.NEED.REST@")
        ("[POS=VERB]")
        ("[KTN=76]")
        ("[KAV=F]")
        ("[GEN=ACT]")
        ("[INF=A]")
        ("[NUM=SG]")
        ("[CASE=LAT]");
    testTagsParser(parser, tagsTietaaInf, expectedLexemeAttribsTietaaInf,
            expectedFormAttribsTietaaInf);
    HfstTags expectedLexemeAttribsOletkos =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsOletkos =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("mood", "indv")
             ("num", "sg")
             ("person", "sec")
             ("pos", "verb")
             ("tense", "pres"));
    HfstRawTags tagsOletkos = boost::assign::list_of
        ("@D.NEED@")
        ("[POS=VERB]")
        ("[KTN=67]")
        ("[GEN=ACT]")
        ("[MOOD=INDV]")
        ("[TENSE=PRES]")
        ("[PRS=SG2]")
        ("[CLIT=KO]")
        ("[CLIT=S]");
    testTagsParser(parser, tagsOletkos, expectedLexemeAttribsOletkos,
            expectedFormAttribsOletkos);
    delete parser;
}

boost::program_options::options_description finnishTagsParserOptionsHandled() {
    boost::program_options::options_description desc;
    desc.add_options()
        ("keep-original-attributes", "keep original HFST attribute tags")
        //@todo: split-pos-tags temporarily disabled as did not work as intended
        //("do-not-split-pos-tags", "do not split POS tags")
        ("include-enclitic-particles", "include enclitic particles (e.g. -kin, -han, -hän) as attributes of form edges")
        ("include-inflection-types", "include inflection type numbers as attributes of form edges");
    return desc;
}

BOOST_AUTO_TEST_CASE ( finnish_tags_parser_keep_original_attributes ) {
    std::ifstream optionsFile(ROOT_DIR "tools/lemmatizers/hfst/t/finnish_tags_parser_keep_original_attributes.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, finnishTagsParserOptionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    HfstTagsParser* parser = dynamic_cast<HfstFinnishTagsParser*>(new HfstFinnishTagsParser(testOptions));
    HfstTags expectedLexemeAttribsLampimampi =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adjective"));
    HfstTags expectedFormAttribsLampimampi =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "nom")
             ("cmp", "cmp")
             ("num", "sg")
             ("pos", "adjective"));
    HfstRawTags tagsLampimampi = boost::assign::list_of
        ("@U.NEED.NOMINAL@")
        ("@C.AGREE@")
        ("@C.PLURALE@")
        ("[POS=ADJECTIVE]")
        ("[KTN=35]")
        ("[KAV=H]")
        ("[CMP=CMP]")
        ("@U.AGREE.SGfNOM@")
        ("@D.ALLOW.SGfPAR@")
        ("[NUM=SG]")
        ("[CASE=NOM]");
    testTagsParser(parser, tagsLampimampi, expectedLexemeAttribsLampimampi,
            expectedFormAttribsLampimampi);
    HfstTags expectedLexemeAttribsTietaa3sg =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsTietaa3sg =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("mood", "indv")
             ("pos", "verb")
             ("prs", "sg3")
             ("tense", "pres"));
    HfstRawTags tagsTietaa3sg = boost::assign::list_of
        ("@D.NEED@")
        ("@P.NEED.REST@")
        ("[POS=VERB]")
        ("[KTN=76]")
        ("[KAV=F]")
        ("[GEN=ACT]")
        ("[MOOD=INDV]")
        ("[TENSE=PRES]")
        ("[PRS=SG3]");
    testTagsParser(parser, tagsTietaa3sg, expectedLexemeAttribsTietaa3sg,
            expectedFormAttribsTietaa3sg);
    HfstTags expectedLexemeAttribsTietaaInf =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsTietaaInf =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("inf", "a")
             ("num", "sg")
             ("pos", "verb"));
    HfstRawTags tagsTietaaInf = boost::assign::list_of
        ("@D.NEED@")
        ("@P.NEED.REST@")
        ("[POS=VERB]")
        ("[KTN=76]")
        ("[KAV=F]")
        ("[GEN=ACT]")
        ("[INF=A]")
        ("[NUM=SG]")
        ("[CASE=LAT]");
    testTagsParser(parser, tagsTietaaInf, expectedLexemeAttribsTietaaInf,
            expectedFormAttribsTietaaInf);
    HfstTags expectedLexemeAttribsOletkos =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsOletkos =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("mood", "indv")
             ("pos", "verb")
             ("prs", "sg2")
             ("tense", "pres"));
    HfstRawTags tagsOletkos = boost::assign::list_of
        ("@D.NEED@")
        ("[POS=VERB]")
        ("[KTN=67]")
        ("[GEN=ACT]")
        ("[MOOD=INDV]")
        ("[TENSE=PRES]")
        ("[PRS=SG2]")
        ("[CLIT=KO]")
        ("[CLIT=S]");
    testTagsParser(parser, tagsOletkos, expectedLexemeAttribsOletkos,
            expectedFormAttribsOletkos);
    delete parser;
}

//@todo: split-pos-tags temporarily disabled as did not work as intended
//BOOST_AUTO_TEST_CASE ( finnish_tags_parser_do_not_split_pos_tags ) {
//    std::ifstream optionsFile(ROOT_DIR "tools/lemmatizers/hfst/t/finnish_tags_parser_do_not_split_pos_tags.ini");
//    boost::program_options::variables_map testOptions;
//    boost::program_options::store(boost::program_options::parse_config_file(
//                optionsFile, finnishTagsParserOptionsHandled()
//                ), testOptions);
//    boost::program_options::notify(testOptions);
//    optionsFile.close();
//    HfstTagsParser* parser = dynamic_cast<HfstFinnishTagsParser*>(new HfstFinnishTagsParser(testOptions));
//    HfstTags expectedLexemeAttribsLampimampi =
//        boost::assign::list_of<std::map<std::string, std::string> >
//            (boost::assign::map_list_of
//             ("pos", "adjective"));
//    HfstTags expectedFormAttribsLampimampi =
//        boost::assign::list_of<std::map<std::string, std::string> >
//            (boost::assign::map_list_of
//             ("case", "nom")
//             ("cmp", "cmp")
//             ("num", "sg")
//             ("pos", "adjective"));
//    HfstRawTags tagsLampimampi = boost::assign::list_of
//        ("@U.NEED.NOMINAL@")
//        ("@C.AGREE@")
//        ("@C.PLURALE@")
//        ("[POS=ADJECTIVE]")
//        ("[KTN=35]")
//        ("[KAV=H]")
//        ("[CMP=CMP]")
//        ("@U.AGREE.SGfNOM@")
//        ("@D.ALLOW.SGfPAR@")
//        ("[NUM=SG]")
//        ("[CASE=NOM]");
//    testTagsParser(parser, tagsLampimampi, expectedLexemeAttribsLampimampi,
//            expectedFormAttribsLampimampi);
//    HfstTags expectedLexemeAttribsTietaa3sg =
//        boost::assign::list_of<std::map<std::string, std::string> >
//            (boost::assign::map_list_of
//             ("pos", "verb"));
//    HfstTags expectedFormAttribsTietaa3sg =
//        boost::assign::list_of<std::map<std::string, std::string> >
//            (boost::assign::map_list_of
//             ("gen", "act")
//             ("mood", "indv")
//             ("num", "sg")
//             ("person", "ter")
//             ("pos", "verb")
//             ("tense", "pres"));
//    HfstRawTags tagsTietaa3sg = boost::assign::list_of
//        ("@D.NEED@")
//        ("@P.NEED.REST@")
//        ("[POS=VERB]")
//        ("[KTN=76]")
//        ("[KAV=F]")
//        ("[GEN=ACT]")
//        ("[MOOD=INDV]")
//        ("[TENSE=PRES]")
//        ("[PRS=SG3]");
//    testTagsParser(parser, tagsTietaa3sg, expectedLexemeAttribsTietaa3sg,
//            expectedFormAttribsTietaa3sg);
//    HfstTags expectedLexemeAttribsTietaaInf =
//        boost::assign::list_of<std::map<std::string, std::string> >
//            (boost::assign::map_list_of
//             ("pos", "verb"));
//    HfstTags expectedFormAttribsTietaaInf =
//        boost::assign::list_of<std::map<std::string, std::string> >
//            (boost::assign::map_list_of
//             ("gen", "act")
//             ("inf", "a")
//             ("num", "sg")
//             ("pos", "verb"));
//    HfstRawTags tagsTietaaInf = boost::assign::list_of
//        ("@D.NEED@")
//        ("@P.NEED.REST@")
//        ("[POS=VERB]")
//        ("[KTN=76]")
//        ("[KAV=F]")
//        ("[GEN=ACT]")
//        ("[INF=A]")
//        ("[NUM=SG]")
//        ("[CASE=LAT]");
//    testTagsParser(parser, tagsTietaaInf, expectedLexemeAttribsTietaaInf,
//            expectedFormAttribsTietaaInf);
//    HfstTags expectedLexemeAttribsOletkos =
//        boost::assign::list_of<std::map<std::string, std::string> >
//            (boost::assign::map_list_of
//             ("pos", "verb"));
//    HfstTags expectedFormAttribsOletkos =
//        boost::assign::list_of<std::map<std::string, std::string> >
//            (boost::assign::map_list_of
//             ("gen", "act")
//             ("mood", "indv")
//             ("num", "sg")
//             ("person", "sec")
//             ("pos", "verb")
//             ("tense", "pres"));
//    HfstRawTags tagsOletkos = boost::assign::list_of
//        ("@D.NEED@")
//        ("[POS=VERB]")
//        ("[KTN=67]")
//        ("[GEN=ACT]")
//        ("[MOOD=INDV]")
//        ("[TENSE=PRES]")
//        ("[PRS=SG2]")
//        ("[CLIT=KO]")
//        ("[CLIT=S]");
//    testTagsParser(parser, tagsOletkos, expectedLexemeAttribsOletkos,
//            expectedFormAttribsOletkos);
//    delete parser;
//}

BOOST_AUTO_TEST_CASE ( finnish_tags_parser_include_enclitic_particles ) {
    std::ifstream optionsFile(ROOT_DIR "tools/lemmatizers/hfst/t/finnish_tags_parser_include_enclitic_particles.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, finnishTagsParserOptionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    HfstTagsParser* parser = dynamic_cast<HfstFinnishTagsParser*>(new HfstFinnishTagsParser(testOptions));
    HfstTags expectedLexemeAttribsLampimampi =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adjective"));
    HfstTags expectedFormAttribsLampimampi =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "nom")
             ("cmp", "cmp")
             ("num", "sg")
             ("pos", "adjective"));
    HfstRawTags tagsLampimampi = boost::assign::list_of
        ("@U.NEED.NOMINAL@")
        ("@C.AGREE@")
        ("@C.PLURALE@")
        ("[POS=ADJECTIVE]")
        ("[KTN=35]")
        ("[KAV=H]")
        ("[CMP=CMP]")
        ("@U.AGREE.SGfNOM@")
        ("@D.ALLOW.SGfPAR@")
        ("[NUM=SG]")
        ("[CASE=NOM]");
    testTagsParser(parser, tagsLampimampi, expectedLexemeAttribsLampimampi,
            expectedFormAttribsLampimampi);
    HfstTags expectedLexemeAttribsTietaa3sg =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsTietaa3sg =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("mood", "indv")
             ("num", "sg")
             ("person", "ter")
             ("pos", "verb")
             ("tense", "pres"));
    HfstRawTags tagsTietaa3sg = boost::assign::list_of
        ("@D.NEED@")
        ("@P.NEED.REST@")
        ("[POS=VERB]")
        ("[KTN=76]")
        ("[KAV=F]")
        ("[GEN=ACT]")
        ("[MOOD=INDV]")
        ("[TENSE=PRES]")
        ("[PRS=SG3]");
    testTagsParser(parser, tagsTietaa3sg, expectedLexemeAttribsTietaa3sg,
            expectedFormAttribsTietaa3sg);
    HfstTags expectedLexemeAttribsTietaaInf =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsTietaaInf =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("inf", "a")
             ("num", "sg")
             ("pos", "verb"));
    HfstRawTags tagsTietaaInf = boost::assign::list_of
        ("@D.NEED@")
        ("@P.NEED.REST@")
        ("[POS=VERB]")
        ("[KTN=76]")
        ("[KAV=F]")
        ("[GEN=ACT]")
        ("[INF=A]")
        ("[NUM=SG]")
        ("[CASE=LAT]");
    testTagsParser(parser, tagsTietaaInf, expectedLexemeAttribsTietaaInf,
            expectedFormAttribsTietaaInf);
    HfstTags expectedLexemeAttribsOletkos =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsOletkos =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("clit", "ko+s")
             ("gen", "act")
             ("mood", "indv")
             ("num", "sg")
             ("person", "sec")
             ("pos", "verb")
             ("tense", "pres"));
    HfstRawTags tagsOletkos = boost::assign::list_of
        ("@D.NEED@")
        ("[POS=VERB]")
        ("[KTN=67]")
        ("[GEN=ACT]")
        ("[MOOD=INDV]")
        ("[TENSE=PRES]")
        ("[PRS=SG2]")
        ("[CLIT=KO]")
        ("[CLIT=S]");
    testTagsParser(parser, tagsOletkos, expectedLexemeAttribsOletkos,
            expectedFormAttribsOletkos);
    delete parser;
}

BOOST_AUTO_TEST_CASE ( finnish_tags_parser_include_inflection_types ) {
    std::ifstream optionsFile(ROOT_DIR "tools/lemmatizers/hfst/t/finnish_tags_parser_include_inflection_types.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, finnishTagsParserOptionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    HfstTagsParser* parser = dynamic_cast<HfstFinnishTagsParser*>(new HfstFinnishTagsParser(testOptions));
    HfstTags expectedLexemeAttribsLampimampi =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "adjective"));
    HfstTags expectedFormAttribsLampimampi =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("case", "nom")
             ("cmp", "cmp")
             ("kav", "h")
             ("ktn", "35")
             ("num", "sg")
             ("pos", "adjective"));
    HfstRawTags tagsLampimampi = boost::assign::list_of
        ("@U.NEED.NOMINAL@")
        ("@C.AGREE@")
        ("@C.PLURALE@")
        ("[POS=ADJECTIVE]")
        ("[KTN=35]")
        ("[KAV=H]")
        ("[CMP=CMP]")
        ("@U.AGREE.SGfNOM@")
        ("@D.ALLOW.SGfPAR@")
        ("[NUM=SG]")
        ("[CASE=NOM]");
    testTagsParser(parser, tagsLampimampi, expectedLexemeAttribsLampimampi,
            expectedFormAttribsLampimampi);
    HfstTags expectedLexemeAttribsTietaa3sg =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsTietaa3sg =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("kav", "f")
             ("ktn", "76")
             ("mood", "indv")
             ("num", "sg")
             ("person", "ter")
             ("pos", "verb")
             ("tense", "pres"));
    HfstRawTags tagsTietaa3sg = boost::assign::list_of
        ("@D.NEED@")
        ("@P.NEED.REST@")
        ("[POS=VERB]")
        ("[KTN=76]")
        ("[KAV=F]")
        ("[GEN=ACT]")
        ("[MOOD=INDV]")
        ("[TENSE=PRES]")
        ("[PRS=SG3]");
    testTagsParser(parser, tagsTietaa3sg, expectedLexemeAttribsTietaa3sg,
            expectedFormAttribsTietaa3sg);
    HfstTags expectedLexemeAttribsTietaaInf =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsTietaaInf =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("kav", "f")
             ("ktn", "76")
             ("inf", "a")
             ("num", "sg")
             ("pos", "verb"));
    HfstRawTags tagsTietaaInf = boost::assign::list_of
        ("@D.NEED@")
        ("@P.NEED.REST@")
        ("[POS=VERB]")
        ("[KTN=76]")
        ("[KAV=F]")
        ("[GEN=ACT]")
        ("[INF=A]")
        ("[NUM=SG]")
        ("[CASE=LAT]");
    testTagsParser(parser, tagsTietaaInf, expectedLexemeAttribsTietaaInf,
            expectedFormAttribsTietaaInf);
    HfstTags expectedLexemeAttribsOletkos =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("pos", "verb"));
    HfstTags expectedFormAttribsOletkos =
        boost::assign::list_of<std::map<std::string, std::string> >
            (boost::assign::map_list_of
             ("gen", "act")
             ("ktn", "67")
             ("mood", "indv")
             ("num", "sg")
             ("person", "sec")
             ("pos", "verb")
             ("tense", "pres"));
    HfstRawTags tagsOletkos = boost::assign::list_of
        ("@D.NEED@")
        ("[POS=VERB]")
        ("[KTN=67]")
        ("[GEN=ACT]")
        ("[MOOD=INDV]")
        ("[TENSE=PRES]")
        ("[PRS=SG2]")
        ("[CLIT=KO]")
        ("[CLIT=S]");
    testTagsParser(parser, tagsOletkos, expectedLexemeAttribsOletkos,
            expectedFormAttribsOletkos);
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
