#include "tests.hpp"

#include "../puddle.hpp"
#include "../tagset_loader.hpp"
#include "../rule_loader.hpp"

BOOST_AUTO_TEST_SUITE( puddle )

BOOST_AUTO_TEST_CASE( puddle ) {

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
    AnnotationItem word_token("word");
    AnnotationItem blank_token("blank");
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

    Lattice::EdgeSequence::Builder blanc_builder;
    for (int i = 0; i < 5; i ++) {
        blanc_builder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(i),
                                  rawMask
                                  ));
    }
    lattice.addEdge(pre_blanc, post_blanc, word_token, token_tag, blanc_builder.build());

    Lattice::EdgeSequence::Builder blank_builder;
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(6),
                              rawMask
                              ));
    lattice.addEdge(post_blanc, pre_chat, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder chat_builder;
    for (int i = 6; i < 10; i ++) {
        chat_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 ));
    }
    lattice.addEdge(pre_chat, post_chat, word_token, token_tag, chat_builder.build());

    Lattice::EdgeSequence::Builder blanc_lemma_builder;
    blanc_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_blanc_lemma("word", "blanc");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lemma, lemma_tag, blanc_lemma_builder.build());
    Lattice::EdgeSequence::Builder blanc_lexeme_builder;
    blanc_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_blanc_lexeme_adj("adj", "blanc_adj");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lexeme_adj, lexeme_tag, blanc_lexeme_builder.build());
    AnnotationItem ai_blanc_lexeme_subst("subst", "blanc_subst");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lexeme_subst, lexeme_tag, blanc_lexeme_builder.build());
    Lattice::EdgeSequence::Builder blanc_form_adj_builder;
    Lattice::EdgeSequence::Builder blanc_form_subst_builder;
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
    AnnotationItem ai_blanc_form_adj("adj", "blanc_adj");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_adj, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_adj, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_adj, "discard", "0");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_form_adj, form_tag, blanc_form_adj_builder.build());
    AnnotationItem ai_blanc_form_subst("subst", "blanc_subst");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_subst, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_subst, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form_subst, "discard", "0");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_form_subst, form_tag, blanc_form_subst_builder.build());

    Lattice::EdgeSequence::Builder chat_lemma_builder;
    chat_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), tokenMask));
    AnnotationItem ai_chat_lemma("word", "chat");
    lattice.addEdge(pre_chat, post_chat, ai_chat_lemma, lemma_tag, chat_lemma_builder.build());
    Lattice::EdgeSequence::Builder chat_lexeme_builder;
    chat_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), lemmaMask));
    AnnotationItem ai_chat_lexeme("subst", "chat_subst");
    lattice.addEdge(pre_chat, post_chat, ai_chat_lexeme, lexeme_tag, chat_lexeme_builder.build());
    Lattice::EdgeSequence::Builder chat_form_builder;
    chat_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), lexemeMask));
    AnnotationItem ai_chat_form("subst", "chat_subst");
    lattice.getAnnotationItemManager().setValue(ai_chat_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_chat_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_chat_form, "discard", "0");
    lattice.addEdge(pre_chat, post_chat, ai_chat_form, form_tag, chat_form_builder.build());

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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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

//    void testPuddleUnify() {
//        //preparing lattice
//        Lattice lattice("ananas noirs");
//        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
//        LayerTagCollection raw_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
//        LayerTagCollection token_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("token");
//        LayerTagCollection lemma_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
//        LayerTagCollection lexeme_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lexeme");
//        LayerTagCollection form_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("form");
//        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
//        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
//        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
//        LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
//        LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
//
//        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
//        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
//        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
//        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
//
//        AnnotationItem word_token("word");
//        AnnotationItem blank_token("blank");
//
//        Lattice::EdgeSequence::Builder ananas_builder;
//        for (int i = 0; i < 6; i ++) {
//            ananas_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        ));
//        }
//        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
//
//        Lattice::EdgeSequence::Builder blank_builder;
//        blank_builder.addEdge(lattice.firstOutEdge(
//            lattice.getVertexForRawCharIndex(6),
//            rawMask
//        ));
//        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
//        Lattice::EdgeSequence::Builder noirs_builder;
//        for (int i = 7; i < 12; i ++) {
//            noirs_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        )
//                    );
//        }
//        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
//
//        Lattice::EdgeSequence::Builder ananas_lemma_builder;
//        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
//        AnnotationItem ai_ananas_lemma("word", "ananas");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag, ananas_lemma_builder.build());
//        Lattice::EdgeSequence::Builder ananas_lexeme_builder;
//        ananas_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
//        AnnotationItem ai_ananas_lexeme("subst", "ananas_subst");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag, ananas_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder ananas_form_builder;
//        ananas_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
//        AnnotationItem ai_ananas_sg_form("subst", "ananas_subst");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "morpho", "sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg_form, form_tag, ananas_form_builder.build());
//        AnnotationItem ai_ananas_pl_form("subst", "ananas_subst");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl_form, form_tag, ananas_form_builder.build());
//
//        Lattice::EdgeSequence::Builder noirs_lemma_builder;
//        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
//        AnnotationItem ai_noirs_lemma("word", "noir");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag, noirs_lemma_builder.build());
//        Lattice::EdgeSequence::Builder noirs_lexeme_builder;
//        noirs_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
//        AnnotationItem ai_noirs_lexeme_adj("adj", "noir_adj");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme_adj, lexeme_tag, noirs_lexeme_builder.build());
//        AnnotationItem ai_noirs_lexeme_subst("subst", "noir_subst");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme_subst, lexeme_tag, noirs_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder noirs_form_adj_builder;
//        Lattice::EdgeSequence::Builder noirs_form_subst_builder;
//        Lattice::InOutEdgesIterator noirsLexemeEdgeIt =
//            lattice.outEdges(lattice.getVertexForRawCharIndex(7), lexemeMask);
//        while (noirsLexemeEdgeIt.hasNext()) {
//            Lattice::EdgeDescriptor edge = noirsLexemeEdgeIt.next();
//            std::string category = lattice.getEdgeAnnotationItem(edge).getCategory();
//            if (category == "adj")
//                noirs_form_adj_builder.addEdge(edge);
//            else if (category == "subst")
//                noirs_form_subst_builder.addEdge(edge);
//        }
//        AnnotationItem ai_noirs_adj_form("adj", "noir_adj");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_form, form_tag, noirs_form_adj_builder.build());
//        AnnotationItem ai_noirs_subst_form("subst", "noir_subst");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_form, form_tag, noirs_form_subst_builder.build());
//
//        //preparing parser
//        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
//        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_unify.fr";
//        poleng::bonsai::puddle::TagsetLoader tagset_loader;
//        poleng::bonsai::puddle::RuleLoader rule_loader;
//        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
//
//        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
//
//        poleng::bonsai::puddle::TagsetPtr tagset;
//        tagset = tagset_loader.load(tagsetFilename);
//        tagger->setTagset(tagset);
//        puddle->setTagset(tagset);
//        rule_loader.setTagset(tagset);
//        puddle->setTagger(tagger);
//        poleng::bonsai::puddle::RulesPtr rules =
//            rule_loader.readFromFile(rulesFilename);
//        puddle->setRules(rules);
//        BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);
//
//        //parsing
//        BOOST_CHECK(puddle->parse(lattice));
//
//        Lattice::EdgeDescriptor edge;
//        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(formMask);
//        BOOST_CHECK(tokenIter.hasNext());
//        edge = tokenIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_ananas_sg_form.getCategory()
//        );
//        std::list< std::pair<std::string, std::string> > av
//            = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "sg:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "1"); //deleted
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(tokenIter.hasNext());
//        edge = tokenIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_ananas_pl_form.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(tokenIter.hasNext());
//        edge = tokenIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_noirs_adj_form.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(tokenIter.hasNext());
//        edge = tokenIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_noirs_subst_form.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(!tokenIter.hasNext());
//
//    }

