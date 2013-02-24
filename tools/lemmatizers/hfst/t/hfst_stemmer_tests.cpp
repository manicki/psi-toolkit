#include "tests.hpp"

#include <boost/program_options/parsers.hpp>
#include <fstream>
#include <map>
#include <boost/assign.hpp>
#include "hfst_adapter_interface.hpp"
#include "plugin_manager.hpp"
#include "hfst_english_stemmer.hpp"
#include "hfst_finnish_stemmer.hpp"
#include "hfst_french_stemmer.hpp"
#include "hfst_german_stemmer.hpp"
#include "hfst_italian_stemmer.hpp"
#include "hfst_swedish_stemmer.hpp"
#include "hfst_turkish_stemmer.hpp"

BOOST_AUTO_TEST_SUITE( hfst_stemmer )

void testStemmer(HfstStemmer* stemmer, HfstAdapterInterface* hfstAdapter, 
        const std::string &word, 
        HfstStems expected);

BOOST_AUTO_TEST_CASE ( finnish_stemmer_default ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/fi.hfst.ol");
    boost::program_options::variables_map noOptions;
    HfstStemmer* stemmer = dynamic_cast<HfstFinnishStemmer*>(new HfstFinnishStemmer(noOptions));
    HfstStems expectedLampimampi = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("lämmin", boost::assign::list_of
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
             ("[CASE=NOM]")
             ("@D.LONGxANALYSIS@")
             ("@D.LOWERCASED@"));
    testStemmer(stemmer, hfstAdapter, "lämpimämpi", expectedLampimampi);
    HfstStems expectedLahde = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("lähde", boost::assign::list_of
             ("@U.NEED.NOMINAL@")
             ("@C.AGREE@")
             ("@C.PLURALE@")
             ("[POS=NOUN]")
             ("[KTN=48]")
             ("[KAV=F]")
             ("@U.AGREE.SGfNOM@")
             ("@D.ALLOW.SGfPAR@")
             ("[NUM=SG]")
             ("[CASE=NOM]")
             ("@D.LONGxANALYSIS@")
             ("@D.LOWERCASED@"))
            ("lähteä", boost::assign::list_of
             ("@C.AGREE@")
             ("[POS=VERB]")
             ("[KAV=F]")
             ("@D.NEED@")
             ("@P.NEED.REST@")
             ("[VOICE=ACT]")
             ("[MOOD=IMPV]")
             ("[PRS=SG2]")
             ("@D.LONGxANALYSIS@")
             ("@D.LOWERCASED@")
             ("[KTN=60]"))
            ("lähteä", boost::assign::list_of
             ("@C.AGREE@")
             ("[POS=VERB]")
             ("[KAV=F]")
             ("@D.NEED@")
             ("@P.NEED.REST@")
             ("[MOOD=INDV]")
             ("[TENSE=PRES]")
             ("[NEG=CON]")
             ("@D.LONGxANALYSIS@")
             ("@D.LOWERCASED@")
             ("[KTN=60]"));
    testStemmer(stemmer, hfstAdapter, "lähde", expectedLahde);
    HfstStems expectedYliopisto = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("yliopisto", boost::assign::list_of
             ("@U.NEED.NOMINAL@")
             ("@C.AGREE@")
             ("@C.PLURALE@")
             ("[POS=NOUN]")
             ("[KTN=2]")
             ("@U.AGREE.SGfNOM@")
             ("@D.ALLOW.SGfPAR@")
             ("[NUM=SG]")
             ("[CASE=NOM]")
             ("@D.LONGxANALYSIS@")
             ("@D.LOWERCASED@"));
    testStemmer(stemmer, hfstAdapter, "yliopisto", expectedYliopisto);
    delete stemmer;
    delete hfstAdapter;
}

boost::program_options::options_description finnishStemmerOptionsHandled() {
    boost::program_options::options_description desc;
    desc.add_options()
        ("include-compounds", "include compounds");
    return desc;
}

