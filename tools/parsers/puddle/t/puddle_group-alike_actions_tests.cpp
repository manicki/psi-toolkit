#include "tests.hpp"

#include "puddle.hpp"
#include "tagset_loader.hpp"
#include "rule_loader.hpp"

BOOST_AUTO_TEST_SUITE( puddle_group_alike_actions )

BOOST_AUTO_TEST_CASE( puddle_attach ) {
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
    LayerTagCollection parse_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("parse", "fr");
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
    LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);

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
    AnnotationItem ai_ananas_form("subst", StringFrag("ananas"));
    aim.setValue(ai_ananas_form, "number", "pl");
    aim.setValue(ai_ananas_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form, form_tag,
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
    AnnotationItem ai_noirs_lexeme("adj", StringFrag("noir+adj"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme, lexeme_tag,
            noirs_lexeme_builder.build());
    Lattice::EdgeSequence::Builder noirs_form_builder(lattice);
    noirs_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lexemeMask));
    AnnotationItem ai_noirs_form("adj", StringFrag("noir"));
    aim.setValue(ai_noirs_form, "number", "pl");
    aim.setValue(ai_noirs_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form, form_tag, noirs_form_builder.build());

    Lattice::EdgeSequence::Builder np_builder(lattice);
    np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), formMask));
    AnnotationItem ai_np("NP");
    aim.setValue(ai_np, "number", "pl");
    aim.setValue(ai_np, "gender", "m");
    aim.setValue(ai_np, "head", "0");
    lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());

    Lattice::EdgeSequence::Builder ap_builder(lattice);
    ap_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), formMask));
    AnnotationItem ai_ap("AP");
    aim.setValue(ai_ap, "number", "pl");
    aim.setValue(ai_ap, "gender", "m");
    aim.setValue(ai_ap, "head", "0");
    lattice.addEdge(pre_noirs, post_noirs, ai_ap, parse_tag, ap_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_attach.fr";
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
            6
            );
    std::list< std::pair<std::string, std::string> > av
        = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
            12
            );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
            "AP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            7
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            5
            );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "NP");
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(discardedEdge),
            0
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(discardedEdge),
            6
            );
    av = aim.getValues(discardedItem);
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!discardedIter.hasNext());

}

BOOST_AUTO_TEST_CASE( puddle_group ) {
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
    LayerTagCollection parse_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("parse", "fr");
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
    LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);

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
    AnnotationItem ai_ananas_form_sg("subst", StringFrag("ananas"));
    aim.setValue(ai_ananas_form_sg, "number", "sg");
    aim.setValue(ai_ananas_form_sg, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form_sg, form_tag,
            ananas_form_builder.build());
    AnnotationItem ai_ananas_form_pl("subst", StringFrag("ananas"));
    aim.setValue(ai_ananas_form_pl, "number", "pl");
    aim.setValue(ai_ananas_form_pl, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form_pl, form_tag,
            ananas_form_builder.build());

    Lattice::EdgeSequence::Builder noirs_lemma_builder(lattice);
    noirs_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_noirs_lemma("word", StringFrag("noir"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag,
            noirs_lemma_builder.build());
    Lattice::EdgeSequence::Builder noirs_lexeme_adj_builder(lattice);
    noirs_lexeme_adj_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_noirs_lexeme_adj("adj", StringFrag("noir+adj"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme_adj, lexeme_tag,
            noirs_lexeme_adj_builder.build());
    Lattice::EdgeSequence::Builder noirs_lexeme_subst_builder(lattice);
    noirs_lexeme_subst_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_noirs_lexeme_subst("subst", StringFrag("noir+subst"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme_subst, lexeme_tag,
            noirs_lexeme_subst_builder.build());
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
    AnnotationItem ai_noirs_form_adj("adj", StringFrag("noir"));
    aim.setValue(ai_noirs_form_adj, "number", "pl");
    aim.setValue(ai_noirs_form_adj, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form_adj, form_tag,
            noirs_form_adj_builder.build());
    AnnotationItem ai_noirs_form_subst("subst", StringFrag("noir"));
    aim.setValue(ai_noirs_form_subst, "number", "pl");
    aim.setValue(ai_noirs_form_subst, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form_subst, form_tag,
            noirs_form_subst_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_group.fr";
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
        12
        );
    std::list< std::pair<std::string, std::string> > av
        = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
        12
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!parseIter.hasNext());

}

BOOST_AUTO_TEST_CASE( puddle_join ) {
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
    LayerTagCollection parse_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("parse", "fr");
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
    LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);

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
    AnnotationItem ai_ananas_form("subst", StringFrag("ananas"));
    aim.setValue(ai_ananas_form, "number", "pl");
    aim.setValue(ai_ananas_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form, form_tag,
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
    AnnotationItem ai_noirs_lexeme("adj", StringFrag("noir+adj"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme, lexeme_tag,
            noirs_lexeme_builder.build());
    Lattice::EdgeSequence::Builder noirs_form_builder(lattice);
    noirs_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lexemeMask));
    AnnotationItem ai_noirs_form("adj", StringFrag("noir"));
    aim.setValue(ai_noirs_form, "number", "pl");
    aim.setValue(ai_noirs_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form, form_tag, noirs_form_builder.build());

    Lattice::EdgeSequence::Builder np_builder(lattice);
    np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), formMask));
    AnnotationItem ai_np("NP");
    aim.setValue(ai_np, "number", "pl");
    aim.setValue(ai_np, "gender", "m");
    aim.setValue(ai_np, "head", "0");
    lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());

    Lattice::EdgeSequence::Builder ap_builder(lattice);
    ap_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), formMask));
    AnnotationItem ai_ap("AP");
    aim.setValue(ai_ap, "number", "pl");
    aim.setValue(ai_ap, "gender", "m");
    aim.setValue(ai_ap, "head", "0");
    lattice.addEdge(pre_noirs, post_noirs, ai_ap, parse_tag, ap_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_join.fr";
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
            6
            );
    std::list< std::pair<std::string, std::string> > av =
        aim.getValues(lattice.getEdgeAnnotationItem(edge));
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
            12
            );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
            "AP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            7
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            5
            );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "NP");
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(discardedEdge),
            0
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(discardedEdge),
            6
            );
    av = aim.getValues(discardedItem);
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(discardedIter.hasNext());
    discardedEdge = discardedIter.next();
    discardedItem = lattice.getEdgeAnnotationItem(discardedEdge);
    BOOST_CHECK_EQUAL(
            lattice.getEdgeAnnotationItem(discardedEdge).getCategory(),
            "AP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(discardedEdge),
            7
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(discardedEdge),
            5
            );
    av = aim.getValues(discardedItem);
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!discardedIter.hasNext());

}

