#include "tests.hpp"

#include "puddle.hpp"
#include "tagset_loader.hpp"
#include "rule_loader.hpp"

BOOST_AUTO_TEST_SUITE( puddle_non_group_actions )

BOOST_AUTO_TEST_CASE( puddle_add ) {
    //preparing lattice
    AnnotationItemManager aim;
    Lattice lattice(aim, "ananas noirs");
    lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
    LayerTagCollection raw_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
    LayerTagCollection token_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("token", "fr");
    LayerTagCollection lemma_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("lemma", "fr");
    LayerTagCollection lexeme_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("lexeme", "fr");
    LayerTagCollection form_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("form", "fr");
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);

    Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();

    AnnotationItem word_token("T");
    AnnotationItem blank_token("B");

    Lattice::EdgeSequence::Builder ananas_builder(lattice);
    for (int i = 0; i < 6; i ++) {
        ananas_builder.addEdge(lattice.firstOutEdge(
                                   lattice.getVertexForRawCharIndex(i),
                                   rawMask
                                   ));
    }
    lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());

    Lattice::EdgeSequence::Builder blank_builder(lattice);
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(6),
                              rawMask
                              ));
    lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder noirs_builder(lattice);
    for (int i = 7; i < 12; i ++) {
        noirs_builder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(i),
                                  rawMask
                                  )
            );
    }
    lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());

    Lattice::EdgeSequence::Builder ananas_lemma_builder(lattice);
    ananas_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_ananas_lemma("word", StringFrag("ananas"));
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag,
            ananas_lemma_builder.build());
    Lattice::EdgeSequence::Builder ananas_lexeme_builder(lattice);
    ananas_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_ananas_lexeme("subst", StringFrag("ananas+subst"));
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag,
            ananas_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ananas_form_builder(lattice);
    ananas_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_ananas_sg_form("subst", StringFrag("ananas"));
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg_form, form_tag,
            ananas_form_builder.build());
    AnnotationItem ai_ananas_pl_form("subst", StringFrag("ananas"));
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl_form, form_tag,
            ananas_form_builder.build());

    Lattice::EdgeSequence::Builder noirs_lemma_builder(lattice);
    noirs_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_noirs_lemma("word", StringFrag("noir"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag,
            noirs_lemma_builder.build());
    Lattice::EdgeSequence::Builder noirs_lexeme_builder(lattice);
    noirs_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_noirs_adj_lexeme("adj", StringFrag("noir+adj"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_lexeme, lexeme_tag,
            noirs_lexeme_builder.build());
    AnnotationItem ai_noirs_subst_lexeme("subst", StringFrag("noir+subst"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_lexeme, lexeme_tag,
            noirs_lexeme_builder.build());
    Lattice::EdgeSequence::Builder noirs_form_adj_builder(lattice);
    Lattice::EdgeSequence::Builder noirs_form_subst_builder(lattice);
    Lattice::InOutEdgesIterator noirsLexemeEdgeIt =
        lattice.outEdges(lattice.getVertexForRawCharIndex(7), lexemeMask);
    while (noirsLexemeEdgeIt.hasNext()) {
        Lattice::EdgeDescriptor edge = noirsLexemeEdgeIt.next();
        std::string category = lattice.getEdgeAnnotationItem(edge).getCategory();
        if (category == "adj")
            noirs_form_adj_builder.addEdge(edge);
        else if (category == "subst")
            noirs_form_subst_builder.addEdge(edge);
    }
    AnnotationItem ai_noirs_adj_form("adj", StringFrag("noir"));
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_form, form_tag,
            noirs_form_adj_builder.build());
    AnnotationItem ai_noirs_subst_form("subst", StringFrag("noir"));
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_form, form_tag,
            noirs_form_subst_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_add.fr";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    rule_loader.setTagset(tagset);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(
            new poleng::bonsai::puddle::Puddle(tagset, rules, "fr"));
    //parsing
    BOOST_CHECK(puddle->parse(lattice));

    Lattice::EdgeDescriptor edge;
    Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(formMask);
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_ananas_sg_form.getCategory()
        );
    std::list< std::pair<std::string, std::string> > av
        = lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_ananas_pl_form.getCategory()
        );
    av = lattice.getAnnotationItemManager().getValues(
        lattice.getEdgeAnnotationItem(edge)
        );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "adj"
        );
    av = lattice.getAnnotationItemManager().getValues(
        lattice.getEdgeAnnotationItem(edge)
        );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "adj"
        );
    av = lattice.getAnnotationItemManager().getValues(
        lattice.getEdgeAnnotationItem(edge)
        );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_noirs_adj_form.getCategory()
        );
    av = lattice.getAnnotationItemManager().getValues(
        lattice.getEdgeAnnotationItem(edge)
        );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        ai_noirs_subst_form.getCategory()
        );
    av = lattice.getAnnotationItemManager().getValues(
        lattice.getEdgeAnnotationItem(edge)
        );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!tokenIter.hasNext());

}