//    void testPuddleDelete() {
//        //preparing lattice
//        Lattice lattice("ananas noirs");
//        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
//        LayerTagCollection raw_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
//        LayerTagCollection token_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("token");
//        LayerTagCollection lemma_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
//        LayerTagCollection lexeme_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lexeme");
//        LayerTagCollection form_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("form");
//        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
//        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
//        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
//        LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
//        LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
//
//        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
//        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
//        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
//        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
//
//        AnnotationItem word_token("word");
//        AnnotationItem blank_token("blank");
//
//        Lattice::EdgeSequence::Builder ananas_builder;
//        for (int i = 0; i < 6; i ++) {
//            ananas_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        ));
//        }
//        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
//
//        Lattice::EdgeSequence::Builder blank_builder;
//        blank_builder.addEdge(lattice.firstOutEdge(
//            lattice.getVertexForRawCharIndex(6),
//            rawMask
//        ));
//        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
//        Lattice::EdgeSequence::Builder noirs_builder;
//        for (int i = 7; i < 12; i ++) {
//            noirs_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        )
//                    );
//        }
//        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
//
//        Lattice::EdgeSequence::Builder ananas_lemma_builder;
//        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
//        AnnotationItem ai_ananas_lemma("word", "ananas");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag, ananas_lemma_builder.build());
//        Lattice::EdgeSequence::Builder ananas_lexeme_builder;
//        ananas_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
//        AnnotationItem ai_ananas_lexeme("subst", "ananas_subst");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag, ananas_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder ananas_form_builder;
//        ananas_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
//        AnnotationItem ai_ananas_sg_form("subst", "ananas_subst");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "morpho", "sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg_form, form_tag, ananas_form_builder.build());
//        AnnotationItem ai_ananas_pl_form("subst", "ananas_subst");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl_form, form_tag, ananas_form_builder.build());
//
//        Lattice::EdgeSequence::Builder noirs_lemma_builder;
//        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
//        AnnotationItem ai_noirs_lemma("word", "noir");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag, noirs_lemma_builder.build());
//        Lattice::EdgeSequence::Builder noirs_lexeme_builder;
//        noirs_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
//        AnnotationItem ai_noirs_adj_lexeme("adj", "noir_adj");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_lexeme, lexeme_tag, noirs_lexeme_builder.build());
//        AnnotationItem ai_noirs_subst_lexeme("subst", "noir_subst");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_lexeme, lexeme_tag, noirs_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder noirs_form_adj_builder;
//        Lattice::EdgeSequence::Builder noirs_form_subst_builder;
//        Lattice::InOutEdgesIterator noirsLexemeEdgeIt =
//            lattice.outEdges(lattice.getVertexForRawCharIndex(7), lexemeMask);
//        while (noirsLexemeEdgeIt.hasNext()) {
//            Lattice::EdgeDescriptor edge = noirsLexemeEdgeIt.next();
//            std::string category = lattice.getEdgeAnnotationItem(edge).getCategory();
//            if (category == "adj")
//                noirs_form_adj_builder.addEdge(edge);
//            else if (category == "subst")
//                noirs_form_subst_builder.addEdge(edge);
//        }
//        AnnotationItem ai_noirs_adj_form("adj", "noir_adj");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_form, form_tag, noirs_form_adj_builder.build());
//        AnnotationItem ai_noirs_subst_form("subst", "noir_subst");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_form, form_tag, noirs_form_subst_builder.build());
//
//        //preparing parser
//        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
//        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_delete.fr";
//        poleng::bonsai::puddle::TagsetLoader tagset_loader;
//        poleng::bonsai::puddle::RuleLoader rule_loader;
//        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
//
//        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
//
//        poleng::bonsai::puddle::TagsetPtr tagset;
//        tagset = tagset_loader.load(tagsetFilename);
//        tagger->setTagset(tagset);
//        puddle->setTagset(tagset);
//        rule_loader.setTagset(tagset);
//        puddle->setTagger(tagger);
//        poleng::bonsai::puddle::RulesPtr rules =
//            rule_loader.readFromFile(rulesFilename);
//        puddle->setRules(rules);
//        BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);
//
//        //parsing
//        BOOST_CHECK(puddle->parse(lattice));
//
//        Lattice::EdgeDescriptor edge;
//        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(formMask);
//        BOOST_CHECK(tokenIter.hasNext());
//        edge = tokenIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_ananas_sg_form.getCategory()
//        );
//        std::list< std::pair<std::string, std::string> > av
//            = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "sg:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "1"); //deleted
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(tokenIter.hasNext());
//        edge = tokenIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_ananas_pl_form.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(tokenIter.hasNext());
//        edge = tokenIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_noirs_adj_form.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(tokenIter.hasNext());
//        edge = tokenIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_noirs_subst_form.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "1"); //deleted
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(!tokenIter.hasNext());
//
//    }

