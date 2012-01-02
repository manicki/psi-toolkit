#include "tests.hpp"

#include "puddle.hpp"
#include "tagset_loader.hpp"
#include "rule_loader.hpp"

BOOST_AUTO_TEST_SUITE( puddle_general )

BOOST_AUTO_TEST_CASE( simple_parse ) {

    Lattice lattice("blanc chat");
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
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
    AnnotationItem word_token("T");
    AnnotationItem blank_token("B");
    AnnotationItem ai("base");

    Lattice::VertexDescriptor pre_blanc = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_blanc = lattice.getVertexForRawCharIndex(5);
    Lattice::VertexDescriptor pre_chat = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor post_chat = lattice.getLastVertex();

    Lattice::EdgesSortedBySourceIterator ei
        = lattice.edgesSortedBySource(lattice.getLayerTagManager().anyTag());
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'b");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'l");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'a");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'n");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'c");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "' ");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'c");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'h");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'a");
    BOOST_CHECK(ei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'t");
    BOOST_CHECK(!ei.hasNext());

    Lattice::EdgeSequence::Builder blanc_builder(lattice);
    for (int i = 0; i < 5; i ++) {
        blanc_builder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(i),
                                  rawMask
                                  ));
    }
    lattice.addEdge(pre_blanc, post_blanc, word_token, token_tag, blanc_builder.build());

    Lattice::EdgeSequence::Builder blank_builder(lattice);
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(6),
                              rawMask
                              ));
    lattice.addEdge(post_blanc, pre_chat, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder chat_builder(lattice);
    for (int i = 6; i < 10; i ++) {
        chat_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 ));
    }
    lattice.addEdge(pre_chat, post_chat, word_token, token_tag, chat_builder.build());

    Lattice::EdgeSequence::Builder blanc_lemma_builder(lattice);
    blanc_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_blanc_lemma("word", StringFrag("blanc"));
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lemma, lemma_tag,
            blanc_lemma_builder.build());
    Lattice::EdgeSequence::Builder blanc_lexeme_builder(lattice);
    blanc_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_blanc_lexeme_adj("adj", StringFrag("blanc_adj"));
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lexeme_adj, lexeme_tag,
            blanc_lexeme_builder.build());
    AnnotationItem ai_blanc_lexeme_subst("subst", StringFrag("blanc_subst"));
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lexeme_subst, lexeme_tag,
            blanc_lexeme_builder.build());
    Lattice::EdgeSequence::Builder blanc_form_adj_builder(lattice);
    Lattice::EdgeSequence::Builder blanc_form_subst_builder(lattice);
    Lattice::InOutEdgesIterator blancLexemeEdgeIt =
        lattice.outEdges(lattice.getVertexForRawCharIndex(0), lexemeMask);
    while (blancLexemeEdgeIt.hasNext()) {
        Lattice::EdgeDescriptor edge = blancLexemeEdgeIt.next();
        std::string category = lattice.getEdgeAnnotationItem(edge).getCategory();
        if (category == "adj")
            blanc_form_adj_builder.addEdge(edge);
        else if (category == "subst")
            blanc_form_subst_builder.addEdge(edge);
    }
    AnnotationItem ai_blanc_form_adj("adj", StringFrag("blanc_adj"));
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_adj, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_adj, "gender", "m");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_form_adj, form_tag,
            blanc_form_adj_builder.build());
    AnnotationItem ai_blanc_form_subst("subst", StringFrag("blanc_subst"));
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_subst, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_subst, "gender", "m");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_form_subst, form_tag,
            blanc_form_subst_builder.build());

    Lattice::EdgeSequence::Builder chat_lemma_builder(lattice);
    chat_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(6), tokenMask));
    AnnotationItem ai_chat_lemma("word", StringFrag("chat"));
    lattice.addEdge(pre_chat, post_chat, ai_chat_lemma, lemma_tag,
            chat_lemma_builder.build());
    Lattice::EdgeSequence::Builder chat_lexeme_builder(lattice);
    chat_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(6), lemmaMask));
    AnnotationItem ai_chat_lexeme("subst", StringFrag("chat_subst"));
    lattice.addEdge(pre_chat, post_chat, ai_chat_lexeme, lexeme_tag,
            chat_lexeme_builder.build());
    Lattice::EdgeSequence::Builder chat_form_builder(lattice);
    chat_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(6), lexemeMask));
    AnnotationItem ai_chat_form("subst", StringFrag("chat_subst"));
    lattice.getAnnotationItemManager().setValue(ai_chat_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_chat_form, "gender", "m");
    lattice.addEdge(pre_chat, post_chat, ai_chat_form, form_tag,
            chat_form_builder.build());

    Lattice::EdgeDescriptor edge;
    Lattice::EdgesSortedBySourceIterator lemmaIter = lattice.edgesSortedBySource(lemmaMask);
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_blanc_lemma.getCategory()
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        ai_blanc_lemma.getText()
        );
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_chat_lemma.getCategory()
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        ai_chat_lemma.getText()
        );
    BOOST_CHECK(!lemmaIter.hasNext());
    Lattice::EdgesSortedBySourceIterator lexemeIter = lattice.edgesSortedBySource(lexemeMask);
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_blanc_lexeme_adj.getCategory()
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        ai_blanc_lexeme_adj.getText()
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_blanc_lexeme_subst.getCategory()
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        ai_blanc_lexeme_subst.getText()
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_chat_lexeme.getCategory()
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        ai_chat_lexeme.getText()
        );
    BOOST_CHECK(!lexemeIter.hasNext());
    Lattice::EdgesSortedBySourceIterator formIter = lattice.edgesSortedBySource(formMask);
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_blanc_form_adj.getCategory()
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        ai_blanc_form_adj.getText()
        );
    std::list< std::pair<std::string, std::string> > av
        = lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_blanc_form_subst.getCategory()
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        ai_blanc_form_subst.getText()
        );
    av = lattice.getAnnotationItemManager().getValues(
        lattice.getEdgeAnnotationItem(edge)
        );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_chat_form.getCategory()
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        ai_chat_form.getText()
        );
    av = lattice.getAnnotationItemManager().getValues(
        lattice.getEdgeAnnotationItem(edge)
        );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!formIter.hasNext());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules.fr";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(new poleng::bonsai::puddle::Puddle());

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    puddle->setTagset(tagset);
    rule_loader.setTagset(tagset);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    puddle->setRules(rules);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);

    //parsing
    BOOST_CHECK(puddle->parse(lattice));

}