BOOST_AUTO_TEST_CASE( puddle_syntok ) {
    //preparing lattice
    AnnotationItemManager aim;
    Lattice lattice(aim, "coup d'état");
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

    Lattice::VertexDescriptor pre_coup = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_coup = lattice.getVertexForRawCharIndex(4);
    Lattice::VertexDescriptor pre_d = lattice.getVertexForRawCharIndex(5);
    Lattice::VertexDescriptor post_d = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_etat = lattice.getLastVertex();

    AnnotationItem word_token("T");
    AnnotationItem blank_token("B");

    Lattice::EdgeSequence::Builder coup_builder(lattice);
    for (int i = 0; i < 4; i ++) {
        coup_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 ));
    }
    lattice.addEdge(pre_coup, post_coup, word_token, token_tag, coup_builder.build());

    Lattice::EdgeSequence::Builder blank_builder(lattice);
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(4),
                              rawMask
                              ));
    lattice.addEdge(post_coup, pre_d, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder d_builder(lattice);
    for (int i = 5; i < 7; i ++) {
        d_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(i),
                              rawMask
                              )
            );
    }
    lattice.addEdge(pre_d, post_d, word_token, token_tag, d_builder.build());
    Lattice::EdgeSequence::Builder etat_builder(lattice);
    for (int i = 7; i < 12; i ++) {
        if (i == 8)
            continue;
        etat_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 )
            );
    }
    lattice.addEdge(post_d, post_etat, word_token, token_tag, etat_builder.build());

    Lattice::EdgeSequence::Builder coup_lemma_builder(lattice);
    coup_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_coup_lemma("word", StringFrag("coup"));
    lattice.addEdge(pre_coup, post_coup, ai_coup_lemma, lemma_tag, coup_lemma_builder.build());
    Lattice::EdgeSequence::Builder coup_lexeme_builder(lattice);
    coup_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_coup_lexeme("subst", StringFrag("coup+subst"));
    lattice.addEdge(pre_coup, post_coup, ai_coup_lexeme, lexeme_tag, coup_lexeme_builder.build());
    Lattice::EdgeSequence::Builder coup_form_builder(lattice);
    coup_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_coup_form("subst", StringFrag("coup"));
    aim.setValue(ai_coup_form, "number", "sg");
    aim.setValue(ai_coup_form, "gender", "m");
    lattice.addEdge(pre_coup, post_coup, ai_coup_form, form_tag, coup_form_builder.build());

    Lattice::EdgeSequence::Builder d__lemma_builder(lattice);
    d__lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(5), tokenMask));
    AnnotationItem ai_d__lemma("word", StringFrag("d'"));
    lattice.addEdge(pre_d, post_d, ai_d__lemma, lemma_tag, d__lemma_builder.build());
    Lattice::EdgeSequence::Builder d_lemma_builder(lattice);
    d_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(5), tokenMask));
    AnnotationItem ai_d_lemma("word", StringFrag("d"));
    lattice.addEdge(pre_d, post_d, ai_d_lemma, lemma_tag, d_lemma_builder.build());
    Lattice::EdgeSequence::Builder de_lemma_builder(lattice);
    de_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(5), tokenMask));
    AnnotationItem ai_de_lemma("word", StringFrag("de"));
    lattice.addEdge(pre_d, post_d, ai_de_lemma, lemma_tag, de_lemma_builder.build());

    Lattice::EdgeSequence::Builder d__lexeme_builder(lattice);
    Lattice::EdgeSequence::Builder d_lexeme_builder(lattice);
    Lattice::EdgeSequence::Builder de_lexeme_builder(lattice);
    Lattice::InOutEdgesIterator dLemmaEdgeIt =
        lattice.outEdges(lattice.getVertexForRawCharIndex(5), lemmaMask);
    while (dLemmaEdgeIt.hasNext()) {
        Lattice::EdgeDescriptor edge = dLemmaEdgeIt.next();
        std::string text = lattice.getEdgeAnnotationItem(edge).getText();
        if (text == "d'")
            d__lexeme_builder.addEdge(edge);
        else if (text == "d")
            d_lexeme_builder.addEdge(edge);
        else if (text == "de")
            de_lexeme_builder.addEdge(edge);
    }
    AnnotationItem ai_d__lexeme("prep", StringFrag("d'+prep"));
    AnnotationItem ai_d_lexeme("subst", StringFrag("d+subst"));
    AnnotationItem ai_de_lexeme("det", StringFrag("de+det"));
    lattice.addEdge(pre_d, post_d, ai_d__lexeme, lexeme_tag, d__lexeme_builder.build());
    lattice.addEdge(pre_d, post_d, ai_d_lexeme, lexeme_tag, d_lexeme_builder.build());
    lattice.addEdge(pre_d, post_d, ai_de_lexeme, lexeme_tag, de_lexeme_builder.build());

    Lattice::EdgeSequence::Builder d__form_builder(lattice);
    Lattice::EdgeSequence::Builder d_form_builder(lattice);
    Lattice::EdgeSequence::Builder de_form_builder(lattice);
    Lattice::InOutEdgesIterator dLexemeEdgeIt =
        lattice.outEdges(lattice.getVertexForRawCharIndex(5), lexemeMask);
    while (dLexemeEdgeIt.hasNext()) {
        Lattice::EdgeDescriptor edge = dLexemeEdgeIt.next();
        std::string category = lattice.getEdgeAnnotationItem(edge).getCategory();
        if (category == "prep")
            d__form_builder.addEdge(edge);
        else if (category == "subst")
            d_form_builder.addEdge(edge);
        else if (category == "det")
            de_form_builder.addEdge(edge);
    }
    AnnotationItem ai_d__form("prep", StringFrag("d'"));
    aim.setValue(ai_d__form, "number", "xyz");
    // with no item in AI getTopEdges meets some bizarre behavior of the lattice
    lattice.addEdge(pre_d, post_d, ai_d__form, form_tag, d__form_builder.build());
    AnnotationItem ai_d_form_pl("subst", StringFrag("d"));
    aim.setValue(ai_d_form_pl, "number", "pl");
    aim.setValue(ai_d_form_pl, "gender", "m");
    lattice.addEdge(pre_d, post_d, ai_d_form_pl, form_tag, d_form_builder.build());
    AnnotationItem ai_d_form_sg("subst", StringFrag("d"));
    aim.setValue(ai_d_form_sg, "number", "sg");
    aim.setValue(ai_d_form_sg, "gender", "m");
    lattice.addEdge(pre_d, post_d, ai_d_form_sg, form_tag, d_form_builder.build());
    AnnotationItem ai_de_form_m("det", StringFrag("de"));
    aim.setValue(ai_de_form_m, "number", "pl");
    aim.setValue(ai_de_form_m, "gender", "m");
    lattice.addEdge(pre_d, post_d, ai_de_form_m, form_tag, de_form_builder.build());
    AnnotationItem ai_de_form_f("det", StringFrag("de"));
    aim.setValue(ai_de_form_f, "number", "pl");
    aim.setValue(ai_de_form_f, "gender", "f");
    lattice.addEdge(pre_d, post_d, ai_de_form_f, form_tag, de_form_builder.build());

    Lattice::EdgeSequence::Builder etat_lemma_builder(lattice);
    etat_lemma_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_etat_lemma("word", StringFrag("état"));
    lattice.addEdge(post_d, post_etat, ai_etat_lemma, lemma_tag, etat_lemma_builder.build());
    Lattice::EdgeSequence::Builder etat_lexeme_builder(lattice);
    etat_lexeme_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_etat_lexeme("subst", StringFrag("état+subst"));
    lattice.addEdge(post_d, post_etat, ai_etat_lexeme, lexeme_tag, etat_lexeme_builder.build());
    Lattice::EdgeSequence::Builder etat_form_builder(lattice);
    etat_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lexemeMask));
    AnnotationItem ai_etat_form("subst", StringFrag("état"));
    aim.setValue(ai_etat_form, "number", "sg");
    aim.setValue(ai_etat_form, "gender", "m");
    lattice.addEdge(post_d, post_etat, ai_etat_form, form_tag, etat_form_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_syntok.fr";
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
    Lattice::EdgesSortedBySourceIterator lemmaIter = lattice.edgesSortedBySource(lemmaMask);
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "word"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        4
        );
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "word"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup d'état"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "word"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup détat"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "word"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup deétat"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "word"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "d'"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "word"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "d"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "word"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "de"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    BOOST_CHECK(lemmaIter.hasNext());
    edge = lemmaIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "word"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "état"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        7
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        5
        );
    BOOST_CHECK(!lemmaIter.hasNext());

    Lattice::EdgesSortedBySourceIterator lexemeIter = lattice.edgesSortedBySource(lexemeMask);
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup+subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        4
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup d'état+subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup détat+subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup deétat+subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "prep"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "d'+prep"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "d+subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "det"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "de+det"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    BOOST_CHECK(lexemeIter.hasNext());
    edge = lexemeIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "état+subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        7
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        5
        );
    BOOST_CHECK(!lexemeIter.hasNext());

    Lattice::EdgesSortedBySourceIterator formIter = lattice.edgesSortedBySource(formMask);
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        4
        );
    std::list< std::pair<std::string, std::string> > av
        = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup d'état"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup détat"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "coup deétat"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "prep"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "d'"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "xyz");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "d"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "d"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "det"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "de"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "det"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "de"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "f");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(formIter.hasNext());
    edge = formIter.next();
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getCategory(),
        "subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeAnnotationItem(edge).getText(),
        "état"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        7
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        5
        );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "sg");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!formIter.hasNext());

}