BOOST_AUTO_TEST_CASE( puddle_delete ) {
    //preparing lattice
    AnnotationItemManager aim;
    Lattice lattice(aim, "ananas noirs");
    lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
    LayerTagCollection raw_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
    LayerTagCollection token_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("token", "fr");
    LayerTagCollection lemma_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("lemma", "fr");
    LayerTagCollection lexeme_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("lexeme", "fr");
    LayerTagCollection form_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("form", "fr");
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);

    Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();

    AnnotationItem word_token("T");
    AnnotationItem blank_token("B");

    Lattice::EdgeSequence::Builder ananas_builder(lattice);
    for (int i = 0; i < 6; i ++) {
        ananas_builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    rawMask
                    ));
    }
    lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());

    Lattice::EdgeSequence::Builder blank_builder(lattice);
    blank_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(6),
                rawMask
                ));
    lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder noirs_builder(lattice);
    for (int i = 7; i < 12; i ++) {
        noirs_builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    rawMask
                    )
                );
    }
    lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());

    Lattice::EdgeSequence::Builder ananas_lemma_builder(lattice);
    ananas_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_ananas_lemma("word", StringFrag("ananas"));
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag,
            ananas_lemma_builder.build());
    Lattice::EdgeSequence::Builder ananas_lexeme_builder(lattice);
    ananas_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_ananas_lexeme("subst", StringFrag("ananas+subst"));
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag,
            ananas_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ananas_form_builder(lattice);
    ananas_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_ananas_sg_form("subst", StringFrag("ananas"));
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg_form, form_tag,
            ananas_form_builder.build());
    AnnotationItem ai_ananas_pl_form("subst", StringFrag("ananas"));
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl_form, form_tag,
            ananas_form_builder.build());

    Lattice::EdgeSequence::Builder noirs_lemma_builder(lattice);
    noirs_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_noirs_lemma("word", StringFrag("noir"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag,
            noirs_lemma_builder.build());
    Lattice::EdgeSequence::Builder noirs_lexeme_builder(lattice);
    noirs_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_noirs_adj_lexeme("adj", StringFrag("noir+adj"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_lexeme, lexeme_tag,
            noirs_lexeme_builder.build());
    AnnotationItem ai_noirs_subst_lexeme("subst", StringFrag("noir+subst"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_lexeme, lexeme_tag,
            noirs_lexeme_builder.build());
    Lattice::EdgeSequence::Builder noirs_form_adj_builder(lattice);
    Lattice::EdgeSequence::Builder noirs_form_subst_builder(lattice);
    Lattice::InOutEdgesIterator noirsLexemeEdgeIt =
        lattice.outEdges(lattice.getVertexForRawCharIndex(7), lexemeMask);
    while (noirsLexemeEdgeIt.hasNext()) {
        Lattice::EdgeDescriptor edge = noirsLexemeEdgeIt.next();
        std::string category = lattice.getEdgeAnnotationItem(edge).getCategory();
        if (category == "adj")
            noirs_form_adj_builder.addEdge(edge);
        else if (category == "subst")
            noirs_form_subst_builder.addEdge(edge);
    }
    AnnotationItem ai_noirs_adj_form("adj", StringFrag("noir"));
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_form, form_tag,
            noirs_form_adj_builder.build());
    AnnotationItem ai_noirs_subst_form("subst", StringFrag("noir"));
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_form, form_tag,
            noirs_form_subst_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_delete.fr";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    rule_loader.setTagset(tagset);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(
            new poleng::bonsai::puddle::Puddle(tagset, rules, "fr"));
    //parsing
    BOOST_CHECK(puddle->parse(lattice));

    Lattice::EdgeDescriptor edge;
    Lattice::EdgesSortedBySourceIterator tokenIter =
        lattice.edgesSortedBySource(formMask);
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_ananas_sg_form.getCategory()
            );
    std::list< std::pair<std::string, std::string> > av
        = lattice.getAnnotationItemManager().getValues(
                lattice.getEdgeAnnotationItem(edge)
                );
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_ananas_pl_form.getCategory()
            );
    av = lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_noirs_adj_form.getCategory()
            );
    av = lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_noirs_subst_form.getCategory()
            );
    av = lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!tokenIter.hasNext());

    LayerTagMask discardedMask = lattice.getLayerTagManager().getMask("discarded");
    Lattice::EdgesSortedBySourceIterator discardedIter =
        lattice.edgesSortedBySource(discardedMask);

    BOOST_CHECK(discardedIter.hasNext());
    Lattice::EdgeDescriptor discardedEdge = discardedIter.next();
    AnnotationItem discardedItem = lattice.getEdgeAnnotationItem(discardedEdge);
    BOOST_CHECK_EQUAL(discardedItem.getText(), "ananas");
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "subst");
    av = lattice.getAnnotationItemManager().getValues(
            discardedItem
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(discardedIter.hasNext());
    discardedEdge = discardedIter.next();
    discardedItem = lattice.getEdgeAnnotationItem(discardedEdge); // StringFrag assignment
    BOOST_CHECK_EQUAL(discardedItem.getText(), "noir");
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "subst");
    av = lattice.getAnnotationItemManager().getValues(
            discardedItem
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    BOOST_CHECK(!discardedIter.hasNext());

}

