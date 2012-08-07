#include "tests.hpp"

#include "me_tagger.hpp"

BOOST_AUTO_TEST_SUITE( me_tagger_tests )

BOOST_AUTO_TEST_CASE( training_test ) {
    AnnotationItemManager aim;
    Lattice lattice(aim, "Ala ma kota. Koty są czarne.");
    lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());

    std::string langCode = "pl";

    LayerTagCollection symbolTag
        = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
    LayerTagCollection tokenTag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "token", langCode);
    LayerTagCollection lemmaTag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "lemma", langCode);
    LayerTagCollection lexemeTag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "lexeme", langCode);
    LayerTagCollection formTag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "form", langCode);
    LayerTagCollection segmentTag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "segment", langCode);
    LayerTagMask symbolMask = lattice.getLayerTagManager().getMask(symbolTag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(tokenTag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemmaTag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexemeTag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(formTag);
    LayerTagMask segmentMask = lattice.getLayerTagManager().getMask(segmentTag);
    AnnotationItem senSegment("sen");
    AnnotationItem wordToken("T");
    AnnotationItem punctuationToken("I");
    AnnotationItem blankToken("B");

    Lattice::VertexDescriptor pre_ala = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ala = lattice.getVertexForRawCharIndex(3);
    Lattice::VertexDescriptor pre_ma = lattice.getVertexForRawCharIndex(4);
    Lattice::VertexDescriptor post_ma = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_kota = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_kota = lattice.getVertexForRawCharIndex(11);
    Lattice::VertexDescriptor pre_dot = lattice.getVertexForRawCharIndex(11);
    Lattice::VertexDescriptor post_dot = lattice.getVertexForRawCharIndex(12);
    Lattice::VertexDescriptor pre_koty = lattice.getVertexForRawCharIndex(13);
    Lattice::VertexDescriptor post_koty = lattice.getVertexForRawCharIndex(17);
    Lattice::VertexDescriptor pre_sa = lattice.getVertexForRawCharIndex(18);
    Lattice::VertexDescriptor post_sa = lattice.getVertexForRawCharIndex(21);
    Lattice::VertexDescriptor pre_czarne = lattice.getVertexForRawCharIndex(22);
    Lattice::VertexDescriptor post_czarne = lattice.getVertexForRawCharIndex(27);
    Lattice::VertexDescriptor pre_dot2 = lattice.getVertexForRawCharIndex(27);
    Lattice::VertexDescriptor post_dot2 = lattice.getLastVertex();

    Lattice::EdgeSequence::Builder sentence1Builder(lattice);
    for (int i = 0; i < 12; i ++) {
        sentence1Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_ala, post_dot, senSegment, segmentTag, sentence1Builder.build());
    Lattice::EdgeSequence::Builder sentence2Builder(lattice);
    for (int i = 13; i < 28; i ++) {
        if (i == 20)
            continue;
        sentence2Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_koty, post_dot2, senSegment, segmentTag, sentence2Builder.build());

    Lattice::EdgeSequence::Builder alaBuilder(lattice);
    for (int i = 0; i < 3; i ++) {
        alaBuilder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_ala, post_ala, wordToken, tokenTag, alaBuilder.build());
    Lattice::EdgeSequence::Builder blank1Builder(lattice);
    for (int i = 3; i < 4; i ++) {
        blank1Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(post_ala, pre_ma, blankToken, tokenTag, blank1Builder.build());
    Lattice::EdgeSequence::Builder maBuilder(lattice);
    for (int i = 4; i < 6; i ++) {
        maBuilder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_ma, post_ma, wordToken, tokenTag, maBuilder.build());
    Lattice::EdgeSequence::Builder blank2Builder(lattice);
    for (int i = 6; i < 7; i ++) {
        blank2Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(post_ma, pre_kota, blankToken, tokenTag, blank2Builder.build());
    Lattice::EdgeSequence::Builder kotaBuilder(lattice);
    for (int i = 7; i < 11; i ++) {
        kotaBuilder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_kota, post_kota, wordToken, tokenTag, kotaBuilder.build());
    Lattice::EdgeSequence::Builder dot1Builder(lattice);
    for (int i = 11; i < 12; i ++) {
        dot1Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_dot, post_dot, punctuationToken, tokenTag, dot1Builder.build());
    Lattice::EdgeSequence::Builder blank3Builder(lattice);
    for (int i = 12; i < 13; i ++) {
        blank3Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(post_dot, pre_koty, blankToken, tokenTag, blank3Builder.build());
    Lattice::EdgeSequence::Builder kotyBuilder(lattice);
    for (int i = 13; i < 17; i ++) {
        kotyBuilder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_koty, post_koty, wordToken, tokenTag, kotyBuilder.build());
    Lattice::EdgeSequence::Builder blank4Builder(lattice);
    for (int i = 17; i < 18; i ++) {
        blank4Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(post_koty, pre_sa, blankToken, tokenTag, blank4Builder.build());
    Lattice::EdgeSequence::Builder saBuilder(lattice);
    for (int i = 18; i < 20; i ++) {
        saBuilder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_sa, post_sa, wordToken, tokenTag, saBuilder.build());
    Lattice::EdgeSequence::Builder blank5Builder(lattice);
    for (int i = 21; i < 22; i ++) {
        blank5Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(post_sa, pre_czarne, blankToken, tokenTag, blank5Builder.build());
    Lattice::EdgeSequence::Builder czarneBuilder(lattice);
    for (int i = 22; i < 27; i ++) {
        czarneBuilder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_czarne, post_czarne, wordToken, tokenTag, czarneBuilder.build());
    Lattice::EdgeSequence::Builder dot2Builder(lattice);
    for (int i = 27; i < 28; i ++) {
        dot2Builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    lattice.addEdge(pre_dot2, post_dot2, punctuationToken, tokenTag, dot2Builder.build());

    Lattice::EdgeSequence::Builder alaLemmaBuilder(lattice);
    alaLemmaBuilder.addEdge(lattice.firstOutEdge(
                pre_ala, tokenMask));
    AnnotationItem alaLemmaAI("word", StringFrag("Ala"));
    lattice.addEdge(pre_ala, post_ala, alaLemmaAI, lemmaTag,
            alaLemmaBuilder.build());
    Lattice::EdgeSequence::Builder alaLexemeBuilder(lattice);
    alaLexemeBuilder.addEdge(lattice.firstOutEdge(
                pre_ala, lemmaMask));
    AnnotationItem alaLexemeAI("subst", StringFrag("Ala+subst"));
    lattice.getAnnotationItemManager().setValue(alaLexemeAI, "gender", "f");
    lattice.addEdge(pre_ala, post_ala, alaLexemeAI, lexemeTag,
            alaLexemeBuilder.build());
    Lattice::EdgeSequence::Builder alaFormBuilder(lattice);
    alaFormBuilder.addEdge(lattice.firstOutEdge(
                pre_ala, lexemeMask));
    AnnotationItem alaFormAI("subst", StringFrag("Ala"));
    lattice.getAnnotationItemManager().setValue(alaFormAI, "gender", "f");
    lattice.getAnnotationItemManager().setValue(alaFormAI, "case", "nom");
    lattice.getAnnotationItemManager().setValue(alaFormAI, "number", "sg");
    lattice.addEdge(pre_ala, post_ala, alaFormAI, formTag,
            alaFormBuilder.build());

    Lattice::EdgeSequence::Builder maLemmaBuilder(lattice);
    maLemmaBuilder.addEdge(lattice.firstOutEdge(
                pre_ma, tokenMask));
    AnnotationItem maLemmaAI("word", StringFrag("mieć"));
    lattice.addEdge(pre_ma, post_ma, maLemmaAI, lemmaTag,
            maLemmaBuilder.build());
    Lattice::EdgeSequence::Builder maLexemeBuilder(lattice);
    maLexemeBuilder.addEdge(lattice.firstOutEdge(
                pre_ma, lemmaMask));
    AnnotationItem maLexemeAI("verb", StringFrag("mieć+verb"));
    lattice.getAnnotationItemManager().setValue(maLexemeAI, "aspect", "imperf");
    lattice.addEdge(pre_ma, post_ma, maLexemeAI, lexemeTag,
            maLexemeBuilder.build());
    Lattice::EdgeSequence::Builder maFormBuilder(lattice);
    maFormBuilder.addEdge(lattice.firstOutEdge(
                pre_ma, lexemeMask));
    AnnotationItem maFormAI("verb", StringFrag("ma"));
    lattice.getAnnotationItemManager().setValue(maFormAI, "number", "sg");
    lattice.getAnnotationItemManager().setValue(maFormAI, "aspect", "imperf");
    lattice.getAnnotationItemManager().setValue(maFormAI, "person", "ter");
    lattice.getAnnotationItemManager().setValue(maFormAI, "tense", "fin");
    lattice.addEdge(pre_ma, post_ma, maFormAI, formTag,
            maFormBuilder.build());

    Lattice::EdgeSequence::Builder kotaLemmaBuilder(lattice);
    kotaLemmaBuilder.addEdge(lattice.firstOutEdge(
                pre_kota, tokenMask));
    AnnotationItem kotaLemmaAI("word", StringFrag("kot"));
    lattice.addEdge(pre_kota, post_kota, kotaLemmaAI, lemmaTag,
            kotaLemmaBuilder.build());
    Lattice::EdgeSequence::Builder kotaLexemeBuilder(lattice);
    kotaLexemeBuilder.addEdge(lattice.firstOutEdge(
                pre_kota, lemmaMask));
    AnnotationItem kotaLexemeAI("subst", StringFrag("kot+subst"));
    lattice.getAnnotationItemManager().setValue(kotaLexemeAI, "gender", "m2");
    lattice.addEdge(pre_kota, post_kota, kotaLexemeAI, lexemeTag,
            kotaLexemeBuilder.build());
    Lattice::EdgeSequence::Builder kotaFormBuilder(lattice);
    kotaFormBuilder.addEdge(lattice.firstOutEdge(
                pre_kota, lexemeMask));
    AnnotationItem kotaFormAI("subst", StringFrag("kota"));
    lattice.getAnnotationItemManager().setValue(kotaFormAI, "gender", "m2");
    lattice.getAnnotationItemManager().setValue(kotaFormAI, "case", "acc");
    lattice.getAnnotationItemManager().setValue(kotaFormAI, "number", "sg");
    lattice.addEdge(pre_kota, post_kota, kotaFormAI, formTag,
            kotaFormBuilder.build());

    Lattice::EdgeSequence::Builder kotyLemmaBuilder(lattice);
    kotyLemmaBuilder.addEdge(lattice.firstOutEdge(
                pre_koty, tokenMask));
    AnnotationItem kotyLemmaAI("word", StringFrag("kot"));
    lattice.addEdge(pre_koty, post_koty, kotyLemmaAI, lemmaTag,
            kotyLemmaBuilder.build());
    Lattice::EdgeSequence::Builder kotyLexemeBuilder(lattice);
    kotyLexemeBuilder.addEdge(lattice.firstOutEdge(
                pre_koty, lemmaMask));
    AnnotationItem kotyLexemeAI("subst", StringFrag("kot+subst"));
    lattice.getAnnotationItemManager().setValue(kotyLexemeAI, "gender", "m2");
    lattice.addEdge(pre_koty, post_koty, kotyLexemeAI, lexemeTag,
            kotyLexemeBuilder.build());
    Lattice::EdgeSequence::Builder kotyFormBuilder(lattice);
    kotyFormBuilder.addEdge(lattice.firstOutEdge(
                pre_koty, lexemeMask));
    AnnotationItem kotyFormAI("subst", StringFrag("koty"));
    lattice.getAnnotationItemManager().setValue(kotyFormAI, "gender", "m2");
    lattice.getAnnotationItemManager().setValue(kotyFormAI, "case", "nom");
    lattice.getAnnotationItemManager().setValue(kotyFormAI, "number", "pl");
    lattice.addEdge(pre_koty, post_koty, kotyFormAI, formTag,
            kotyFormBuilder.build());

    Lattice::EdgeSequence::Builder saLemmaBuilder(lattice);
    saLemmaBuilder.addEdge(lattice.firstOutEdge(
                pre_sa, tokenMask));
    AnnotationItem saLemmaAI("word", StringFrag("kot"));
    lattice.addEdge(pre_sa, post_sa, saLemmaAI, lemmaTag,
            saLemmaBuilder.build());
    Lattice::EdgeSequence::Builder saLexemeBuilder(lattice);
    saLexemeBuilder.addEdge(lattice.firstOutEdge(
                pre_sa, lemmaMask));
    AnnotationItem saLexemeAI("verb", StringFrag("być+verb"));
    lattice.getAnnotationItemManager().setValue(saLexemeAI, "aspect", "imperf");
    lattice.addEdge(pre_sa, post_sa, saLexemeAI, lexemeTag,
            saLexemeBuilder.build());
    Lattice::EdgeSequence::Builder saFormBuilder(lattice);
    saFormBuilder.addEdge(lattice.firstOutEdge(
                pre_sa, lexemeMask));
    AnnotationItem saFormAI("verb", StringFrag("są"));
    lattice.getAnnotationItemManager().setValue(saFormAI, "number", "pl");
    lattice.getAnnotationItemManager().setValue(saFormAI, "aspect", "imperf");
    lattice.getAnnotationItemManager().setValue(saFormAI, "person", "ter");
    lattice.getAnnotationItemManager().setValue(saFormAI, "tense", "fin");
    lattice.addEdge(pre_sa, post_sa, saFormAI, formTag,
            saFormBuilder.build());

    Lattice::EdgeSequence::Builder czarneLemmaBuilder(lattice);
    czarneLemmaBuilder.addEdge(lattice.firstOutEdge(
                pre_czarne, tokenMask));
    AnnotationItem czarneLemmaAI("word", StringFrag("czarny"));
    lattice.addEdge(pre_czarne, post_czarne, czarneLemmaAI, lemmaTag,
            czarneLemmaBuilder.build());
    Lattice::EdgeSequence::Builder czarneLexemeBuilder(lattice);
    czarneLexemeBuilder.addEdge(lattice.firstOutEdge(
                pre_czarne, lemmaMask));
    AnnotationItem czarneLexemeAI("adj", StringFrag("czarny+adj"));
    lattice.addEdge(pre_czarne, post_czarne, czarneLexemeAI, lexemeTag,
            czarneLexemeBuilder.build());
    Lattice::EdgeSequence::Builder czarneFormBuilder(lattice);
    czarneFormBuilder.addEdge(lattice.firstOutEdge(
                pre_czarne, lexemeMask));
    AnnotationItem czarneFormAI("adj", StringFrag("czarne"));
    lattice.getAnnotationItemManager().setValue(czarneFormAI, "gender", "m2");
    lattice.getAnnotationItemManager().setValue(czarneFormAI, "case", "nom");
    lattice.getAnnotationItemManager().setValue(czarneFormAI, "number", "pl");
    lattice.getAnnotationItemManager().setValue(czarneFormAI, "degree", "pos");
    lattice.addEdge(pre_czarne, post_czarne, czarneFormAI, formTag,
            czarneFormBuilder.build());

    std::string modelFile = ROOT_DIR "tools/taggers/metagger/t/files/pl.blm";
    boost::scoped_ptr<MeTagger> trainer(new MeTagger(
            langCode,
            true,
            modelFile
            ) );
    trainer->train(lattice);
    trainer->saveModel(trainer->getModelFile());

    Lattice test_lattice(aim, "Kobieta ma psa.");
    test_lattice.addSymbols(test_lattice.getFirstVertex(), test_lattice.getLastVertex());

    symbolTag
        = test_lattice.getLayerTagManager().createSingletonTagCollection("symbol");
    tokenTag
        = test_lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "token", langCode);
    lemmaTag
        = test_lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "lemma", langCode);
    lexemeTag
        = test_lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "lexeme", langCode);
    formTag
        = test_lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "form", langCode);
    segmentTag
        = test_lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(
            "segment", langCode);
    symbolMask = test_lattice.getLayerTagManager().getMask(symbolTag);
    tokenMask = test_lattice.getLayerTagManager().getMask(tokenTag);
    lemmaMask = test_lattice.getLayerTagManager().getMask(lemmaTag);
    lexemeMask = test_lattice.getLayerTagManager().getMask(lexemeTag);
    formMask = test_lattice.getLayerTagManager().getMask(formTag);
    segmentMask = test_lattice.getLayerTagManager().getMask(segmentTag);

    Lattice::VertexDescriptor test_pre_kobieta = test_lattice.getFirstVertex();
    Lattice::VertexDescriptor test_post_kobieta = test_lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor test_pre_blank1 = test_lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor test_post_blank1 = test_lattice.getVertexForRawCharIndex(8);
    Lattice::VertexDescriptor test_pre_ma = test_lattice.getVertexForRawCharIndex(8);
    Lattice::VertexDescriptor test_post_ma = test_lattice.getVertexForRawCharIndex(10);
    Lattice::VertexDescriptor test_pre_blank2 = test_lattice.getVertexForRawCharIndex(10);
    Lattice::VertexDescriptor test_post_blank2 = test_lattice.getVertexForRawCharIndex(11);
    Lattice::VertexDescriptor test_pre_psa = test_lattice.getVertexForRawCharIndex(11);
    Lattice::VertexDescriptor test_post_psa = test_lattice.getVertexForRawCharIndex(14);
    Lattice::VertexDescriptor test_pre_dot1 = test_lattice.getVertexForRawCharIndex(14);
    Lattice::VertexDescriptor test_post_dot1 = test_lattice.getVertexForRawCharIndex(15);

    Lattice::EdgeSequence::Builder test_sentence1Builder(test_lattice);
    for (int i = 0; i < 15; i ++) {
        test_sentence1Builder.addEdge(test_lattice.firstOutEdge(
                    test_lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    test_lattice.addEdge(test_pre_kobieta, test_post_dot1, senSegment,
            segmentTag, test_sentence1Builder.build());

    Lattice::EdgeSequence::Builder test_kobietaBuilder(test_lattice);
    for (int i = 0; i < 7; i ++) {
        test_kobietaBuilder.addEdge(test_lattice.firstOutEdge(
                    test_lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    test_lattice.addEdge(test_pre_kobieta, test_post_kobieta, wordToken,
            tokenTag, test_kobietaBuilder.build());
    Lattice::EdgeSequence::Builder test_blank1Builder(test_lattice);
    for (int i = 7; i < 8; i ++) {
        test_blank1Builder.addEdge(test_lattice.firstOutEdge(
                    test_lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    test_lattice.addEdge(test_pre_blank1, test_post_blank1, blankToken,
            tokenTag, test_blank1Builder.build());
    Lattice::EdgeSequence::Builder test_maBuilder(test_lattice);
    for (int i = 8; i < 10; i ++) {
        test_maBuilder.addEdge(test_lattice.firstOutEdge(
                    test_lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    test_lattice.addEdge(test_pre_ma, test_post_ma, wordToken, tokenTag, test_maBuilder.build());
    Lattice::EdgeSequence::Builder test_blank2Builder(test_lattice);
    for (int i = 10; i < 11; i ++) {
        test_blank2Builder.addEdge(test_lattice.firstOutEdge(
                    test_lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    test_lattice.addEdge(test_pre_blank2, test_post_blank2, blankToken,
            tokenTag, test_blank2Builder.build());
    Lattice::EdgeSequence::Builder test_psaBuilder(test_lattice);
    for (int i = 11; i < 14; i ++) {
        test_psaBuilder.addEdge(test_lattice.firstOutEdge(
                    test_lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    test_lattice.addEdge(test_pre_psa, test_post_psa, wordToken, tokenTag, test_psaBuilder.build());
    Lattice::EdgeSequence::Builder test_dot1Builder(test_lattice);
    for (int i = 14; i < 15; i ++) {
        test_dot1Builder.addEdge(test_lattice.firstOutEdge(
                    test_lattice.getVertexForRawCharIndex(i),
                    symbolMask
                    ));
    }
    test_lattice.addEdge(test_pre_dot1, test_post_dot1, punctuationToken,
            tokenTag, test_dot1Builder.build());

    Lattice::EdgeSequence::Builder test_kobietaLemmaBuilder(test_lattice);
    test_kobietaLemmaBuilder.addEdge(test_lattice.firstOutEdge(
                test_pre_kobieta, tokenMask));
    AnnotationItem test_kobietaLemmaAI("word", StringFrag("kobieta"));
    test_lattice.addEdge(test_pre_kobieta, test_post_kobieta, test_kobietaLemmaAI, lemmaTag,
            test_kobietaLemmaBuilder.build());
    Lattice::EdgeSequence::Builder test_kobietaLexemeBuilder(test_lattice);
    test_kobietaLexemeBuilder.addEdge(test_lattice.firstOutEdge(
                test_pre_kobieta, lemmaMask));
    AnnotationItem test_kobietaLexemeAI("subst", StringFrag("kobieta+subst"));
    test_lattice.getAnnotationItemManager().setValue(test_kobietaLexemeAI, "gender", "f");
    test_lattice.addEdge(test_pre_kobieta, test_post_kobieta, test_kobietaLexemeAI, lexemeTag,
            test_kobietaLexemeBuilder.build());
    Lattice::EdgeSequence::Builder test_kobietaFormBuilder(test_lattice);
    test_kobietaFormBuilder.addEdge(test_lattice.firstOutEdge(
                test_pre_kobieta, lexemeMask));
    AnnotationItem test_kobietaFormAI("subst", StringFrag("kobieta"));
    test_lattice.getAnnotationItemManager().setValue(test_kobietaFormAI, "gender", "f");
    test_lattice.getAnnotationItemManager().setValue(test_kobietaFormAI, "case", "nom");
    test_lattice.getAnnotationItemManager().setValue(test_kobietaFormAI, "number", "sg");
    test_lattice.addEdge(test_pre_kobieta, test_post_kobieta, test_kobietaFormAI, formTag,
            test_kobietaFormBuilder.build());

    Lattice::EdgeSequence::Builder test_maLemmaBuilder(test_lattice);
    test_maLemmaBuilder.addEdge(test_lattice.firstOutEdge(
                test_pre_ma, tokenMask));
    AnnotationItem test_maLemmaVerbAI("word", StringFrag("mieć"));
    test_lattice.addEdge(test_pre_ma, test_post_ma, test_maLemmaVerbAI, lemmaTag,
            test_maLemmaBuilder.build());
    AnnotationItem test_maLemmaAdjAI("word", StringFrag("mój"));
    test_lattice.addEdge(test_pre_ma, test_post_ma, test_maLemmaAdjAI, lemmaTag,
            test_maLemmaBuilder.build());
    Lattice::EdgeSequence::Builder test_maLexemeVerbBuilder(test_lattice);
    Lattice::EdgeSequence::Builder test_maLexemeAdjBuilder(test_lattice);
    Lattice::InOutEdgesIterator maLemmaEdgeIt = test_lattice.outEdges(test_pre_ma, lemmaMask);
    while (maLemmaEdgeIt.hasNext()) {
        Lattice::EdgeDescriptor lemma = maLemmaEdgeIt.next();
        std::string text = test_lattice.getEdgeAnnotationItem(lemma).getText();
        if (text == "mieć")
            test_maLexemeVerbBuilder.addEdge(lemma);
        else if (text == "mój")
            test_maLexemeAdjBuilder.addEdge(lemma);
    }
    AnnotationItem test_maLexemeVerbAI("verb", StringFrag("mieć+verb"));
    test_lattice.getAnnotationItemManager().setValue(test_maLexemeVerbAI, "mieć", "imperf");
    test_lattice.addEdge(test_pre_ma, test_post_ma, test_maLexemeVerbAI, lexemeTag,
            test_maLexemeVerbBuilder.build());
    AnnotationItem test_maLexemeAdjAI("adj", StringFrag("mój+adj"));
    test_lattice.addEdge(test_pre_ma, test_post_ma, test_maLexemeAdjAI, lexemeTag,
            test_maLexemeAdjBuilder.build());
    Lattice::EdgeSequence::Builder test_maFormVerbBuilder(test_lattice);
    Lattice::EdgeSequence::Builder test_maFormAdjBuilder(test_lattice);
    Lattice::InOutEdgesIterator maLexemeEdgeIt = test_lattice.outEdges(test_pre_ma, lexemeMask);
    while (maLexemeEdgeIt.hasNext()) {
        Lattice::EdgeDescriptor lexeme = maLexemeEdgeIt.next();
        std::string category = test_lattice.getEdgeAnnotationItem(lexeme).getCategory();
        if (category == "verb")
            test_maFormVerbBuilder.addEdge(lexeme);
        else if (category == "adj")
            test_maFormAdjBuilder.addEdge(lexeme);
    }
    AnnotationItem test_maFormVerbAI("verb", StringFrag("mieć"));
    test_lattice.getAnnotationItemManager().setValue(test_maFormVerbAI, "number", "sg");
    test_lattice.getAnnotationItemManager().setValue(test_maFormVerbAI, "aspect", "imperf");
    test_lattice.getAnnotationItemManager().setValue(test_maFormVerbAI, "person", "ter");
    test_lattice.getAnnotationItemManager().setValue(test_maFormVerbAI, "tense", "fin");
    test_lattice.addEdge(test_pre_ma, test_post_ma, test_maFormVerbAI, formTag,
            test_maFormVerbBuilder.build());
    AnnotationItem test_maFormAdjNomAI("adj", StringFrag("ma"));
    test_lattice.getAnnotationItemManager().setValue(test_maFormAdjNomAI, "gender", "f");
    test_lattice.getAnnotationItemManager().setValue(test_maFormAdjNomAI, "case", "nom");
    test_lattice.getAnnotationItemManager().setValue(test_maFormAdjNomAI, "number", "sg");
    test_lattice.getAnnotationItemManager().setValue(test_maFormAdjNomAI, "degree", "pos");
    test_lattice.addEdge(test_pre_ma, test_post_ma, test_maFormAdjNomAI, formTag,
            test_maFormAdjBuilder.build());
    AnnotationItem test_maFormAdjVocAI("adj", StringFrag("ma"));
    test_lattice.getAnnotationItemManager().setValue(test_maFormAdjVocAI, "gender", "f");
    test_lattice.getAnnotationItemManager().setValue(test_maFormAdjVocAI, "case", "voc");
    test_lattice.getAnnotationItemManager().setValue(test_maFormAdjVocAI, "number", "sg");
    test_lattice.getAnnotationItemManager().setValue(test_maFormAdjVocAI, "degree", "pos");
    test_lattice.addEdge(test_pre_ma, test_post_ma, test_maFormAdjVocAI, formTag,
            test_maFormAdjBuilder.build());

    Lattice::EdgeSequence::Builder test_psaLemmaBuilder(test_lattice);
    test_psaLemmaBuilder.addEdge(test_lattice.firstOutEdge(
                test_pre_psa, tokenMask));
    AnnotationItem test_psaLemmaAI("word", StringFrag("pies"));
    test_lattice.addEdge(test_pre_psa, test_post_psa, test_psaLemmaAI, lemmaTag,
            test_psaLemmaBuilder.build());
    Lattice::EdgeSequence::Builder test_psaLexemeBuilder(test_lattice);
    test_psaLexemeBuilder.addEdge(test_lattice.firstOutEdge(
                test_pre_psa, lemmaMask));
    AnnotationItem test_psaLexemeAI("subst", StringFrag("pies+subst"));
    test_lattice.getAnnotationItemManager().setValue(test_psaLexemeAI, "gender", "m2");
    test_lattice.addEdge(test_pre_psa, test_post_psa, test_psaLexemeAI, lexemeTag,
            test_psaLexemeBuilder.build());
    Lattice::EdgeSequence::Builder test_psaFormBuilder(test_lattice);
    test_psaFormBuilder.addEdge(test_lattice.firstOutEdge(
                test_pre_psa, lexemeMask));
    AnnotationItem test_psaFormAccAI("subst", StringFrag("psa"));
    test_lattice.getAnnotationItemManager().setValue(test_psaFormAccAI, "gender", "m2");
    test_lattice.getAnnotationItemManager().setValue(test_psaFormAccAI, "case", "acc");
    test_lattice.getAnnotationItemManager().setValue(test_psaFormAccAI, "number", "sg");
    test_lattice.addEdge(test_pre_psa, test_post_psa, test_psaFormAccAI, formTag,
            test_psaFormBuilder.build());
    AnnotationItem test_psaFormGenAI("subst", StringFrag("psa"));
    test_lattice.getAnnotationItemManager().setValue(test_psaFormGenAI, "gender", "m2");
    test_lattice.getAnnotationItemManager().setValue(test_psaFormGenAI, "case", "gen");
    test_lattice.getAnnotationItemManager().setValue(test_psaFormGenAI, "number", "sg");
    test_lattice.addEdge(test_pre_psa, test_post_psa, test_psaFormGenAI, formTag,
            test_psaFormBuilder.build());

    boost::scoped_ptr<MeTagger> tagger(new MeTagger(
            langCode,
            true,
            modelFile
            ));
    tagger->loadModel(tagger->getModelFile());
    tagger->tag(test_lattice);

    LayerTagMask discardedMask = test_lattice.getLayerTagManager().getMask("discarded");
    Lattice::EdgesSortedBySourceIterator discardedIt =
        test_lattice.edgesSortedBySource(discardedMask);
    BOOST_CHECK(discardedIt.hasNext());
    Lattice::EdgeDescriptor discardedEdge = discardedIt.next();
    AnnotationItem discardedItem = test_lattice.getEdgeAnnotationItem(discardedEdge);
    BOOST_CHECK_EQUAL(discardedItem.getText(), "ma");
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "adj");
    std::list< std::pair<std::string, std::string> > av =
        test_lattice.getAnnotationItemManager().getValues(
            discardedItem
            );
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "f");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "nom");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "degree");
    BOOST_CHECK_EQUAL((*avi).second, "pos");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(discardedIt.hasNext());
    discardedEdge = discardedIt.next();
    discardedItem = test_lattice.getEdgeAnnotationItem(discardedEdge);
    BOOST_CHECK_EQUAL(discardedItem.getText(), "ma");
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "adj");
    av = test_lattice.getAnnotationItemManager().getValues(
            discardedItem
            );
    avi = av.begin();
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "f");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "voc");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "degree");
    BOOST_CHECK_EQUAL((*avi).second, "pos");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(discardedIt.hasNext());
    discardedEdge = discardedIt.next();
    discardedItem = test_lattice.getEdgeAnnotationItem(discardedEdge);
    BOOST_CHECK_EQUAL(discardedItem.getText(), "psa");
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "subst");
    av = test_lattice.getAnnotationItemManager().getValues(
            discardedItem
            );
    avi = av.begin();
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m2");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "gen");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!discardedIt.hasNext());
}

BOOST_AUTO_TEST_SUITE_END()