BOOST_AUTO_TEST_CASE( puddle_transform ) {
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
    LayerTagCollection parse_tag
        = lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode("parse", "fr");
    LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
    LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
    LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);

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
    AnnotationItem ai_ananas_form("subst", StringFrag("ananas"));
    aim.setValue(ai_ananas_form, "number", "pl");
    aim.setValue(ai_ananas_form, "gender", "m");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form, form_tag,
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
    AnnotationItem ai_noirs_lexeme("adj", StringFrag("noir+adj"));
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme, lexeme_tag,
            noirs_lexeme_builder.build());
    Lattice::EdgeSequence::Builder noirs_form_builder(lattice);
    noirs_form_builder.addEdge(lattice.firstOutEdge(
                lattice.getVertexForRawCharIndex(7), lexemeMask));
    AnnotationItem ai_noirs_form("adj", StringFrag("noir"));
    aim.setValue(ai_noirs_form, "number", "pl");
    aim.setValue(ai_noirs_form, "gender", "m");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form, form_tag, noirs_form_builder.build());

    Lattice::EdgeSequence::Builder np_builder(lattice);
    np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), formMask));
    AnnotationItem ai_np("NP");
    aim.setValue(ai_np, "number", "pl");
    aim.setValue(ai_np, "gender", "m");
    aim.setValue(ai_np, "head", "0");
    lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_transform.fr";
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
            6
            );
    std::list< std::pair<std::string, std::string> > av
        = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
            "XP"
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(edge),
            0
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(edge),
            6
            );
    av = aim.getValues(lattice.getEdgeAnnotationItem(edge));
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
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
    BOOST_CHECK_EQUAL(discardedItem.getCategory(), "NP");
    BOOST_CHECK_EQUAL(
            lattice.getEdgeBeginIndex(discardedEdge),
            0
            );
    BOOST_CHECK_EQUAL(
            lattice.getEdgeLength(discardedEdge),
            6
            );
    av = aim.getValues(discardedItem);
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "pl");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "gender");
    BOOST_CHECK_EQUAL((*avi).second, "m");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!discardedIter.hasNext());

}

BOOST_AUTO_TEST_SUITE_END()