BOOST_AUTO_TEST_CASE( puddle_add ) {
    //preparing lattice
    Lattice lattice("ananas noirs");
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

    Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();

    AnnotationItem word_token("word");
    AnnotationItem blank_token("blank");

    Lattice::EdgeSequence::Builder ananas_builder;
    for (int i = 0; i < 6; i ++) {
        ananas_builder.addEdge(lattice.firstOutEdge(
                                   lattice.getVertexForRawCharIndex(i),
                                   rawMask
                                   ));
    }
    lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());

    Lattice::EdgeSequence::Builder blank_builder;
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(6),
                              rawMask
                              ));
    lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder noirs_builder;
    for (int i = 7; i < 12; i ++) {
        noirs_builder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(i),
                                  rawMask
                                  )
            );
    }
    lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());

    Lattice::EdgeSequence::Builder ananas_lemma_builder;
    ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_ananas_lemma("word", "ananas");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag, ananas_lemma_builder.build());
    Lattice::EdgeSequence::Builder ananas_lexeme_builder;
    ananas_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_ananas_lexeme("subst", "ananas_subst");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag, ananas_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ananas_form_builder;
    ananas_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_ananas_sg_form("subst", "ananas_subst");
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_ananas_sg_form, "discard", "0");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg_form, form_tag, ananas_form_builder.build());
    AnnotationItem ai_ananas_pl_form("subst", "ananas_subst");
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_ananas_pl_form, "discard", "0");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl_form, form_tag, ananas_form_builder.build());

    Lattice::EdgeSequence::Builder noirs_lemma_builder;
    noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_noirs_lemma("word", "noir");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag, noirs_lemma_builder.build());
    Lattice::EdgeSequence::Builder noirs_lexeme_builder;
    noirs_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_noirs_adj_lexeme("adj", "noir_adj");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_lexeme, lexeme_tag, noirs_lexeme_builder.build());
    AnnotationItem ai_noirs_subst_lexeme("subst", "noir_subst");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_lexeme, lexeme_tag, noirs_lexeme_builder.build());
    Lattice::EdgeSequence::Builder noirs_form_adj_builder;
    Lattice::EdgeSequence::Builder noirs_form_subst_builder;
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
    AnnotationItem ai_noirs_adj_form("adj", "noir_adj");
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_noirs_adj_form, "discard", "0");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj_form, form_tag, noirs_form_adj_builder.build());
    AnnotationItem ai_noirs_subst_form("subst", "noir_subst");
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_noirs_subst_form, "discard", "0");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst_form, form_tag, noirs_form_subst_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_add.fr";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;
    poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(new poleng::bonsai::puddle::Puddle());

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    tagger->setTagset(tagset);
    puddle->setTagset(tagset);
    rule_loader.setTagset(tagset);
    puddle->setTagger(tagger);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    puddle->setRules(rules);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);

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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!tokenIter.hasNext());

}

BOOST_AUTO_TEST_CASE( puddle_group ) {
    //preparing lattice
    Lattice lattice("ananas noirs");
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

    Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();

    AnnotationItem word_token("word");
    AnnotationItem blank_token("blank");

    Lattice::EdgeSequence::Builder ananas_builder;
    for (int i = 0; i < 6; i ++) {
        ananas_builder.addEdge(lattice.firstOutEdge(
                                   lattice.getVertexForRawCharIndex(i),
                                   rawMask
                                   ));
    }
    lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());

    Lattice::EdgeSequence::Builder blank_builder;
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(6),
                              rawMask
                              ));
    lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder noirs_builder;
    for (int i = 7; i < 12; i ++) {
        noirs_builder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(i),
                                  rawMask
                                  )
            );
    }
    lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());

    Lattice::EdgeSequence::Builder ananas_lemma_builder;
    ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_ananas_lemma("word", "ananas");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag, ananas_lemma_builder.build());
    Lattice::EdgeSequence::Builder ananas_lexeme_builder;
    ananas_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_ananas_lexeme("subst", "ananas_subst");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag, ananas_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ananas_form_builder;
    ananas_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_ananas_form_sg("subst", "ananas_subst");
    lattice.getAnnotationItemManager().setValue(ai_ananas_form_sg, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_ananas_form_sg, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_ananas_form_sg, "discard", "0");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form_sg, form_tag, ananas_form_builder.build());
    AnnotationItem ai_ananas_form_pl("subst", "ananas_subst");
    lattice.getAnnotationItemManager().setValue(ai_ananas_form_pl, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_ananas_form_pl, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_ananas_form_pl, "discard", "0");
    lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form_pl, form_tag, ananas_form_builder.build());

    Lattice::EdgeSequence::Builder noirs_lemma_builder;
    noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_noirs_lemma("word", "noir");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag, noirs_lemma_builder.build());
    Lattice::EdgeSequence::Builder noirs_lexeme_adj_builder;
    noirs_lexeme_adj_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_noirs_lexeme_adj("adj", "noir_adj");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme_adj, lexeme_tag, noirs_lexeme_adj_builder.build());
    Lattice::EdgeSequence::Builder noirs_lexeme_subst_builder;
    noirs_lexeme_subst_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_noirs_lexeme_subst("subst", "noir_subst");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme_subst, lexeme_tag, noirs_lexeme_subst_builder.build());
    Lattice::EdgeSequence::Builder noirs_form_adj_builder;
    Lattice::EdgeSequence::Builder noirs_form_subst_builder;
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
    AnnotationItem ai_noirs_form_adj("adj", "noir_adj");
    lattice.getAnnotationItemManager().setValue(ai_noirs_form_adj, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_form_adj, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_noirs_form_adj, "discard", "0");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form_adj, form_tag, noirs_form_adj_builder.build());
    AnnotationItem ai_noirs_form_subst("subst", "noir_subst");
    lattice.getAnnotationItemManager().setValue(ai_noirs_form_subst, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_noirs_form_subst, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_noirs_form_subst, "discard", "0");
    lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form_subst, form_tag, noirs_form_subst_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_group.fr";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;
    poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(new poleng::bonsai::puddle::Puddle());

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    tagger->setTagset(tagset);
    puddle->setTagset(tagset);
    rule_loader.setTagset(tagset);
    puddle->setTagger(tagger);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    puddle->setRules(rules);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);

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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "orth");
    BOOST_CHECK_EQUAL((*avi).second, "ananas");
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "orth");
    BOOST_CHECK_EQUAL((*avi).second, "ananas");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "head");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "subst"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            6
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK( avi == av.end() );
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "adj"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            7
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            5
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK( avi == av.end() );
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "subst"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            7
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            5
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                lattice.getEdgeAnnotationItem(edge)
//                );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK( avi == av.end() );
    BOOST_CHECK(!parseIter.hasNext());

}

