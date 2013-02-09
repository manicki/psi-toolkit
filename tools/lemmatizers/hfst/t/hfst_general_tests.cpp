#include "tests.hpp"

#include "lattice.hpp"
#include "annotation_item_manager.hpp"
#include "sample_cutter.hpp"
#include "lemmatizer_annotator.hpp"
#include <boost/program_options/parsers.hpp>
#include <fstream>
#include "hfst_lemmatizer.hpp"

BOOST_AUTO_TEST_SUITE( hfst_general )

void initTestLattice_(Lattice &lattice, const std::string &text, 
        const std::string &lang) {
    lattice.appendStringWithSymbols(text);

    LayerTagCollection textTags(
            lattice.getLayerTagManager().
            createSingletonTagCollectionWithLangCode("text", lang));
    AnnotationItem item("TEXT", text);
    try {
        lattice.addEdge(lattice.getFirstVertex(), lattice.getLastVertex(), item, textTags);
    } catch (LoopEdgeException) { }

    SampleCutter cutter(lang);

    LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");
    LayerTagMask textMask = lattice.getLayerTagManager().getMaskWithLangCode(
            "text", lang);

    lattice.runCutter(cutter, symbolMask, textMask);
}

BOOST_AUTO_TEST_CASE( lemmatize ) {
    AnnotationItemManager aim;
    Lattice lattice(aim);
    initTestLattice_(lattice, "lämpimämpi", "fi");

    std::ifstream optionsFile( ROOT_DIR "tools/lemmatizers/hfst/t/lemmatize.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, HfstLemmatizer::optionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    LemmatizerAnnotator<HfstLemmatizer> annotator(testOptions);

    annotator.annotate(lattice);

    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lemma");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

        BOOST_CHECK(lemmaIter.hasNext());
        Lattice::EdgeDescriptor edge = lemmaIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "word");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lämmin");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            12U);

        BOOST_CHECK(!lemmaIter.hasNext());
    }

    {
        LayerTagMask lexemeMask_ = lattice.getLayerTagManager().getMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lexemeIter = lattice.edgesSortedByTarget(lexemeMask_);

        BOOST_CHECK(lexemeIter.hasNext());
        Lattice::EdgeDescriptor edge = lexemeIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "adjective");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lämmin+adjective");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            12U);

        BOOST_CHECK(!lexemeIter.hasNext());
    }

    {
        LayerTagMask formMask_ = lattice.getLayerTagManager().getMask("form");
        Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);

        BOOST_CHECK(formIter.hasNext());
        Lattice::EdgeDescriptor edge = formIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "adjective");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lämmin");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            12U);

        std::list< std::pair<std::string, std::string> > av
            = aim.getValues(lattice.getEdgeAnnotationItem(edge));
        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "case");
        BOOST_CHECK_EQUAL((*avi).second, "nom");
        ++avi;
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "cmp");
        BOOST_CHECK_EQUAL((*avi).second, "cmp");
        ++avi;
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "num");
        BOOST_CHECK_EQUAL((*avi).second, "sg");
        ++avi;
        BOOST_CHECK(avi == av.end());

        BOOST_CHECK(!formIter.hasNext());
    }

}

BOOST_AUTO_TEST_CASE( multiple_interpretations ) {
    AnnotationItemManager aim;
    Lattice lattice(aim);
    initTestLattice_(lattice, "lähde", "fi");

    std::ifstream optionsFile( ROOT_DIR "tools/lemmatizers/hfst/t/lemmatize.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, HfstLemmatizer::optionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    LemmatizerAnnotator<HfstLemmatizer> annotator(testOptions);

    annotator.annotate(lattice);

    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lemma");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

        BOOST_CHECK(lemmaIter.hasNext());
        Lattice::EdgeDescriptor edge = lemmaIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "word");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lähde");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            6U);

        BOOST_CHECK(lemmaIter.hasNext());
        edge = lemmaIter.next();
        annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "word");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lähteä");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            6U);

        BOOST_CHECK(!lemmaIter.hasNext());
    }

    {
        LayerTagMask lexemeMask_ = lattice.getLayerTagManager().getMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lexemeIter = lattice.edgesSortedByTarget(lexemeMask_);

        BOOST_CHECK(lexemeIter.hasNext());
        Lattice::EdgeDescriptor edge = lexemeIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "noun");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lähde+noun");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            6U);

        BOOST_CHECK(lexemeIter.hasNext());
        edge = lexemeIter.next();
        annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "verb");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lähteä+verb");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            6U);

        BOOST_CHECK(!lexemeIter.hasNext());
    }

    {
        LayerTagMask formMask_ = lattice.getLayerTagManager().getMask("form");
        Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);

        BOOST_CHECK(formIter.hasNext());
        Lattice::EdgeDescriptor edge = formIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "noun");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lähde");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            6U);

        std::list< std::pair<std::string, std::string> > av
            = aim.getValues(lattice.getEdgeAnnotationItem(edge));
        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "case");
        BOOST_CHECK_EQUAL((*avi).second, "nom");
        ++avi;
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "num");
        BOOST_CHECK_EQUAL((*avi).second, "sg");
        ++avi;
        BOOST_CHECK(avi == av.end());

        BOOST_CHECK(formIter.hasNext());
        edge = formIter.next();
        annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "verb");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lähteä");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            6U);

        av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
        avi = av.begin();
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "mood");
        BOOST_CHECK_EQUAL((*avi).second, "indv");
        ++avi;
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "person");
        BOOST_CHECK_EQUAL((*avi).second, "neg");
        ++avi;
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "tense");
        BOOST_CHECK_EQUAL((*avi).second, "pres");
        ++avi;
        BOOST_CHECK(avi == av.end());

        BOOST_CHECK(formIter.hasNext());
        edge = formIter.next();
        annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "verb");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lähteä");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            6U);

        av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
        avi = av.begin();
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "num");
        BOOST_CHECK_EQUAL((*avi).second, "sg");
        ++avi;
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "mood");
        BOOST_CHECK_EQUAL((*avi).second, "impv");
        ++avi;
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "person");
        BOOST_CHECK_EQUAL((*avi).second, "sec");
        ++avi;
        BOOST_CHECK(avi != av.end());
        BOOST_CHECK_EQUAL((*avi).first, "gen");
        BOOST_CHECK_EQUAL((*avi).second, "act");
        ++avi;
        BOOST_CHECK(avi == av.end());

        BOOST_CHECK(!formIter.hasNext());
    }

}