BOOST_AUTO_TEST_CASE( puddle_unify ) {
    //preparing lattice
    AnnotationItemManager aim;
    Lattice lattice(aim, "ananas noirs");
    lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
    LayerTagCollection raw_tag
        = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
    LayerTagCollection token_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("token", "fr");
    LayerTagCollection lemma_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("lemma", "fr");
    LayerTagCollection lexeme_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("lexeme", "fr");
    LayerTagCollection form_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("form", "fr");
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);

    Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();

    AnnotationItem word_token("T");
    AnnotationItem blank_token("B");

    Lattice::EdgeSequence::Builder ananas_builder(lattice);
    for (int i = 0; i < 6; i ++) {
        ananas_builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    rawMask
                    ));
    }
    lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());

    Lattice::EdgeSequence::Builder blank_builder(lattice);
    blank_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(6),
                rawMask
                ));
    lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder noirs_builder(lattice);
    for (int i = 7; i < 12; i ++) {
        noirs_builder.addEdge(lattice.firstOutEdge(
                    lattice.getVertexForRawCharIndex(i),
                    rawMask
                    )
                );
    }
    lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());

    Lattice::EdgeSequence::Builder ananas_lemma_builder(lattice);
    ananas_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_ananas_lemma("word", StringFrag("ananas"));
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag,
            ananas_lemma_builder.build());
    Lattice::EdgeSequence::Builder ananas_lexeme_builder(lattice);
    ananas_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_ananas_lexeme("subst", StringFrag("ananas+subst"));
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag,
            ananas_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ananas_form_builder(lattice);
    ananas_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_ananas_sg_form("subst", StringFrag("ananas"));
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg_form, form_tag,
            ananas_form_builder.build());
    AnnotationItem ai_ananas_pl_form("subst", StringFrag("ananas"));
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl_form, form_tag,
            ananas_form_builder.build());

    Lattice::EdgeSequence::Builder noirs_lemma_builder(lattice);
    noirs_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_noirs_lemma("word", StringFrag("noir"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag,
            noirs_lemma_builder.build());
    Lattice::EdgeSequence::Builder noirs_lexeme_builder(lattice);
    noirs_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_noirs_lexeme_adj("adj", StringFrag("noir+adj"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme_adj, lexeme_tag,
            noirs_lexeme_builder.build());
    AnnotationItem ai_noirs_lexeme_subst("subst", StringFrag("noir+subst"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme_subst, lexeme_tag,
            noirs_lexeme_builder.build());
    Lattice::EdgeSequence::Builder noirs_form_adj_builder(lattice);
    Lattice::EdgeSequence::Builder noirs_form_subst_builder(lattice);
    Lattice::InOutEdgesIterator noirsLexemeEdgeIt =
        lattice.outEdges(lattice.getVertexForRawCharIndex(7), lexemeMask);
    while (noirsLexemeEdgeIt.hasNext()) {
        Lattice::EdgeDescriptor edge = noirsLexemeEdgeIt.next();
        std::string category = lattice.getEdgeAnnotationItem(edge).getCategory();
        if (category == "adj")
            noirs_form_adj_builder.addEdge(edge);
        else if (category == "subst")
            noirs_form_subst_builder.addEdge(edge);
    }
    AnnotationItem ai_noirs_adj_form("adj", StringFrag("noir"));
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_form, form_tag,
            noirs_form_adj_builder.build());
    AnnotationItem ai_noirs_subst_form("subst", StringFrag("noir"));
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_form, form_tag,
            noirs_form_subst_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_unify.fr";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    rule_loader.setTagset(tagset);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(
            new poleng::bonsai::puddle::Puddle(tagset, rules, "fr"));
    //parsing
    BOOST_CHECK(puddle->parse(lattice));

    Lattice::EdgeDescriptor edge;
    Lattice::EdgesSortedBySourceIterator tokenIter =
        lattice.edgesSortedBySource(formMask);
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_ananas_sg_form.getCategory()
            );
    std::list< std::pair<std::string, std::string> > av
        = lattice.getAnnotationItemManager().getValues(
                lattice.getEdgeAnnotationItem(edge)
                );
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_ananas_pl_form.getCategory()
            );
    av = lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_noirs_adj_form.getCategory()
            );
    av = lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(tokenIter.hasNext());
    edge = tokenIter.next();
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_noirs_subst_form.getCategory()
            );
    av = lattice.getAnnotationItemManager().getValues(
            lattice.getEdgeAnnotationItem(edge)
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!tokenIter.hasNext());

    LayerTagMask discardedMask = lattice.getLayerTagManager().getMask("discarded");
    Lattice::EdgesSortedBySourceIterator discardedIter =
        lattice.edgesSortedBySource(discardedMask);

    BOOST_CHECK(discardedIter.hasNext());
    Lattice::EdgeDescriptor discardedEdge = discardedIter.next();
    AnnotationItem discardedItem = lattice.getEdgeAnnotationItem(discardedEdge);
    BOOST_CHECK_EQUAL(discardedItem.getText(), "ananas");
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "subst");
    av = lattice.getAnnotationItemManager().getValues(
            discardedItem
            );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!discardedIter.hasNext());

}

BOOST_AUTO_TEST_SUITE_END()