//    void testPuddleJoin() {
//        //preparing lattice
//        Lattice lattice("ananas noirs");
//        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
//        LayerTagCollection raw_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
//        LayerTagCollection token_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("token");
//        LayerTagCollection lemma_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
//        LayerTagCollection lexeme_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lexeme");
//        LayerTagCollection form_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("form");
//        LayerTagCollection parse_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("parse");
//        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
//        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
//        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
//        LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
//        LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
//        LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
//
//        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
//        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
//        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
//        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
//
//        AnnotationItem word_token("word");
//        AnnotationItem blank_token("blank");
//
//        Lattice::EdgeSequence::Builder ananas_builder;
//        for (int i = 0; i < 6; i ++) {
//            ananas_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        ));
//        }
//        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
//
//        Lattice::EdgeSequence::Builder blank_builder;
//        blank_builder.addEdge(lattice.firstOutEdge(
//            lattice.getVertexForRawCharIndex(6),
//            rawMask
//        ));
//        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
//        Lattice::EdgeSequence::Builder noirs_builder;
//        for (int i = 7; i < 12; i ++) {
//            noirs_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        )
//                    );
//        }
//        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
//
//        Lattice::EdgeSequence::Builder ananas_lemma_builder;
//        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
//        AnnotationItem ai_ananas_lemma("word", "ananas");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag, ananas_lemma_builder.build());
//        Lattice::EdgeSequence::Builder ananas_lexeme_builder;
//        ananas_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
//        AnnotationItem ai_ananas_lexeme("subst", "ananas_subst");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag, ananas_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder ananas_form_builder;
//        ananas_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
//        AnnotationItem ai_ananas_form("subst", "ananas_subst");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_form, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form, form_tag, ananas_form_builder.build());
//
//        Lattice::EdgeSequence::Builder noirs_lemma_builder;
//        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
//        AnnotationItem ai_noirs_lemma("word", "noir");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag, noirs_lemma_builder.build());
//        Lattice::EdgeSequence::Builder noirs_lexeme_builder;
//        noirs_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
//        AnnotationItem ai_noirs_lexeme("adj", "noir_adj");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme, lexeme_tag, noirs_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder noirs_form_builder;
//        noirs_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lexemeMask));
//        AnnotationItem ai_noirs_form("adj", "noir_adj");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_form, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form, form_tag, noirs_form_builder.build());
//
//        Lattice::EdgeSequence::Builder np_builder;
//        np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), formMask));
//        AnnotationItem ai_np("NP");
//        lattice.getAnnotationItemManager().setValue(ai_np, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_np, "discard", "0");
//        lattice.getAnnotationItemManager().setValue(ai_np, "orth", "ananas");
//        lattice.getAnnotationItemManager().setValue(ai_np, "head", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());
//
//        Lattice::EdgeSequence::Builder ap_builder;
//        ap_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), formMask));
//        AnnotationItem ai_ap("AP");
//        lattice.getAnnotationItemManager().setValue(ai_ap, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ap, "discard", "0");
//        lattice.getAnnotationItemManager().setValue(ai_ap, "orth", "noirs");
//        lattice.getAnnotationItemManager().setValue(ai_ap, "head", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_ap, parse_tag, ap_builder.build());
//
//        //preparing parser
//        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
//        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_join.fr";
//        poleng::bonsai::puddle::TagsetLoader tagset_loader;
//        poleng::bonsai::puddle::RuleLoader rule_loader;
//        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
//
//        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
//
//        poleng::bonsai::puddle::TagsetPtr tagset;
//        tagset = tagset_loader.load(tagsetFilename);
//        tagger->setTagset(tagset);
//        puddle->setTagset(tagset);
//        rule_loader.setTagset(tagset);
//        puddle->setTagger(tagger);
//        poleng::bonsai::puddle::RulesPtr rules =
//            rule_loader.readFromFile(rulesFilename);
//        puddle->setRules(rules);
//        BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);
//
//        //parsing
//        BOOST_CHECK(puddle->parse(lattice));
//
//        Lattice::EdgeDescriptor edge;
//        Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(parseMask);
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "NP"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            6
//        );
//        std::list< std::pair<std::string, std::string> > av
//            = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "1");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "orth");
//        BOOST_CHECK_EQUAL((*avi).second, "ananas");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "head");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "NP"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            12
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "orth");
//        BOOST_CHECK_EQUAL((*avi).second, "ananas");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "head");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "AP"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            7
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            5
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "1");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "orth");
//        BOOST_CHECK_EQUAL((*avi).second, "noirs");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "head");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(!parseIter.hasNext());
//
//    }