BOOST_AUTO_TEST_CASE( stems_on_lexeme_level ) {
    AnnotationItemManager aim;
    Lattice lattice(aim);
    initTestLattice_(lattice, "lämpimämpi", "fi");

    std::ifstream optionsFile( ROOT_DIR "tools/lemmatizers/hfst/t/stems_lexeme_level.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, HfstLemmatizer::optionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    LemmatizerAnnotator<HfstLemmatizer> annotator(testOptions);

    annotator.annotate(lattice);

    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lemma");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

        BOOST_CHECK(lemmaIter.hasNext());
        Lattice::EdgeDescriptor edge = lemmaIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "word");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lämmin");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            12U);

        BOOST_CHECK(!lemmaIter.hasNext());
    }

    {
        LayerTagMask lexemeMask_ = lattice.getLayerTagManager().getMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lexemeIter = lattice.edgesSortedByTarget(lexemeMask_);

        BOOST_CHECK(lexemeIter.hasNext());
        Lattice::EdgeDescriptor edge = lexemeIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "adjective");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lämmin+adjective");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            12U);

        BOOST_CHECK(!lexemeIter.hasNext());
    }

    {
        LayerTagMask formMask_ = lattice.getLayerTagManager().getMask("form");
        Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);
        BOOST_CHECK(!formIter.hasNext());
    }

}

BOOST_AUTO_TEST_CASE( stems_on_lemma_level ) {
    AnnotationItemManager aim;
    Lattice lattice(aim);
    initTestLattice_(lattice, "lämpimämpi", "fi");

    std::ifstream optionsFile( ROOT_DIR "tools/lemmatizers/hfst/t/stems_lemma_level.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, HfstLemmatizer::optionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    LemmatizerAnnotator<HfstLemmatizer> annotator(testOptions);

    annotator.annotate(lattice);

    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lemma");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

        BOOST_CHECK(lemmaIter.hasNext());
        Lattice::EdgeDescriptor edge = lemmaIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);

        BOOST_CHECK_EQUAL(annotationItem.getCategory(), "word");
        BOOST_CHECK_EQUAL(annotationItem.getText(), "lämmin");

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeSource(edge)),
            0U);

        BOOST_CHECK_EQUAL(
            lattice.getVertexRawCharIndex(lattice.getEdgeTarget(edge)),
            12U);

        BOOST_CHECK(!lemmaIter.hasNext());
    }

    {
        LayerTagMask lexemeMask_ = lattice.getLayerTagManager().getMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lexemeIter = lattice.edgesSortedByTarget(lexemeMask_);
        BOOST_CHECK(!lexemeIter.hasNext());
    }

    {
        LayerTagMask formMask_ = lattice.getLayerTagManager().getMask("form");
        Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);
        BOOST_CHECK(!formIter.hasNext());
    }

}

BOOST_AUTO_TEST_CASE( stems_zero_level ) {
    AnnotationItemManager aim;
    Lattice lattice(aim);
    initTestLattice_(lattice, "lämpimämpi", "fi");

    std::ifstream optionsFile( ROOT_DIR "tools/lemmatizers/hfst/t/stems_zero_level.ini");
    boost::program_options::variables_map testOptions;
    boost::program_options::store(boost::program_options::parse_config_file(
                optionsFile, HfstLemmatizer::optionsHandled()
                ), testOptions);
    boost::program_options::notify(testOptions);
    optionsFile.close();
    LemmatizerAnnotator<HfstLemmatizer> annotator(testOptions);

    annotator.annotate(lattice);

    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lemma");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);
        BOOST_CHECK(!lemmaIter.hasNext());
    }

    {
        LayerTagMask lexemeMask_ = lattice.getLayerTagManager().getMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lexemeIter = lattice.edgesSortedByTarget(lexemeMask_);
        BOOST_CHECK(!lexemeIter.hasNext());
    }

    {
        LayerTagMask formMask_ = lattice.getLayerTagManager().getMask("form");
        Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);
        BOOST_CHECK(!formIter.hasNext());
    }

}

BOOST_AUTO_TEST_SUITE_END()