BOOST_AUTO_TEST_CASE( load_rules_fr ) {
    //preparing lattice
    Lattice lattice("blanc chat");
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
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
    AnnotationItem word_token("T");
    AnnotationItem blank_token("B");
    AnnotationItem ai("base");

    Lattice::VertexDescriptor pre_blanc = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_blanc = lattice.getVertexForRawCharIndex(5);
    Lattice::VertexDescriptor pre_chat = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor post_chat = lattice.getLastVertex();

    Lattice::EdgeSequence::Builder blanc_builder(lattice);
    for (int i = 0; i < 5; i ++) {
        blanc_builder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(i),
                                  rawMask
                                  ));
    }
    lattice.addEdge(pre_blanc, post_blanc, word_token, token_tag, blanc_builder.build());

    Lattice::EdgeSequence::Builder blank_builder(lattice);
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(6),
                              rawMask
                              ));
    lattice.addEdge(post_blanc, pre_chat, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder chat_builder(lattice);
    for (int i = 6; i < 10; i ++) {
        chat_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 ));
    }
    lattice.addEdge(pre_chat, post_chat, word_token, token_tag, chat_builder.build());

    Lattice::EdgeSequence::Builder blanc_lemma_builder(lattice);
    blanc_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_blanc_lemma("word", StringFrag("blanc"));
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lemma, lemma_tag,
            blanc_lemma_builder.build());
    Lattice::EdgeSequence::Builder blanc_lexeme_builder(lattice);
    blanc_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_blanc_lexeme_adj("adj", StringFrag("blanc_adj"));
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lexeme_adj, lexeme_tag,
            blanc_lexeme_builder.build());
    Lattice::EdgeSequence::Builder blanc_form_builder(lattice);
    blanc_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_blanc_form("adj", StringFrag("blanc_adj"));
    lattice.getAnnotationItemManager().setValue(ai_blanc_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form, "gender", "m");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_form, form_tag, blanc_form_builder.build());

    Lattice::EdgeSequence::Builder chat_lemma_builder(lattice);
    chat_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(6), tokenMask));
    AnnotationItem ai_chat_lemma("word", StringFrag("chat"));
    lattice.addEdge(pre_chat, post_chat, ai_chat_lemma, lemma_tag, chat_lemma_builder.build());
    Lattice::EdgeSequence::Builder chat_lexeme_builder(lattice);
    chat_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(6), lemmaMask));
    AnnotationItem ai_chat_lexeme("subst", StringFrag("chat_subst"));
    lattice.addEdge(pre_chat, post_chat, ai_chat_lexeme, lexeme_tag, chat_lexeme_builder.build());
    Lattice::EdgeSequence::Builder chat_form_builder(lattice);
    chat_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(6), lexemeMask));
    AnnotationItem ai_chat_form("subst", StringFrag("chat_subst"));
    lattice.getAnnotationItemManager().setValue(ai_chat_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_chat_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_chat_form, "discard", "0");
    lattice.addEdge(pre_chat, post_chat, ai_chat_form, form_tag, chat_form_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/data/fr/tagset.fr.cfg";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/data/fr/rules.fr";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(new poleng::bonsai::puddle::Puddle());

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    puddle->setTagset(tagset);
    rule_loader.setTagset(tagset);
    BOOST_CHECK_EQUAL(tagset->size(), (size_t) 28);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    puddle->setRules(rules);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 167);

    //parsing
    BOOST_CHECK(puddle->parse(lattice));

}

