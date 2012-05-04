#include "tests.hpp"

#include "puddle.hpp"
#include "tagset_loader.hpp"
#include "rule_loader.hpp"

BOOST_AUTO_TEST_SUITE( puddle_autodelete )

BOOST_AUTO_TEST_CASE( autodelete_pl ) {
    //preparing lattice
    Lattice lattice("Ala ma kota");
    lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
    LayerTagCollection raw_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
    LayerTagCollection token_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("token");
    LayerTagCollection lemma_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
    LayerTagCollection lexeme_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("lexeme");
    LayerTagCollection form_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("form");
    LayerTagCollection parse_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("parse");
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
    LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
    AnnotationItem word_token("T");
    AnnotationItem blank_token("B");

    Lattice::VertexDescriptor pre_ala = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ala = lattice.getVertexForRawCharIndex(3);
    Lattice::VertexDescriptor pre_ma = lattice.getVertexForRawCharIndex(4);
    Lattice::VertexDescriptor post_ma = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_kota = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_kota = lattice.getLastVertex();

    Lattice::EdgeSequence::Builder ala_builder(lattice);
    for (int i = 0; i < 3; i ++) {
        ala_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(i),
                                rawMask
                                ));
    }
    lattice.addEdge(pre_ala, post_ala, word_token, token_tag, ala_builder.build());

    Lattice::EdgeSequence::Builder blank_builder(lattice);
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(3),
                              rawMask
                              ));
    lattice.addEdge(post_ala, pre_ma, blank_token, token_tag, blank_builder.build());

    Lattice::EdgeSequence::Builder ma_builder(lattice);
    for (int i = 4; i < 6; i ++) {
        ma_builder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(i),
                               rawMask
                               ));
    }
    lattice.addEdge(pre_ma, post_ma, word_token, token_tag, ma_builder.build());

    Lattice::EdgeSequence::Builder blank2_builder(lattice);
    blank2_builder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(6),
                               rawMask
                               ));
    lattice.addEdge(post_ma, pre_kota, blank_token, token_tag, blank2_builder.build());

    Lattice::EdgeSequence::Builder kota_builder(lattice);
    for (int i = 7; i < 11; i ++) {
        kota_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 ));
    }
    lattice.addEdge(pre_kota, post_kota, word_token, token_tag, kota_builder.build());

    Lattice::EdgeSequence::Builder al_lemma_builder(lattice);
    al_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_al_lemma("word", StringFrag("Al"));
    Lattice::EdgeDescriptor al_lemma_edge = lattice.addEdge(pre_ala, post_ala, ai_al_lemma, lemma_tag, al_lemma_builder.build());
    Lattice::EdgeSequence::Builder ala_lemma_builder(lattice);
    ala_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_ala_lemma("word", StringFrag("Ala"));
    Lattice::EdgeDescriptor ala_lemma_edge = lattice.addEdge(pre_ala, post_ala, ai_ala_lemma, lemma_tag, ala_lemma_builder.build());
    Lattice::EdgeSequence::Builder al_lexeme_builder(lattice);
    al_lexeme_builder.addEdge(al_lemma_edge);
    AnnotationItem ai_al_lexeme("subst", StringFrag("Al+subst"));
    lattice.getAnnotationItemManager().setValue(ai_al_lexeme, "gender", "m1");
    Lattice::EdgeDescriptor al_lexeme_edge = lattice.addEdge(pre_ala, post_ala, ai_al_lexeme, lexeme_tag, al_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ala_lexeme_builder(lattice);
    ala_lexeme_builder.addEdge(ala_lemma_edge);
    AnnotationItem ai_ala_lexeme("subst", StringFrag("Ala+subst"));
    lattice.getAnnotationItemManager().setValue(ai_ala_lexeme, "gender", "f");
    Lattice::EdgeDescriptor ala_lexeme_edge = lattice.addEdge(pre_ala, post_ala, ai_ala_lexeme, lexeme_tag, ala_lexeme_builder.build());
    Lattice::EdgeSequence::Builder al_form_gen_builder(lattice);
    al_form_gen_builder.addEdge(al_lexeme_edge);
    AnnotationItem ai_al_form_gen("subst", StringFrag("Ala"));
    lattice.getAnnotationItemManager().setValue(ai_al_form_gen, "gender", "m1");
    lattice.getAnnotationItemManager().setValue(ai_al_form_gen, "case", "gen");
    lattice.getAnnotationItemManager().setValue(ai_al_form_gen, "number", "sg");
    lattice.addEdge(pre_ala, post_ala, ai_al_form_gen, form_tag, al_form_gen_builder.build());
    Lattice::EdgeSequence::Builder al_form_acc_builder(lattice);
    al_form_acc_builder.addEdge(al_lexeme_edge);
    AnnotationItem ai_al_form_acc("subst", StringFrag("Ala"));
    lattice.getAnnotationItemManager().setValue(ai_al_form_acc, "gender", "m1");
    lattice.getAnnotationItemManager().setValue(ai_al_form_acc, "case", "acc");
    lattice.getAnnotationItemManager().setValue(ai_al_form_acc, "number", "sg");
    lattice.addEdge(pre_ala, post_ala, ai_al_form_acc, form_tag, al_form_acc_builder.build());
    Lattice::EdgeSequence::Builder ala_form_builder(lattice);
    ala_form_builder.addEdge(ala_lexeme_edge);
    AnnotationItem ai_ala_form("subst", StringFrag("Ala"));
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "gender", "f");
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "case", "nom");
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "number", "sg");
    lattice.addEdge(pre_ala, post_ala, ai_ala_form, form_tag, ala_form_builder.build());

    Lattice::EdgeSequence::Builder miec_lemma_builder(lattice);
    miec_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), tokenMask));
    AnnotationItem ai_miec_lemma("word", StringFrag("mieć"));
    Lattice::EdgeDescriptor miec_lemma_edge = lattice.addEdge(pre_ma, post_ma, ai_miec_lemma, lemma_tag, miec_lemma_builder.build());
    Lattice::EdgeSequence::Builder moj_lemma_builder(lattice);
    moj_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), tokenMask));
    AnnotationItem ai_moj_lemma("word", StringFrag("mój"));
    Lattice::EdgeDescriptor moj_lemma_edge = lattice.addEdge(pre_ma, post_ma, ai_moj_lemma, lemma_tag, moj_lemma_builder.build());
    Lattice::EdgeSequence::Builder miec_lexeme_builder(lattice);
    miec_lexeme_builder.addEdge(miec_lemma_edge);
    AnnotationItem ai_miec_lexeme("verb", StringFrag("mieć+verb"));
    lattice.getAnnotationItemManager().setValue(ai_miec_lexeme, "aspect", "imperf");
    Lattice::EdgeDescriptor miec_lexeme_edge = lattice.addEdge(pre_ma, post_ma, ai_miec_lexeme, lexeme_tag, miec_lexeme_builder.build());
    Lattice::EdgeSequence::Builder moj_lexeme_builder(lattice);
    moj_lexeme_builder.addEdge(moj_lemma_edge);
    AnnotationItem ai_moj_lexeme("adj", StringFrag("mój+adj"));
    Lattice::EdgeDescriptor moj_lexeme_edge = lattice.addEdge(pre_ma, post_ma, ai_moj_lexeme, lexeme_tag, moj_lexeme_builder.build());
    Lattice::EdgeSequence::Builder miec_form_builder(lattice);
    miec_form_builder.addEdge(miec_lexeme_edge);
    AnnotationItem ai_miec_form("verb", StringFrag("ma"));
    lattice.getAnnotationItemManager().setValue(ai_miec_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_miec_form, "aspect", "imperf");
    lattice.getAnnotationItemManager().setValue(ai_miec_form, "person", "ter");
    lattice.getAnnotationItemManager().setValue(ai_miec_form, "tense", "fin");
    lattice.addEdge(pre_ma, post_ma, ai_miec_form, form_tag, miec_form_builder.build());
    Lattice::EdgeSequence::Builder moj_form_nom_builder(lattice);
    moj_form_nom_builder.addEdge(moj_lexeme_edge);
    AnnotationItem ai_moj_nom_form("adj", StringFrag("ma"));
    lattice.getAnnotationItemManager().setValue(ai_moj_nom_form, "gender", "f");
    lattice.getAnnotationItemManager().setValue(ai_moj_nom_form, "case", "nom");
    lattice.getAnnotationItemManager().setValue(ai_moj_nom_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_moj_nom_form, "degree", "pos");
    lattice.addEdge(pre_ma, post_ma, ai_moj_nom_form, form_tag, moj_form_nom_builder.build());
    Lattice::EdgeSequence::Builder moj_form_voc_builder(lattice);
    moj_form_voc_builder.addEdge(moj_lexeme_edge);
    AnnotationItem ai_moj_voc_form("adj", StringFrag("ma"));
    lattice.getAnnotationItemManager().setValue(ai_moj_voc_form, "gender", "f");
    lattice.getAnnotationItemManager().setValue(ai_moj_voc_form, "case", "voc");
    lattice.getAnnotationItemManager().setValue(ai_moj_voc_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_moj_voc_form, "degree", "pos");
    lattice.addEdge(pre_ma, post_ma, ai_moj_voc_form, form_tag, moj_form_voc_builder.build());

    Lattice::EdgeSequence::Builder kot_lemma_builder(lattice);
    kot_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_kot_lemma("word", StringFrag("kot"));
    Lattice::EdgeDescriptor kot_lemma_edge = lattice.addEdge(pre_kota, post_kota, ai_kot_lemma, lemma_tag, kot_lemma_builder.build());
    Lattice::EdgeSequence::Builder kota_lemma_builder(lattice);
    kota_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_kota_lemma("word", StringFrag("kota"));
    Lattice::EdgeDescriptor kota_lemma_edge = lattice.addEdge(pre_kota, post_kota, ai_kota_lemma, lemma_tag, kota_lemma_builder.build());
    Lattice::EdgeSequence::Builder kot_lexeme_builder(lattice);
    kot_lexeme_builder.addEdge(kot_lemma_edge);
    AnnotationItem ai_kot_lexeme("subst", StringFrag("kot+subst"));
    lattice.getAnnotationItemManager().setValue(ai_kot_lexeme, "gender", "m2");
    Lattice::EdgeDescriptor kot_lexeme_edge = lattice.addEdge(pre_kota, post_kota, ai_kot_lexeme, lexeme_tag, kot_lexeme_builder.build());
    Lattice::EdgeSequence::Builder kota_lexeme_builder(lattice);
    kota_lexeme_builder.addEdge(kota_lemma_edge);
    AnnotationItem ai_kota_lexeme("subst", StringFrag("kota+subst"));
    lattice.getAnnotationItemManager().setValue(ai_kota_lexeme, "gender", "f");
    Lattice::EdgeDescriptor kota_lexeme_edge = lattice.addEdge(pre_kota, post_kota, ai_kota_lexeme, lexeme_tag, kota_lexeme_builder.build());
    Lattice::EdgeSequence::Builder kot_form_acc_builder(lattice);
    kot_form_acc_builder.addEdge(kot_lexeme_edge);
    AnnotationItem ai_kot_form_acc("subst", StringFrag("kota"));
    lattice.getAnnotationItemManager().setValue(ai_kot_form_acc, "gender", "m2");
    lattice.getAnnotationItemManager().setValue(ai_kot_form_acc, "case", "acc");
    lattice.getAnnotationItemManager().setValue(ai_kot_form_acc, "number", "sg");
    lattice.addEdge(pre_kota, post_kota, ai_kot_form_acc, form_tag, kot_form_acc_builder.build());
    Lattice::EdgeSequence::Builder kot_form_gen_builder(lattice);
    kot_form_gen_builder.addEdge(kot_lexeme_edge);
    AnnotationItem ai_kot_form_gen("subst", StringFrag("kota"));
    lattice.getAnnotationItemManager().setValue(ai_kot_form_gen, "gender", "m2");
    lattice.getAnnotationItemManager().setValue(ai_kot_form_gen, "case", "gen");
    lattice.getAnnotationItemManager().setValue(ai_kot_form_gen, "number", "sg");
    lattice.addEdge(pre_kota, post_kota, ai_kot_form_gen, form_tag, kot_form_gen_builder.build());
    Lattice::EdgeSequence::Builder kota_form_builder(lattice);
    kota_form_builder.addEdge(kota_lexeme_edge);
    AnnotationItem ai_kota_form("subst", StringFrag("kota"));
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "gender", "f");
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "case", "nom");
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "number", "sg");
    lattice.addEdge(pre_kota, post_kota, ai_kota_form, form_tag, kota_form_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/data/pl/tagset.pl.cfg";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/data/pl/rules.pl";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(new poleng::bonsai::puddle::Puddle());

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    puddle->setTagset(tagset);
    rule_loader.setTagset(tagset);
    //BOOST_CHECK_EQUAL(tagset->size(), (size_t) 21);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    puddle->setRules(rules);
    //BOOST_CHECK_EQUAL(rules->size(), (size_t) 274);

    //parsing
    BOOST_CHECK(puddle->parse(lattice));

    Lattice::EdgeDescriptor edge;
    Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(parseMask);
    BOOST_CHECK(parseIter.hasNext());
    edge = parseIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            "NP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            0
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            3
            );
    std::list< std::pair<std::string, std::string> > av =
        lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m1");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "gen");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(parseIter.hasNext());
    edge = parseIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            "NP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            0
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            3
            );
    av =
        lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m1");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "acc");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(parseIter.hasNext());
    edge = parseIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            "NP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            0
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            3
            );
    av =
        lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
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
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(parseIter.hasNext());
    edge = parseIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            "S"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            0
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            11
            );
    av =
        lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "aspect");
    BOOST_CHECK_EQUAL((*avi).second, "imperf");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "person");
    BOOST_CHECK_EQUAL((*avi).second, "ter");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "tense");
    BOOST_CHECK_EQUAL((*avi).second, "fin");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "1");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(parseIter.hasNext());
    edge = parseIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            "VP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            4
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            2
            );
    av =
        lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "aspect");
    BOOST_CHECK_EQUAL((*avi).second, "imperf");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "person");
    BOOST_CHECK_EQUAL((*avi).second, "ter");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "tense");
    BOOST_CHECK_EQUAL((*avi).second, "fin");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(parseIter.hasNext());
    edge = parseIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            "NP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            7
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            4
            );
    av =
        lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m2");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "acc");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(parseIter.hasNext());
    edge = parseIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            "NP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            7
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            4
            );
    av =
        lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(parseIter.hasNext());
    edge = parseIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            "NP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            7
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            4
            );
    av =
        lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
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
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!parseIter.hasNext());

    LayerTagMask discardedMask = lattice.getLayerTagManager().getMask("discarded");
    Lattice::EdgesSortedBySourceIterator discardedIter = lattice.edgesSortedBySource(discardedMask);
    BOOST_CHECK(discardedIter.hasNext());
    Lattice::EdgeDescriptor discardedEdge = discardedIter.next();
    AnnotationItem discardedItem = lattice.getEdgeAnnotationItem(discardedEdge);
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "adj");
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(discardedEdge),
            4
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(discardedEdge),
            2
            );
    av = lattice.getAnnotationItemManager().getValues(
            discardedItem
            );
    avi = av.begin();
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
    BOOST_CHECK(discardedIter.hasNext());
    discardedEdge = discardedIter.next();
    discardedItem = lattice.getEdgeAnnotationItem(discardedEdge);
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "adj");
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(discardedEdge),
            4
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(discardedEdge),
            2
            );
    av = lattice.getAnnotationItemManager().getValues(
            discardedItem
            );
    avi = av.begin();
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

    BOOST_CHECK(!discardedIter.hasNext());

}

BOOST_AUTO_TEST_SUITE_END()