//    void testPuddleAttach() {
//        //preparing lattice
//        Lattice lattice("ananas noirs");
//        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
//        LayerTagCollection raw_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
//        LayerTagCollection token_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("token");
//        LayerTagCollection lemma_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
//        LayerTagCollection lexeme_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lexeme");
//        LayerTagCollection form_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("form");
//        LayerTagCollection parse_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("parse");
//        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
//        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
//        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
//        LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
//        LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
//        LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
//
//        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
//        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
//        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
//        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
//
//        AnnotationItem word_token("word");
//        AnnotationItem blank_token("blank");
//
//        Lattice::EdgeSequence::Builder ananas_builder;
//        for (int i = 0; i < 6; i ++) {
//            ananas_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        ));
//        }
//        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
//
//        Lattice::EdgeSequence::Builder blank_builder;
//        blank_builder.addEdge(lattice.firstOutEdge(
//            lattice.getVertexForRawCharIndex(6),
//            rawMask
//        ));
//        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
//        Lattice::EdgeSequence::Builder noirs_builder;
//        for (int i = 7; i < 12; i ++) {
//            noirs_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        )
//                    );
//        }
//        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
//
//        Lattice::EdgeSequence::Builder ananas_lemma_builder;
//        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
//        AnnotationItem ai_ananas_lemma("word", "ananas");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag, ananas_lemma_builder.build());
//        Lattice::EdgeSequence::Builder ananas_lexeme_builder;
//        ananas_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
//        AnnotationItem ai_ananas_lexeme("subst", "ananas_subst");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag, ananas_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder ananas_form_builder;
//        ananas_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
//        AnnotationItem ai_ananas_form("subst", "ananas_subst");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_form, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form, form_tag, ananas_form_builder.build());
//
//        Lattice::EdgeSequence::Builder noirs_lemma_builder;
//        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
//        AnnotationItem ai_noirs_lemma("word", "noir");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag, noirs_lemma_builder.build());
//        Lattice::EdgeSequence::Builder noirs_lexeme_builder;
//        noirs_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
//        AnnotationItem ai_noirs_lexeme("adj", "noir_adj");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme, lexeme_tag, noirs_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder noirs_form_builder;
//        noirs_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lexemeMask));
//        AnnotationItem ai_noirs_form("adj", "noir_adj");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_form, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form, form_tag, noirs_form_builder.build());
//
//        Lattice::EdgeSequence::Builder np_builder;
//        np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), formMask));
//        AnnotationItem ai_np("NP");
//        lattice.getAnnotationItemManager().setValue(ai_np, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_np, "discard", "0");
//        lattice.getAnnotationItemManager().setValue(ai_np, "orth", "ananas");
//        lattice.getAnnotationItemManager().setValue(ai_np, "head", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());
//
//        Lattice::EdgeSequence::Builder ap_builder;
//        ap_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), formMask));
//        AnnotationItem ai_ap("AP");
//        lattice.getAnnotationItemManager().setValue(ai_ap, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ap, "discard", "0");
//        lattice.getAnnotationItemManager().setValue(ai_ap, "orth", "noirs");
//        lattice.getAnnotationItemManager().setValue(ai_ap, "head", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_ap, parse_tag, ap_builder.build());
//
//        //preparing parser
//        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
//        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_attach.fr";
//        poleng::bonsai::puddle::TagsetLoader tagset_loader;
//        poleng::bonsai::puddle::RuleLoader rule_loader;
//        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
//
//        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
//
//        poleng::bonsai::puddle::TagsetPtr tagset;
//        tagset = tagset_loader.load(tagsetFilename);
//        tagger->setTagset(tagset);
//        puddle->setTagset(tagset);
//        rule_loader.setTagset(tagset);
//        puddle->setTagger(tagger);
//        poleng::bonsai::puddle::RulesPtr rules =
//            rule_loader.readFromFile(rulesFilename);
//        puddle->setRules(rules);
//        BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);
//
//        //parsing
//        BOOST_CHECK(puddle->parse(lattice));
//
//        Lattice::EdgeDescriptor edge;
//        Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(parseMask);
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "NP"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            6
//        );
//        std::list< std::pair<std::string, std::string> > av
//            = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "1");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "orth");
//        BOOST_CHECK_EQUAL((*avi).second, "ananas");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "head");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "NP"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            12
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "orth");
//        BOOST_CHECK_EQUAL((*avi).second, "ananas");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "head");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "AP"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            7
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            5
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "orth");
//        BOOST_CHECK_EQUAL((*avi).second, "noirs");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "head");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(!parseIter.hasNext());
//
//    }