BOOST_AUTO_TEST_CASE( load_rules_pl ) {
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
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
    AnnotationItem word_token("T");
    AnnotationItem blank_token("B");
    AnnotationItem ai("base");

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

    Lattice::EdgeSequence::Builder ala_lemma_builder(lattice);
    ala_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_ala_lemma("word", StringFrag("Ala"));
    lattice.addEdge(pre_ala, post_ala, ai_ala_lemma, lemma_tag, ala_lemma_builder.build());
    Lattice::EdgeSequence::Builder ala_lexeme_builder(lattice);
    ala_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_ala_lexeme("R", StringFrag("Ala_R"));
    lattice.addEdge(pre_ala, post_ala, ai_ala_lexeme, lexeme_tag, ala_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ala_form_builder(lattice);
    ala_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_ala_form("R", StringFrag("Ala_R"));
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "gender", "f");
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "case", "nom");
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "number", "sg");
    lattice.addEdge(pre_ala, post_ala, ai_ala_form, form_tag, ala_form_builder.build());

    Lattice::EdgeSequence::Builder ma_lemma_builder(lattice);
    ma_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), tokenMask));
    AnnotationItem ai_ma_lemma("word", StringFrag("mieć"));
    lattice.addEdge(pre_ma, post_ma, ai_ma_lemma, lemma_tag, ma_lemma_builder.build());
    Lattice::EdgeSequence::Builder ma_lexeme_builder(lattice);
    ma_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), lemmaMask));
    AnnotationItem ai_ma_lexeme("C", StringFrag("mieć_C"));
    lattice.addEdge(pre_ma, post_ma, ai_ma_lexeme, lexeme_tag, ma_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ma_form_builder(lattice);
    ma_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), lexemeMask));
    AnnotationItem ai_ma_form("C", StringFrag("mieć_C"));
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "tense", "pres");
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "person", "ter");
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "aspect", "imperf");
    lattice.addEdge(pre_ma, post_ma, ai_ma_form, form_tag, ma_form_builder.build());

    Lattice::EdgeSequence::Builder kota_lemma_builder(lattice);
    kota_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_kota_lemma("word", StringFrag("kot"));
    lattice.addEdge(pre_kota, post_kota, ai_kota_lemma, lemma_tag, kota_lemma_builder.build());
    Lattice::EdgeSequence::Builder kota_lexeme_builder(lattice);
    kota_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_kota_lexeme("R", StringFrag("kot_R"));
    lattice.addEdge(pre_kota, post_kota, ai_kota_lexeme, lexeme_tag, kota_lexeme_builder.build());
    Lattice::EdgeSequence::Builder kota_form_builder(lattice);
    kota_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lexemeMask));
    AnnotationItem ai_kota_form("R", StringFrag("kot_R"));
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "gender", "m2");
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "case", "acc");
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
    BOOST_CHECK_EQUAL(tagset->size(), (size_t) 21);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    puddle->setRules(rules);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 274);

    //parsing
    BOOST_CHECK(puddle->parse(lattice));

}

BOOST_AUTO_TEST_SUITE_END()