BOOST_AUTO_TEST_CASE ( finnish_stemmer_include_compounds ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/fi.hfst.ol");
    std::ifstream optionsFile( ROOT_DIR "tools/lemmatizers/hfst/t/finnish_stemmer_include_compounds.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, finnishStemmerOptionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    HfstStemmer* stemmer = dynamic_cast<HfstFinnishStemmer*>(new HfstFinnishStemmer(testOptions));
    HfstStems expectedYliopisto = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("yliopisto", boost::assign::list_of
             ("@U.NEED.NOMINAL@")
             ("@C.AGREE@")
             ("@C.PLURALE@")
             ("[POS=NOUN]")
             ("[KTN=2]")
             ("@U.AGREE.SGfNOM@")
             ("@D.ALLOW.SGfPAR@")
             ("[NUM=SG]")
             ("[CASE=NOM]")
             ("@D.LONGxANALYSIS@")
             ("@D.LOWERCASED@"))
            ("yli", boost::assign::list_of
             ("@D.NEED@")
             ("[POS=NOUN]")
             ("[SUBCAT=PREFIX]"))
            ("opisto", boost::assign::list_of
             ("[GUESS=COMPOUND]")
             ("@U.NEED.NOMINAL@")
             ("@C.AGREE@")
             ("@C.PLURALE@")
             ("[POS=NOUN]")
             ("[KTN=2]")
             ("@U.AGREE.SGfNOM@")
             ("@D.ALLOW.SGfPAR@")
             ("[NUM=SG]")
             ("[CASE=NOM]")
             ("@D.LONGxANALYSIS@")
             ("@D.LOWERCASED@"));
    testStemmer(stemmer, hfstAdapter, "yliopisto", expectedYliopisto);
    delete stemmer;
    delete hfstAdapter;
}

BOOST_AUTO_TEST_CASE ( english_stemmer_default ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/en.hfst.ol");
    boost::program_options::variables_map noOptions;
    HfstStemmer* stemmer = dynamic_cast<HfstEnglishStemmer*>(new HfstEnglishStemmer(noOptions));
    HfstStems expectedGreen = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("green", boost::assign::list_of
             ("V")
             ("V")
             ("INF"))
            ("green", boost::assign::list_of
             ("N")
             ("N"))
            ("green", boost::assign::list_of
             ("ADJ")
             ("ADJ"));
    testStemmer(stemmer, hfstAdapter, "green", expectedGreen);
    HfstStems expectedGreener = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("green", boost::assign::list_of
             ("ADJ")
             ("ADJ")
             ("COMP"));
    testStemmer(stemmer, hfstAdapter, "greener", expectedGreener);
    delete stemmer;
    delete hfstAdapter;
}

boost::program_options::options_description englishStemmerOptionsHandled() {
    boost::program_options::options_description desc;
    desc.add_options()
        ("include-derivatives", "include derivatives");
    return desc;
}

BOOST_AUTO_TEST_CASE ( english_stemmer_include_derivatives ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/en.hfst.ol");
    std::ifstream optionsFile( ROOT_DIR "tools/lemmatizers/hfst/t/english_stemmer_include_derivatives.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, englishStemmerOptionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    HfstStemmer* stemmer = dynamic_cast<HfstEnglishStemmer*>(new HfstEnglishStemmer(testOptions));
    HfstStems expectedGreener = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("ER", boost::assign::list_of
             ("V/N")
             ("N"))
            ("green", boost::assign::list_of
             ("V"))
            ("green", boost::assign::list_of
             ("ADJ")
             ("ADJ")
             ("COMP"));
    testStemmer(stemmer, hfstAdapter, "greener", expectedGreener);
    delete stemmer;
    delete hfstAdapter;
}

BOOST_AUTO_TEST_CASE ( german_stemmer ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/de.hfst.ol");
    boost::program_options::variables_map noOptions;
    HfstStemmer* stemmer = dynamic_cast<HfstGermanStemmer*>(new HfstGermanStemmer(noOptions));
    HfstStems expectedGemuse = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("Gemüse", boost::assign::list_of
             ("<+NN>")
             ("<Neut>")
             ("<Nom>")
             ("<Sg>"))
            ("Gemüse", boost::assign::list_of
             ("<+NN>")
             ("<Neut>")
             ("<Dat>")
             ("<Sg>"))
            ("Gemüse", boost::assign::list_of
             ("<+NN>")
             ("<Neut>")
             ("<Akk>")
             ("<Sg>"));
    testStemmer(stemmer, hfstAdapter, "Gemüse", expectedGemuse);
    HfstStems expectedKontaktsmann = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("Kontakt", boost::assign::list_of
             ("<NN>"))
            ("Kontakt", boost::assign::list_of
             ("<NN>"))
            ("Kontakt", boost::assign::list_of
             ("<NN>"))
            ("Mann", boost::assign::list_of
             ("<+NN>")
             ("<Masc>")
             ("<Nom>")
             ("<Sg>"))
            ("Mann", boost::assign::list_of
             ("<+NN>")
             ("<Masc>")
             ("<Dat>")
             ("<Sg>"))
            ("Mann", boost::assign::list_of
             ("<+NN>")
             ("<Masc>")
             ("<Akk>")
             ("<Sg>"));
    testStemmer(stemmer, hfstAdapter, "Kontaktsmann", expectedKontaktsmann);
    delete stemmer;
    delete hfstAdapter;
}

BOOST_AUTO_TEST_CASE ( french_stemmer ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/fr.hfst.ol");
    boost::program_options::variables_map noOptions;
    HfstStemmer* stemmer = dynamic_cast<HfstFrenchStemmer*>(new HfstFrenchStemmer(noOptions));
    HfstStems expectedChat = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("chat", boost::assign::list_of
             ("+commonNoun")
             ("+masculine")
             ("+singular"));
    testStemmer(stemmer, hfstAdapter, "chat", expectedChat);
    HfstStems expectedBlanc = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("blanc", boost::assign::list_of
             ("+commonNoun")
             ("+masculine")
             ("+singular"))
            ("blanc", boost::assign::list_of
             ("+adjective")
             ("+singular")
             ("+masculine"));
    testStemmer(stemmer, hfstAdapter, "blanc", expectedBlanc);
    delete stemmer;
    delete hfstAdapter;
}

BOOST_AUTO_TEST_CASE ( italian_stemmer ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/it.hfst.ol");
    boost::program_options::variables_map noOptions;
    HfstStemmer* stemmer = dynamic_cast<HfstItalianStemmer*>(new HfstItalianStemmer(noOptions));
    HfstStems expectedGatta = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("gatta", boost::assign::list_of
             ("NOUN")
             ("-F")
             ("s"));
    testStemmer(stemmer, hfstAdapter, "gatta", expectedGatta);
    HfstStems expectedBianca = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("bianca", boost::assign::list_of
             ("NOUN")
             ("-F")
             ("s"))
            ("bianco", boost::assign::list_of
             ("ADJ")
             ("pos")
             ("f")
             ("s"));
    testStemmer(stemmer, hfstAdapter, "bianca", expectedBianca);
    delete stemmer;
    delete hfstAdapter;
}

BOOST_AUTO_TEST_CASE ( swedish_stemmer ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/se.hfst.ol");
    boost::program_options::variables_map noOptions;
    HfstStemmer* stemmer = dynamic_cast<HfstSwedishStemmer*>(new HfstSwedishStemmer(noOptions));
    HfstStems expectedKvinnorna = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("kvinna", boost::assign::list_of
             ("<substantiv>")
             ("<utrum>")
             ("<pl>")
             ("<best>")
             ("<nom>"));
    testStemmer(stemmer, hfstAdapter, "kvinnorna", expectedKvinnorna);
    HfstStems expectedDopas = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("döpa", boost::assign::list_of
             ("<verb>")
             ("<infinitiv>")
             ("<passiv>"));
    testStemmer(stemmer, hfstAdapter, "döpas", expectedDopas);
    HfstStems expectedVara = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("vara", boost::assign::list_of
             ("<verb>")
             ("<infinitiv>")
             ("<aktiv>"))
            ("vara", boost::assign::list_of
             ("<verb>")
             ("<imperativ>")
             ("<aktiv>"))
            ("vara", boost::assign::list_of
             ("<substantiv>")
             ("<utrum>")
             ("<sg>")
             ("<obest>")
             ("<nom>"))
            ("vara", boost::assign::list_of
             ("<substantiv>")
             ("<neutrum>")
             ("<sg>")
             ("<obest>")
             ("<nom>"));
    testStemmer(stemmer, hfstAdapter, "vara", expectedVara);
    delete stemmer;
    delete hfstAdapter;
}

BOOST_AUTO_TEST_CASE ( turkish_stemmer ) {
    HfstAdapterInterface* hfstAdapter = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstAdapter->initHfst(ROOT_DIR "tools/lemmatizers/hfst/data/tr.hfst.ol");
    boost::program_options::variables_map noOptions;
    HfstStemmer* stemmer = dynamic_cast<HfstTurkishStemmer*>(new HfstTurkishStemmer(noOptions));
    HfstStems expectedBilgisayarlarin = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("bilgisayar", boost::assign::list_of
             ("<n>")
             ("<pl>")
             ("<p2s>")
             ("<3s>"))
            ("bilgisayar", boost::assign::list_of
             ("<n>")
             ("<pl>")
             ("<p2s>"))
            ("bilgisayar", boost::assign::list_of
             ("<n>")
             ("<pl>")
             ("<gen>")
             ("<3s>"))
            ("bilgisayar", boost::assign::list_of
             ("<n>")
             ("<pl>")
             ("<gen>"));
    testStemmer(stemmer, hfstAdapter, "bilgisayarların", expectedBilgisayarlarin);
    HfstStems expectedYemiyorsun = 
        boost::assign::map_list_of<std::string, std::vector<std::string> >
            ("ye", boost::assign::list_of
             ("<v>")
             ("<neg>")
             ("<t_cont>")
             ("<2s>"));
    testStemmer(stemmer, hfstAdapter, "yemiyorsun", expectedYemiyorsun);
    delete stemmer;
    delete hfstAdapter;
}


void testStemmer(HfstStemmer* stemmer, HfstAdapterInterface* hfstAdapter, 
        const std::string &word, 
        HfstStems expected) {
    HfstStems result =
        stemmer->stem(word, *hfstAdapter);
    BOOST_CHECK_EQUAL(result.size(), expected.size());
    HfstStems::iterator resultIt = result.begin();
    HfstStems::iterator expectedIt = expected.begin();
    while (resultIt != result.end() && expectedIt != expected.end()) {
        BOOST_CHECK_EQUAL(resultIt->first, expectedIt->first);
        BOOST_CHECK_EQUAL_COLLECTIONS(resultIt->second.begin(), resultIt->second.end(), 
                expectedIt->second.begin(), expectedIt->second.end());
        ++ resultIt;
        ++ expectedIt;
    }
}

BOOST_AUTO_TEST_SUITE_END()