//    void testPuddleTransform() {
//        //preparing lattice
//        Lattice lattice("ananas noirs");
//        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
//        LayerTagCollection raw_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
//        LayerTagCollection token_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("token");
//        LayerTagCollection lemma_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
//        LayerTagCollection lexeme_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lexeme");
//        LayerTagCollection form_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("form");
//        LayerTagCollection parse_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("parse");
//        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
//        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
//        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
//        LayerTagMask lexemeMask = lattice.getLayerTagManager().getMask(lexeme_tag);
//        LayerTagMask formMask = lattice.getLayerTagManager().getMask(form_tag);
//        LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
//
//        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
//        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
//        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
//        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
//
//        AnnotationItem word_token("word");
//        AnnotationItem blank_token("blank");
//
//        Lattice::EdgeSequence::Builder ananas_builder;
//        for (int i = 0; i < 6; i ++) {
//            ananas_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        ));
//        }
//        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
//
//        Lattice::EdgeSequence::Builder blank_builder;
//        blank_builder.addEdge(lattice.firstOutEdge(
//            lattice.getVertexForRawCharIndex(6),
//            rawMask
//        ));
//        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
//        Lattice::EdgeSequence::Builder noirs_builder;
//        for (int i = 7; i < 12; i ++) {
//            noirs_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        )
//                    );
//        }
//        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
//
//        Lattice::EdgeSequence::Builder ananas_lemma_builder;
//        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
//        AnnotationItem ai_ananas_lemma("word", "ananas");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lemma, lemma_tag, ananas_lemma_builder.build());
//        Lattice::EdgeSequence::Builder ananas_lexeme_builder;
//        ananas_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
//        AnnotationItem ai_ananas_lexeme("subst", "ananas_subst");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_lexeme, lexeme_tag, ananas_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder ananas_form_builder;
//        ananas_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
//        AnnotationItem ai_ananas_form("subst", "ananas_subst");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_form, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_form, form_tag, ananas_form_builder.build());
//
//        Lattice::EdgeSequence::Builder noirs_lemma_builder;
//        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
//        AnnotationItem ai_noirs_lemma("word", "noir");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lemma, lemma_tag, noirs_lemma_builder.build());
//        Lattice::EdgeSequence::Builder noirs_lexeme_builder;
//        noirs_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
//        AnnotationItem ai_noirs_lexeme("adj", "noir_adj");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_lexeme, lexeme_tag, noirs_lexeme_builder.build());
//        Lattice::EdgeSequence::Builder noirs_form_builder;
//        noirs_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lexemeMask));
//        AnnotationItem ai_noirs_form("adj", "noir_adj");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_form, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_form, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_form, form_tag, noirs_form_builder.build());
//
//        Lattice::EdgeSequence::Builder np_builder;
//        np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), formMask));
//        AnnotationItem ai_np("NP");
//        lattice.getAnnotationItemManager().setValue(ai_np, "morpho", "pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_np, "discard", "0");
//        lattice.getAnnotationItemManager().setValue(ai_np, "orth", "ananas");
//        lattice.getAnnotationItemManager().setValue(ai_np, "head", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());
//
//        //preparing parser
//        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
//        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_transform.fr";
//        poleng::bonsai::puddle::TagsetLoader tagset_loader;
//        poleng::bonsai::puddle::RuleLoader rule_loader;
//        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
//
//        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
//
//        poleng::bonsai::puddle::TagsetPtr tagset;
//        tagset = tagset_loader.load(tagsetFilename);
//        tagger->setTagset(tagset);
//        puddle->setTagset(tagset);
//        rule_loader.setTagset(tagset);
//        puddle->setTagger(tagger);
//        poleng::bonsai::puddle::RulesPtr rules =
//            rule_loader.readFromFile(rulesFilename);
//        puddle->setRules(rules);
//        BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);
//
//        //parsing
//        BOOST_CHECK(puddle->parse(lattice));
//
//        Lattice::EdgeDescriptor edge;
//        Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(parseMask);
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "NP"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            6
//        );
//        std::list< std::pair<std::string, std::string> > av
//            = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "1");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "orth");
//        BOOST_CHECK_EQUAL((*avi).second, "ananas");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "head");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(parseIter.hasNext());
//        edge = parseIter.next();
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "XP"
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        BOOST_CHECK_EQUAL(
//            lattice.getEdgeLength(edge),
//            6
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        BOOST_CHECK_EQUAL((*avi).first, "morpho");
//        BOOST_CHECK_EQUAL((*avi).second, "pl:m");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "discard");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "orth");
//        BOOST_CHECK_EQUAL((*avi).second, "ananas");
//        ++avi;
//        BOOST_CHECK(avi != av.end());
//        BOOST_CHECK_EQUAL((*avi).first, "head");
//        BOOST_CHECK_EQUAL((*avi).second, "0");
//        ++avi;
//        BOOST_CHECK(avi == av.end());
//        BOOST_CHECK(!parseIter.hasNext());
//
//    }

BOOST_AUTO_TEST_CASE( puddle_syntok ) {
    //preparing lattice
    Lattice lattice("coup d'état");
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

    Lattice::VertexDescriptor pre_coup = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_coup = lattice.getVertexForRawCharIndex(4);
    Lattice::VertexDescriptor pre_d = lattice.getVertexForRawCharIndex(5);
    Lattice::VertexDescriptor post_d = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_etat = lattice.getLastVertex();

    AnnotationItem word_token("word");
    AnnotationItem blank_token("blank");

    Lattice::EdgeSequence::Builder coup_builder;
    for (int i = 0; i < 4; i ++) {
        coup_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 ));
    }
    lattice.addEdge(pre_coup, post_coup, word_token, token_tag, coup_builder.build());

    Lattice::EdgeSequence::Builder blank_builder;
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(4),
                              rawMask
                              ));
    lattice.addEdge(post_coup, pre_d, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder d_builder;
    for (int i = 5; i < 7; i ++) {
        d_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(i),
                              rawMask
                              )
            );
    }
    lattice.addEdge(pre_d, post_d, word_token, token_tag, d_builder.build());
    Lattice::EdgeSequence::Builder etat_builder;
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

    Lattice::EdgeSequence::Builder coup_lemma_builder;
    coup_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_coup_lemma("word", "coup");
    lattice.addEdge(pre_coup, post_coup, ai_coup_lemma, lemma_tag, coup_lemma_builder.build());
    Lattice::EdgeSequence::Builder coup_lexeme_builder;
    coup_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_coup_lexeme("subst", "coup_subst");
    lattice.addEdge(pre_coup, post_coup, ai_coup_lexeme, lexeme_tag, coup_lexeme_builder.build());
    Lattice::EdgeSequence::Builder coup_form_builder;
    coup_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_coup_form("subst", "coup_subst");
    lattice.getAnnotationItemManager().setValue(ai_coup_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_coup_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_coup_form, "discard", "0");
    lattice.addEdge(pre_coup, post_coup, ai_coup_form, form_tag, coup_form_builder.build());

    Lattice::EdgeSequence::Builder d__lemma_builder;
    d__lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(5), tokenMask));
    AnnotationItem ai_d__lemma("word", "d'");
    lattice.addEdge(pre_d, post_d, ai_d__lemma, lemma_tag, d__lemma_builder.build());
    Lattice::EdgeSequence::Builder d_lemma_builder;
    d_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(5), tokenMask));
    AnnotationItem ai_d_lemma("word", "d");
    lattice.addEdge(pre_d, post_d, ai_d_lemma, lemma_tag, d_lemma_builder.build());
    Lattice::EdgeSequence::Builder de_lemma_builder;
    de_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(5), tokenMask));
    AnnotationItem ai_de_lemma("word", "de");
    lattice.addEdge(pre_d, post_d, ai_de_lemma, lemma_tag, de_lemma_builder.build());

    Lattice::EdgeSequence::Builder d__lexeme_builder;
    Lattice::EdgeSequence::Builder d_lexeme_builder;
    Lattice::EdgeSequence::Builder de_lexeme_builder;
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
    AnnotationItem ai_d__lexeme("prep", "d'_prep");
    AnnotationItem ai_d_lexeme("subst", "d_subst");
    AnnotationItem ai_de_lexeme("det", "de_det");
    lattice.addEdge(pre_d, post_d, ai_d__lexeme, lexeme_tag, d__lexeme_builder.build());
    lattice.addEdge(pre_d, post_d, ai_d_lexeme, lexeme_tag, d_lexeme_builder.build());
    lattice.addEdge(pre_d, post_d, ai_de_lexeme, lexeme_tag, de_lexeme_builder.build());

    Lattice::EdgeSequence::Builder d__form_builder;
    Lattice::EdgeSequence::Builder d_form_builder;
    Lattice::EdgeSequence::Builder de_form_builder;
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
    AnnotationItem ai_d__form("prep", "d'_prep");
    //lattice.getAnnotationItemManager().setValue(ai_d__form, "morpho", "");
    lattice.getAnnotationItemManager().setValue(ai_d__form, "discard", "0");
    lattice.addEdge(pre_d, post_d, ai_d__form, form_tag, d__form_builder.build());
    AnnotationItem ai_d_form_pl("subst", "d_subst");
    lattice.getAnnotationItemManager().setValue(ai_d_form_pl, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_d_form_pl, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_d_form_pl, "discard", "0");
    lattice.addEdge(pre_d, post_d, ai_d_form_pl, form_tag, d_form_builder.build());
    AnnotationItem ai_d_form_sg("subst", "d_subst");
    lattice.getAnnotationItemManager().setValue(ai_d_form_sg, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_d_form_sg, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_d_form_sg, "discard", "0");
    lattice.addEdge(pre_d, post_d, ai_d_form_sg, form_tag, d_form_builder.build());
    AnnotationItem ai_de_form_m("det", "de_det");
    lattice.getAnnotationItemManager().setValue(ai_de_form_m, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_de_form_m, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_de_form_m, "discard", "0");
    lattice.addEdge(pre_d, post_d, ai_de_form_m, form_tag, de_form_builder.build());
    AnnotationItem ai_de_form_f("det", "de_det");
    lattice.getAnnotationItemManager().setValue(ai_de_form_f, "number", "pl");
    lattice.getAnnotationItemManager().setValue(ai_de_form_f, "gender", "f");
    lattice.getAnnotationItemManager().setValue(ai_de_form_f, "discard", "0");
    lattice.addEdge(pre_d, post_d, ai_de_form_f, form_tag, de_form_builder.build());

    Lattice::EdgeSequence::Builder etat_lemma_builder;
    etat_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_etat_lemma("word", "état");
    lattice.addEdge(post_d, post_etat, ai_etat_lemma, lemma_tag, etat_lemma_builder.build());
    Lattice::EdgeSequence::Builder etat_lexeme_builder;
    etat_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_etat_lexeme("subst", "état_subst");
    lattice.addEdge(post_d, post_etat, ai_etat_lexeme, lexeme_tag, etat_lexeme_builder.build());
    Lattice::EdgeSequence::Builder etat_form_builder;
    etat_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lexemeMask));
    AnnotationItem ai_etat_form("subst", "état_subst");
    lattice.getAnnotationItemManager().setValue(ai_etat_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_etat_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_etat_form, "discard", "0");
    lattice.addEdge(post_d, post_etat, ai_etat_form, form_tag, etat_form_builder.build());

    //preparing parser
    std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
    std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_syntok.fr";
    poleng::bonsai::puddle::TagsetLoader tagset_loader;
    poleng::bonsai::puddle::RuleLoader rule_loader;
    poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());

    boost::scoped_ptr<poleng::bonsai::puddle::Puddle> puddle(new poleng::bonsai::puddle::Puddle());

    poleng::bonsai::puddle::TagsetPtr tagset;
    tagset = tagset_loader.load(tagsetFilename);
    tagger->setTagset(tagset);
    puddle->setTagset(tagset);
    rule_loader.setTagset(tagset);
    puddle->setTagger(tagger);
    poleng::bonsai::puddle::RulesPtr rules =
        rule_loader.readFromFile(rulesFilename);
    puddle->setRules(rules);
    BOOST_CHECK_EQUAL(rules->size(), (size_t) 1);

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
        "coup_subst"
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
        "coup d'état_subst"
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
        "coup détat_subst"
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
        "coup deétat_subst"
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
        "d'_prep"
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
        "d_subst"
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
        "de_det"
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
        "état_subst"
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
        "coup_subst"
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "coup d'état_subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "coup détat_subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "coup deétat_subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        0
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        12
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "d'_prep"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
        );
    av = lattice.getAnnotationItemManager().getValues(
        lattice.getEdgeAnnotationItem(edge)
        );
    avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "d_subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "d_subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "de_det"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "de_det"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        5
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        2
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
    BOOST_CHECK_EQUAL((*avi).second, "f");
    ++avi;
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
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
        "état_subst"
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeBeginIndex(edge),
        7
        );
    BOOST_CHECK_EQUAL(
        lattice.getEdgeLength(edge),
        5
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
    BOOST_CHECK(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "discard");
    BOOST_CHECK_EQUAL((*avi).second, "0");
    ++avi;
    BOOST_CHECK(avi == av.end());
    BOOST_CHECK(!formIter.hasNext());

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
    AnnotationItem word_token("word");
    AnnotationItem blank_token("blank");
    AnnotationItem ai("base");

    Lattice::VertexDescriptor pre_blanc = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_blanc = lattice.getVertexForRawCharIndex(5);
    Lattice::VertexDescriptor pre_chat = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor post_chat = lattice.getLastVertex();

    Lattice::EdgeSequence::Builder blanc_builder;
    for (int i = 0; i < 5; i ++) {
        blanc_builder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(i),
                                  rawMask
                                  ));
    }
    lattice.addEdge(pre_blanc, post_blanc, word_token, token_tag, blanc_builder.build());

    Lattice::EdgeSequence::Builder blank_builder;
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(6),
                              rawMask
                              ));
    lattice.addEdge(post_blanc, pre_chat, blank_token, token_tag, blank_builder.build());
    Lattice::EdgeSequence::Builder chat_builder;
    for (int i = 6; i < 10; i ++) {
        chat_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 ));
    }
    lattice.addEdge(pre_chat, post_chat, word_token, token_tag, chat_builder.build());

    Lattice::EdgeSequence::Builder blanc_lemma_builder;
    blanc_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_blanc_lemma("word", "blanc");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lemma, lemma_tag, blanc_lemma_builder.build());
    Lattice::EdgeSequence::Builder blanc_lexeme_builder;
    blanc_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_blanc_lexeme_adj("adj", "blanc_adj");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_lexeme_adj, lexeme_tag, blanc_lexeme_builder.build());
    Lattice::EdgeSequence::Builder blanc_form_builder;
    blanc_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_blanc_form("adj", "blanc_adj");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form, "number", "sg:m");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form, "gender", "m");
    lattice.getAnnotationItemManager().setValue(ai_blanc_form, "discard", "0");
    lattice.addEdge(pre_blanc, post_blanc, ai_blanc_form, form_tag, blanc_form_builder.build());

    Lattice::EdgeSequence::Builder chat_lemma_builder;
    chat_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), tokenMask));
    AnnotationItem ai_chat_lemma("word", "chat");
    lattice.addEdge(pre_chat, post_chat, ai_chat_lemma, lemma_tag, chat_lemma_builder.build());
    Lattice::EdgeSequence::Builder chat_lexeme_builder;
    chat_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), lemmaMask));
    AnnotationItem ai_chat_lexeme("subst", "chat_subst");
    lattice.addEdge(pre_chat, post_chat, ai_chat_lexeme, lexeme_tag, chat_lexeme_builder.build());
    Lattice::EdgeSequence::Builder chat_form_builder;
    chat_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), lexemeMask));
    AnnotationItem ai_chat_form("subst", "chat_subst");
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
    AnnotationItem word_token("word");
    AnnotationItem blank_token("blank");
    AnnotationItem ai("base");

    Lattice::VertexDescriptor pre_ala = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ala = lattice.getVertexForRawCharIndex(3);
    Lattice::VertexDescriptor pre_ma = lattice.getVertexForRawCharIndex(4);
    Lattice::VertexDescriptor post_ma = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_kota = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_kota = lattice.getLastVertex();

    Lattice::EdgeSequence::Builder ala_builder;
    for (int i = 0; i < 3; i ++) {
        ala_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(i),
                                rawMask
                                ));
    }
    lattice.addEdge(pre_ala, post_ala, word_token, token_tag, ala_builder.build());

    Lattice::EdgeSequence::Builder blank_builder;
    blank_builder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(3),
                              rawMask
                              ));
    lattice.addEdge(post_ala, pre_ma, blank_token, token_tag, blank_builder.build());

    Lattice::EdgeSequence::Builder ma_builder;
    for (int i = 4; i < 6; i ++) {
        ma_builder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(i),
                               rawMask
                               ));
    }
    lattice.addEdge(pre_ma, post_ma, word_token, token_tag, ma_builder.build());

    Lattice::EdgeSequence::Builder blank2_builder;
    blank2_builder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(6),
                               rawMask
                               ));
    lattice.addEdge(post_ma, pre_kota, blank_token, token_tag, blank2_builder.build());

    Lattice::EdgeSequence::Builder kota_builder;
    for (int i = 7; i < 11; i ++) {
        kota_builder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(i),
                                 rawMask
                                 ));
    }
    lattice.addEdge(pre_kota, post_kota, word_token, token_tag, kota_builder.build());

    Lattice::EdgeSequence::Builder ala_lemma_builder;
    ala_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
    AnnotationItem ai_ala_lemma("word", "Ala");
    lattice.addEdge(pre_ala, post_ala, ai_ala_lemma, lemma_tag, ala_lemma_builder.build());
    Lattice::EdgeSequence::Builder ala_lexeme_builder;
    ala_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
    AnnotationItem ai_ala_lexeme("R", "Ala_R");
    lattice.addEdge(pre_ala, post_ala, ai_ala_lexeme, lexeme_tag, ala_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ala_form_builder;
    ala_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lexemeMask));
    AnnotationItem ai_ala_form("R", "Ala_R");
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "gender", "f");
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "case", "nom");
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_ala_form, "discard", "0");
    lattice.addEdge(pre_ala, post_ala, ai_ala_form, form_tag, ala_form_builder.build());

    Lattice::EdgeSequence::Builder ma_lemma_builder;
    ma_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), tokenMask));
    AnnotationItem ai_ma_lemma("word", "mieć");
    lattice.addEdge(pre_ma, post_ma, ai_ma_lemma, lemma_tag, ma_lemma_builder.build());
    Lattice::EdgeSequence::Builder ma_lexeme_builder;
    ma_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), lemmaMask));
    AnnotationItem ai_ma_lexeme("C", "mieć_C");
    lattice.addEdge(pre_ma, post_ma, ai_ma_lexeme, lexeme_tag, ma_lexeme_builder.build());
    Lattice::EdgeSequence::Builder ma_form_builder;
    ma_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), lexemeMask));
    AnnotationItem ai_ma_form("C", "mieć_C");
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "tense", "pres");
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "person", "ter");
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "aspect", "imperf");
    lattice.getAnnotationItemManager().setValue(ai_ma_form, "discard", "0");
    lattice.addEdge(pre_ma, post_ma, ai_ma_form, form_tag, ma_form_builder.build());

    Lattice::EdgeSequence::Builder kota_lemma_builder;
    kota_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
    AnnotationItem ai_kota_lemma("word", "kot");
    lattice.addEdge(pre_kota, post_kota, ai_kota_lemma, lemma_tag, kota_lemma_builder.build());
    Lattice::EdgeSequence::Builder kota_lexeme_builder;
    kota_lexeme_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
    AnnotationItem ai_kota_lexeme("R", "kot_R");
    lattice.addEdge(pre_kota, post_kota, ai_kota_lexeme, lexeme_tag, kota_lexeme_builder.build());
    Lattice::EdgeSequence::Builder kota_form_builder;
    kota_form_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lexemeMask));
    AnnotationItem ai_kota_form("R", "kot_R");
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "gender", "m2");
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "case", "acc");
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "number", "sg");
    lattice.getAnnotationItemManager().setValue(ai_kota_form, "discard", "0");
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
