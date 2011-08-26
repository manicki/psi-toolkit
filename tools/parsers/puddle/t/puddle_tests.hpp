#include "puddle.hpp"
#include "tagset_loader.hpp"
#include "rule_loader.hpp"


class PuddleTests : public CxxTest::TestSuite {

public:

    void testPuddle() {
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
//        lattice.getAnnotationItemManager().setValue(ai, "base", "xxx"); //czy to jest potrzebne w ogole?
//        lattice.getAnnotationItemManager().setValue(ai, "number", "xxx");
//        lattice.getAnnotationItemManager().setValue(ai, "gender", "xxx");

        Lattice::VertexDescriptor pre_blanc = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_blanc = lattice.getVertexForRawCharIndex(5);
        Lattice::VertexDescriptor pre_chat = lattice.getVertexForRawCharIndex(6);
        Lattice::VertexDescriptor post_chat = lattice.getLastVertex();

        Lattice::EdgesSortedBySourceIterator ei
            = lattice.edgesSortedBySource(lattice.getLayerTagManager().anyTag());
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'b");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'l");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'a");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'n");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'c");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "' ");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'c");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'h");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'a");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'t");
        TS_ASSERT(!ei.hasNext());

//        Lattice::Partition blanc_partition;
//        for (int i = 0; i < 5; i ++) {
//            blanc_partition.links.push_back(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        ));
//        }
        Lattice::EdgeSequence::Builder blanc_builder;
        for (int i = 0; i < 5; i ++) {
            blanc_builder.addEdge(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }
        lattice.addEdge(pre_blanc, post_blanc, word_token, token_tag, blanc_builder.build());

//        Lattice::Partition blank_partition;
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
        lattice.getAnnotationItemManager().setValue(ai_blanc_form_adj, "morpho", "sg:m");
        lattice.getAnnotationItemManager().setValue(ai_blanc_form_adj, "discard", "0");
        lattice.addEdge(pre_blanc, post_blanc, ai_blanc_form_adj, form_tag, blanc_form_adj_builder.build());
        AnnotationItem ai_blanc_form_subst("subst", "blanc_subst");
        lattice.getAnnotationItemManager().setValue(ai_blanc_form_subst, "morpho", "sg:m");
        lattice.getAnnotationItemManager().setValue(ai_blanc_form_subst, "discard", "0");
        lattice.addEdge(pre_blanc, post_blanc, ai_blanc_form_subst, form_tag, blanc_form_subst_builder.build());
//        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "pos", "adj");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "number", "sg");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "gender", "m");
//        AnnotationItem ai_blanc_subst("blanc");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "base", "blanc");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "morphology", "subst:sg:m");
////        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "pos", "subst");
////        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "number", "sg");
////        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "gender", "m");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "discard", "0");
//        lattice.addEdge(pre_blanc, post_blanc, ai_blanc_subst, lemma_tag, blanc_lemma_builder.build());

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
        lattice.getAnnotationItemManager().setValue(ai_chat_form, "morpho", "sg:m");
        lattice.getAnnotationItemManager().setValue(ai_chat_form, "discard", "0");
        lattice.addEdge(pre_chat, post_chat, ai_chat_form, form_tag, chat_form_builder.build());
//        lattice.getAnnotationItemManager().setValue(ai_chat, "pos", "subst");
//        lattice.getAnnotationItemManager().setValue(ai_chat, "number", "sg");
//        lattice.getAnnotationItemManager().setValue(ai_chat, "gender", "m");
//        lattice.getAnnotationItemManager().setValue(ai_chat, "discard", "0");

        Lattice::EdgeDescriptor edge;
        Lattice::EdgesSortedBySourceIterator lemmaIter = lattice.edgesSortedBySource(lemmaMask);
        TS_ASSERT(lemmaIter.hasNext());
        edge = lemmaIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_lemma.getCategory()
        );
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getText(),
            ai_blanc_lemma.getText()
        );
        TS_ASSERT(lemmaIter.hasNext());
        edge = lemmaIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_chat_lemma.getCategory()
        );
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getText(),
            ai_chat_lemma.getText()
        );
        TS_ASSERT(!lemmaIter.hasNext());
        Lattice::EdgesSortedBySourceIterator lexemeIter = lattice.edgesSortedBySource(lexemeMask);
        TS_ASSERT(lexemeIter.hasNext());
        edge = lexemeIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_lexeme_adj.getCategory()
        );
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getText(),
            ai_blanc_lexeme_adj.getText()
        );
        TS_ASSERT(lexemeIter.hasNext());
        edge = lexemeIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_lexeme_subst.getCategory()
        );
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getText(),
            ai_blanc_lexeme_subst.getText()
        );
        TS_ASSERT(lexemeIter.hasNext());
        edge = lexemeIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_chat_lexeme.getCategory()
        );
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getText(),
            ai_chat_lexeme.getText()
        );
        TS_ASSERT(!lexemeIter.hasNext());
        Lattice::EdgesSortedBySourceIterator formIter = lattice.edgesSortedBySource(formMask);
        TS_ASSERT(formIter.hasNext());
        edge = formIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_form_adj.getCategory()
        );
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getText(),
            ai_blanc_form_adj.getText()
        );
        std::list< std::pair<std::string, std::string> > av
            = lattice.getAnnotationItemManager().getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "morpho");
        TS_ASSERT_EQUALS((*avi).second, "sg:m");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "discard");
        TS_ASSERT_EQUALS((*avi).second, "0");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(formIter.hasNext());
        edge = formIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_form_subst.getCategory()
        );
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getText(),
            ai_blanc_form_subst.getText()
        );
        av = lattice.getAnnotationItemManager().getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "morpho");
        TS_ASSERT_EQUALS((*avi).second, "sg:m");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "discard");
        TS_ASSERT_EQUALS((*avi).second, "0");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(formIter.hasNext());
        edge = formIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_chat_form.getCategory()
        );
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getText(),
            ai_chat_form.getText()
        );
        av = lattice.getAnnotationItemManager().getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "morpho");
        TS_ASSERT_EQUALS((*avi).second, "sg:m");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "discard");
        TS_ASSERT_EQUALS((*avi).second, "0");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(!formIter.hasNext());

        //preparing parser
        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules.fr";
        poleng::bonsai::puddle::TagsetLoader tagset_loader;
        poleng::bonsai::puddle::RuleLoader rule_loader;
//        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());

        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();

        poleng::bonsai::puddle::TagsetPtr tagset;
        tagset = tagset_loader.load(tagsetFilename);
//        tagger->setTagset(tagset);
        puddle->setTagset(tagset);
        rule_loader.setTagset(tagset);
//        puddle->setTagger(tagger);
        poleng::bonsai::puddle::RulesPtr rules =
            rule_loader.readFromFile(rulesFilename);//, puddle->getLatticeWrapper());
        puddle->setRules(rules);
        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);

        //parsing
        TS_ASSERT(puddle->parse(lattice));

        delete puddle;
    }

////    void testPuddleUnify() {
////        //preparing lattice
////        Lattice lattice("ananas noirs");
////        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
////        LayerTagCollection raw_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
////        LayerTagCollection token_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("token");
////        LayerTagCollection lemma_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
////        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
////        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
////        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
////
////        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
////        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
////        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
////        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
////
////        AnnotationItem word_token("word");
////        AnnotationItem blank_token("blank");
////
////        Lattice::EdgeSequence::Builder ananas_builder;
////        for (int i = 0; i < 6; i ++) {
////            ananas_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        ));
////        }
////        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
////
////        Lattice::EdgeSequence::Builder blank_builder;
////        blank_builder.addEdge(lattice.firstOutEdge(
////            lattice.getVertexForRawCharIndex(6),
////            rawMask
////        ));
////        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
////        Lattice::EdgeSequence::Builder noirs_builder;
////        for (int i = 7; i < 12; i ++) {
////            noirs_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        )
////                    );
////        }
////        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
////
////        Lattice::EdgeSequence::Builder ananas_lemma_builder;
////        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
////        AnnotationItem ai_ananas_sg("ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "base", "ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "morphology", "subst:sg:m");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg, lemma_tag, ananas_lemma_builder.build());
////        AnnotationItem ai_ananas_pl("ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "base", "ananas"); //@todo: tu problem, bo dla niego identyczne sa morphology jak sie zaczynaja od tej samej litery czy podciagu. nie moga byc wiec dwie interpretacje 'subst'. poki co radzimy se tak
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl, lemma_tag, ananas_lemma_builder.build());
////
////        Lattice::EdgeSequence::Builder noirs_lemma_builder;
////        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
////        AnnotationItem ai_noirs_adj("noirs");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "morphology", "adj:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj, lemma_tag, noirs_lemma_builder.build());
////        AnnotationItem ai_noirs_subst("noirs");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst, lemma_tag, noirs_lemma_builder.build());
////
////        //preparing parser
////        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
////        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_unify.fr";
////        poleng::bonsai::puddle::TagsetLoader tagset_loader;
////        poleng::bonsai::puddle::RuleLoader rule_loader;
////        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
////
////        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
////
////        poleng::bonsai::puddle::TagsetPtr tagset;
////        tagset = tagset_loader.load(tagsetFilename);
////        tagger->setTagset(tagset);
////        puddle->setTagset(tagset);
////        rule_loader.setTagset(tagset);
////        puddle->setTagger(tagger);
////        poleng::bonsai::puddle::RulesPtr rules =
////            rule_loader.readFromFile(rulesFilename);
////        puddle->setRules(rules);
////        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);
////
////        //parsing
////        TS_ASSERT(puddle->parse(lattice));
////
////        Lattice::EdgeDescriptor edge;
////        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(lemmaMask);
////        TS_ASSERT(tokenIter.hasNext());
////        edge = tokenIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            ai_ananas_sg.getCategory()
////        );
////        std::list< std::pair<std::string, std::string> > av
////            = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "1"); //deleted
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(tokenIter.hasNext());
////        edge = tokenIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            ai_ananas_pl.getCategory()
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas"); //@todo: poprawic to
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(tokenIter.hasNext());
////        edge = tokenIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            ai_noirs_adj.getCategory()
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "noir");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "adj:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(tokenIter.hasNext());
////        edge = tokenIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            ai_noirs_subst.getCategory()
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "noir");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(!tokenIter.hasNext());
////
////        delete puddle
////    }
//
////    void testPuddleDelete() {
////        //preparing lattice
////        Lattice lattice("ananas noirs");
////        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
////        LayerTagCollection raw_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
////        LayerTagCollection token_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("token");
////        LayerTagCollection lemma_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
////        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
////        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
////        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
////
////        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
////        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
////        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
////        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
////
////        AnnotationItem word_token("word");
////        AnnotationItem blank_token("blank");
////
////        Lattice::EdgeSequence::Builder ananas_builder;
////        for (int i = 0; i < 6; i ++) {
////            ananas_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        ));
////        }
////        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
////
////        Lattice::EdgeSequence::Builder blank_builder;
////        blank_builder.addEdge(lattice.firstOutEdge(
////            lattice.getVertexForRawCharIndex(6),
////            rawMask
////        ));
////        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
////        Lattice::EdgeSequence::Builder noirs_builder;
////        for (int i = 7; i < 12; i ++) {
////            noirs_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        )
////                    );
////        }
////        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
////
////        Lattice::EdgeSequence::Builder ananas_lemma_builder;
////        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
////        AnnotationItem ai_ananas_sg("ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "base", "ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "morphology", "subst:sg:m");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg, lemma_tag, ananas_lemma_builder.build());
////        AnnotationItem ai_ananas_pl("ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "base", "ananas"); //@todo: tu problem, bo dla niego identyczne sa morphology jak sie zaczynaja od tej samej litery czy podciagu. nie moga byc wiec dwie interpretacje 'subst'. poki co radzimy se tak
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl, lemma_tag, ananas_lemma_builder.build());
////
////        Lattice::EdgeSequence::Builder noirs_lemma_builder;
////        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
////        AnnotationItem ai_noirs_adj("noirs");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "morphology", "adj:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj, lemma_tag, noirs_lemma_builder.build());
////        AnnotationItem ai_noirs_subst("noirs");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst, lemma_tag, noirs_lemma_builder.build());
////
////        //preparing parser
////        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
////        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_delete.fr";
////        poleng::bonsai::puddle::TagsetLoader tagset_loader;
////        poleng::bonsai::puddle::RuleLoader rule_loader;
////        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
////
////        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
////
////        poleng::bonsai::puddle::TagsetPtr tagset;
////        tagset = tagset_loader.load(tagsetFilename);
////        tagger->setTagset(tagset);
////        puddle->setTagset(tagset);
////        rule_loader.setTagset(tagset);
////        puddle->setTagger(tagger);
////        poleng::bonsai::puddle::RulesPtr rules =
////            rule_loader.readFromFile(rulesFilename);
////        puddle->setRules(rules);
////        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);
////
////        //parsing
////        TS_ASSERT(puddle->parse(lattice));
////
////        Lattice::EdgeDescriptor edge;
////        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(lemmaMask);
////        TS_ASSERT(tokenIter.hasNext());
////        edge = tokenIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            ai_ananas_sg.getCategory()
////        );
////        std::list< std::pair<std::string, std::string> > av
////            = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "1"); //deleted
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(tokenIter.hasNext());
////        edge = tokenIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            ai_ananas_pl.getCategory()
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas"); //@todo: poprawic to
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(tokenIter.hasNext());
////        edge = tokenIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            ai_noirs_adj.getCategory()
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "noir");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "adj:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(tokenIter.hasNext());
////        edge = tokenIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            ai_noirs_subst.getCategory()
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "noir");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "1"); //deleted
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(!tokenIter.hasNext());
////
////        delete puddle;
////    }
//
//    void testPuddleAdd() {
//        //preparing lattice
//        Lattice lattice("ananas noirs");
//        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
//        LayerTagCollection raw_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
//        LayerTagCollection token_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("token");
//        LayerTagCollection lemma_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
//        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
//        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
//        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
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
//        AnnotationItem ai_ananas_sg("ananas");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "base", "ananas");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "morphology", "subst:sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg, lemma_tag, ananas_lemma_builder.build());
//        AnnotationItem ai_ananas_pl("ananas");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "base", "ananas"); //@todo: tu problem, bo dla niego identyczne sa morphology jak sie zaczynaja od tej samej litery czy podciagu. nie moga byc wiec dwie interpretacje 'subst'. poki co radzimy se tak
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "morphology", "subst:pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl, lemma_tag, ananas_lemma_builder.build());
//
//        Lattice::EdgeSequence::Builder noirs_lemma_builder;
//        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
//        AnnotationItem ai_noirs_adj("noirs");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "base", "noir");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "morphology", "adj:pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj, lemma_tag, noirs_lemma_builder.build());
//        AnnotationItem ai_noirs_subst("noirs");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "base", "noir");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "morphology", "subst:pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst, lemma_tag, noirs_lemma_builder.build());
//
//        //preparing parser
//        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
//        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_add.fr";
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
//        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);
//
//        //parsing
//        TS_ASSERT(puddle->parse(lattice));
//
//        Lattice::EdgeDescriptor edge;
//        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(lemmaMask);
//        TS_ASSERT(tokenIter.hasNext());
//        edge = tokenIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_ananas_sg.getCategory()
//        );
//        std::list< std::pair<std::string, std::string> > av
//            = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "ananas");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(tokenIter.hasNext());
//        edge = tokenIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_ananas_pl.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "ananas");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(tokenIter.hasNext());
//        edge = tokenIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_ananas_pl.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "ananas");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "adj:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(tokenIter.hasNext());
//        edge = tokenIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_ananas_pl.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "ananas");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "adj:pl:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(tokenIter.hasNext());
//        edge = tokenIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_noirs_adj.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "noir");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "adj:pl:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(tokenIter.hasNext());
//        edge = tokenIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            ai_noirs_subst.getCategory()
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "noir");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(!tokenIter.hasNext());
//
//        delete puddle;
//    }
//
//    void testPuddleGroup() {
//        //preparing lattice
//        Lattice lattice("ananas noirs");
//        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
//        LayerTagCollection raw_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
//        LayerTagCollection token_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("token");
//        LayerTagCollection lemma_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
//        LayerTagCollection parse_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("parse");
//        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
//        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
//        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
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
//        AnnotationItem ai_ananas_sg("ananas");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "base", "ananas");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "morphology", "subst:sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg, lemma_tag, ananas_lemma_builder.build());
//        AnnotationItem ai_ananas_pl("ananas");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "base", "ananas");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "morphology", "subst:pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "discard", "0");
//        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl, lemma_tag, ananas_lemma_builder.build());
//
//        Lattice::EdgeSequence::Builder noirs_lemma_builder;
//        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
//        AnnotationItem ai_noirs_adj("noirs");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "base", "noir");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "morphology", "adj:pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj, lemma_tag, noirs_lemma_builder.build());
//        AnnotationItem ai_noirs_subst("noirs");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "base", "noir");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "morphology", "subst:pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_noirs_subst, "discard", "0");
//        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_subst, lemma_tag, noirs_lemma_builder.build());
//
//        //preparing parser
//        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
//        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_group.fr";
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
//        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);
//
//        //parsing
//        TS_ASSERT(puddle->parse(lattice));
//
//        Lattice::EdgeDescriptor edge;
//        Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(parseMask);
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "NP"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            12
//        );
//        std::list< std::pair<std::string, std::string> > av
//            = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "ananas");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "orth");
//        TS_ASSERT_EQUALS((*avi).second, "ananas");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "NP"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            12
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "ananas");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "orth");
//        TS_ASSERT_EQUALS((*avi).second, "ananas");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "subst"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            6
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT( avi == av.end() );
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "adj"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            7
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            5
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT( avi == av.end() );
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "subst"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            7
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            5
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                lattice.getEdgeAnnotationItem(edge)
//                );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT( avi == av.end() );
//        TS_ASSERT(!parseIter.hasNext());
//
//        delete puddle;
//    }
//
////    void testPuddleJoin() {
////        //preparing lattice
////        Lattice lattice("ananas noirs");
////        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
////        LayerTagCollection raw_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
////        LayerTagCollection token_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("token");
////        LayerTagCollection lemma_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
////        LayerTagCollection parse_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("parse");
////        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
////        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
////        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
////        LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
////
////        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
////        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
////        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
////        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
////
////        AnnotationItem word_token("word");
////        AnnotationItem blank_token("blank");
////
////        Lattice::EdgeSequence::Builder ananas_builder;
////        for (int i = 0; i < 6; i ++) {
////            ananas_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        ));
////        }
////        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
////
////        Lattice::EdgeSequence::Builder blank_builder;
////        blank_builder.addEdge(lattice.firstOutEdge(
////            lattice.getVertexForRawCharIndex(6),
////            rawMask
////        ));
////        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
////        Lattice::EdgeSequence::Builder noirs_builder;
////        for (int i = 7; i < 12; i ++) {
////            noirs_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        )
////                    );
////        }
////        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
////
////        Lattice::EdgeSequence::Builder ananas_lemma_builder;
////        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
////        AnnotationItem ai_ananas_pl("ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "base", "ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl, lemma_tag, ananas_lemma_builder.build());
////
////        Lattice::EdgeSequence::Builder noirs_lemma_builder;
////        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
////        AnnotationItem ai_noirs_adj("noirs");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "morphology", "adj:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj, lemma_tag, noirs_lemma_builder.build());
////
////        Lattice::EdgeSequence::Builder np_builder;
////        np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
////        AnnotationItem ai_np("NP");
////        lattice.getAnnotationItemManager().setValue(ai_np, "base", "ananas");
////        lattice.getAnnotationItemManager().setValue(ai_np, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_np, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());
////
////        Lattice::EdgeSequence::Builder ap_builder;
////        np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
////        AnnotationItem ai_ap("AP");
////        lattice.getAnnotationItemManager().setValue(ai_ap, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_ap, "morphology", "adj:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_ap, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_ap, parse_tag, ap_builder.build());
////
////        //preparing parser
////        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
////        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_join.fr";
////        poleng::bonsai::puddle::TagsetLoader tagset_loader;
////        poleng::bonsai::puddle::RuleLoader rule_loader;
////        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
////
////        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
////
////        poleng::bonsai::puddle::TagsetPtr tagset;
////        tagset = tagset_loader.load(tagsetFilename);
////        tagger->setTagset(tagset);
////        puddle->setTagset(tagset);
////        rule_loader.setTagset(tagset);
////        puddle->setTagger(tagger);
////        poleng::bonsai::puddle::RulesPtr rules =
////            rule_loader.readFromFile(rulesFilename);
////        puddle->setRules(rules);
////        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);
////
////        //parsing
////        TS_ASSERT(puddle->parse(lattice));
////
////        Lattice::EdgeDescriptor edge;
////        Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(parseMask);
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "NP"
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeBeginIndex(edge),
////            0
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeLength(edge),
////            6
////        );
////        std::list< std::pair<std::string, std::string> > av
////            = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "1");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "orth");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "NP"
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeBeginIndex(edge),
////            0
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeLength(edge),
////            12
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "orth");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "subst"
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT( avi == av.end() );
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "AP"
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeBeginIndex(edge),
////            7
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeLength(edge),
////            5
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "noir");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "adj:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "1");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "orth");
////        TS_ASSERT_EQUALS((*avi).second, "noirs");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "adj"
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT( avi == av.end() );
////        TS_ASSERT(!parseIter.hasNext());
////
////        delete puddle;
////    }
//
////    void testPuddleAttach() {
////        //preparing lattice
////        Lattice lattice("ananas noirs");
////        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
////        LayerTagCollection raw_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
////        LayerTagCollection token_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("token");
////        LayerTagCollection lemma_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
////        LayerTagCollection parse_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("parse");
////        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
////        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
////        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
////        LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
////
////        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
////        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
////        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
////        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
////
////        AnnotationItem word_token("word");
////        AnnotationItem blank_token("blank");
////
////        Lattice::EdgeSequence::Builder ananas_builder;
////        for (int i = 0; i < 6; i ++) {
////            ananas_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        ));
////        }
////        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
////
////        Lattice::EdgeSequence::Builder blank_builder;
////        blank_builder.addEdge(lattice.firstOutEdge(
////            lattice.getVertexForRawCharIndex(6),
////            rawMask
////        ));
////        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
////        Lattice::EdgeSequence::Builder noirs_builder;
////        for (int i = 7; i < 12; i ++) {
////            noirs_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        )
////                    );
////        }
////        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
////
////        Lattice::EdgeSequence::Builder ananas_lemma_builder;
////        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
////        AnnotationItem ai_ananas_pl("ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "base", "ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl, lemma_tag, ananas_lemma_builder.build());
////
////        Lattice::EdgeSequence::Builder noirs_lemma_builder;
////        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
////        AnnotationItem ai_noirs_adj("noirs");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "morphology", "adj:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj, lemma_tag, noirs_lemma_builder.build());
////
////        Lattice::EdgeSequence::Builder np_builder;
////        np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
////        AnnotationItem ai_np("NP");
////        lattice.getAnnotationItemManager().setValue(ai_np, "base", "ananas");
////        lattice.getAnnotationItemManager().setValue(ai_np, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_np, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());
////
////        Lattice::EdgeSequence::Builder ap_builder;
////        np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), lemmaMask));
////        AnnotationItem ai_ap("AP");
////        lattice.getAnnotationItemManager().setValue(ai_ap, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_ap, "morphology", "adj:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_ap, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_ap, parse_tag, ap_builder.build());
////
////        //preparing parser
////        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
////        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_attach.fr";
////        poleng::bonsai::puddle::TagsetLoader tagset_loader;
////        poleng::bonsai::puddle::RuleLoader rule_loader;
////        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
////
////        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
////
////        poleng::bonsai::puddle::TagsetPtr tagset;
////        tagset = tagset_loader.load(tagsetFilename);
////        tagger->setTagset(tagset);
////        puddle->setTagset(tagset);
////        rule_loader.setTagset(tagset);
////        puddle->setTagger(tagger);
////        poleng::bonsai::puddle::RulesPtr rules =
////            rule_loader.readFromFile(rulesFilename);
////        puddle->setRules(rules);
////        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);
////
////        //parsing
////        TS_ASSERT(puddle->parse(lattice));
////
////        Lattice::EdgeDescriptor edge;
////        Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(parseMask);
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "NP"
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeBeginIndex(edge),
////            0
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeLength(edge),
////            6
////        );
////        std::list< std::pair<std::string, std::string> > av
////            = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "1");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "orth");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "NP"
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeBeginIndex(edge),
////            0
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeLength(edge),
////            12
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "orth");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "subst"
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT( avi == av.end() );
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "AP"
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeBeginIndex(edge),
////            7
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeLength(edge),
////            5
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "noir");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "adj:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "orth");
////        TS_ASSERT_EQUALS((*avi).second, "noirs");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "adj"
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT( avi == av.end() );
////        TS_ASSERT(!parseIter.hasNext());
////
////        delete puddle;
////    }
//
////    void testPuddleTransform() {
////        //preparing lattice
////        Lattice lattice("ananas noirs");
////        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
////        LayerTagCollection raw_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
////        LayerTagCollection token_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("token");
////        LayerTagCollection lemma_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
////        LayerTagCollection parse_tag
////            = lattice.getLayerTagManager().createSingletonTagCollection("parse");
////        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
////        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
////        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
////        LayerTagMask parseMask = lattice.getLayerTagManager().getMask(parse_tag);
////
////        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
////        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);
////        Lattice::VertexDescriptor pre_noirs = lattice.getVertexForRawCharIndex(7);
////        Lattice::VertexDescriptor post_noirs = lattice.getLastVertex();
////
////        AnnotationItem word_token("word");
////        AnnotationItem blank_token("blank");
////
////        Lattice::EdgeSequence::Builder ananas_builder;
////        for (int i = 0; i < 6; i ++) {
////            ananas_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        ));
////        }
////        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
////
////        Lattice::EdgeSequence::Builder blank_builder;
////        blank_builder.addEdge(lattice.firstOutEdge(
////            lattice.getVertexForRawCharIndex(6),
////            rawMask
////        ));
////        lattice.addEdge(post_ananas, pre_noirs, blank_token, token_tag, blank_builder.build());
////        Lattice::EdgeSequence::Builder noirs_builder;
////        for (int i = 7; i < 12; i ++) {
////            noirs_builder.addEdge(lattice.firstOutEdge(
////                        lattice.getVertexForRawCharIndex(i),
////                        rawMask
////                        )
////                    );
////        }
////        lattice.addEdge(pre_noirs, post_noirs, word_token, token_tag, noirs_builder.build());
////
////        Lattice::EdgeSequence::Builder ananas_lemma_builder;
////        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
////        AnnotationItem ai_ananas_pl("ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "base", "ananas");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl, lemma_tag, ananas_lemma_builder.build());
////
////        Lattice::EdgeSequence::Builder noirs_lemma_builder;
////        noirs_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
////        AnnotationItem ai_noirs_adj("noirs");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "base", "noir");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "morphology", "adj:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_noirs_adj, "discard", "0");
////        lattice.addEdge(pre_noirs, post_noirs, ai_noirs_adj, lemma_tag, noirs_lemma_builder.build());
////
////        Lattice::EdgeSequence::Builder np_builder;
////        np_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), lemmaMask));
////        AnnotationItem ai_np("NP");
////        lattice.getAnnotationItemManager().setValue(ai_np, "base", "ananas");
////        lattice.getAnnotationItemManager().setValue(ai_np, "morphology", "subst:pl:m");
////        lattice.getAnnotationItemManager().setValue(ai_np, "discard", "0");
////        lattice.addEdge(pre_ananas, post_ananas, ai_np, parse_tag, np_builder.build());
////
////        //preparing parser
////        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
////        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_transform.fr";
////        poleng::bonsai::puddle::TagsetLoader tagset_loader;
////        poleng::bonsai::puddle::RuleLoader rule_loader;
////        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());
////
////        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();
////
////        poleng::bonsai::puddle::TagsetPtr tagset;
////        tagset = tagset_loader.load(tagsetFilename);
////        tagger->setTagset(tagset);
////        puddle->setTagset(tagset);
////        rule_loader.setTagset(tagset);
////        puddle->setTagger(tagger);
////        poleng::bonsai::puddle::RulesPtr rules =
////            rule_loader.readFromFile(rulesFilename);
////        puddle->setRules(rules);
////        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);
////
////        //parsing
////        TS_ASSERT(puddle->parse(lattice));
////
////        Lattice::EdgeDescriptor edge;
////        Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(parseMask);
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "NP"
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeBeginIndex(edge),
////            0
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeLength(edge),
////            6
////        );
////        std::list< std::pair<std::string, std::string> > av
////            = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "1");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "orth");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "XP"
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeBeginIndex(edge),
////            0
////        );
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeLength(edge),
////            6
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "base");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT_EQUALS((*avi).first, "morphology");
////        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT(avi != av.end());
////        TS_ASSERT_EQUALS((*avi).first, "orth");
////        TS_ASSERT_EQUALS((*avi).second, "ananas");
////        ++avi;
////        TS_ASSERT(avi == av.end());
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "subst"
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT( avi == av.end() );
////        TS_ASSERT(parseIter.hasNext());
////        edge = parseIter.next();
////        TS_ASSERT_EQUALS(
////            lattice.getEdgeAnnotationItem(edge).getCategory(),
////            "adj"
////        );
////        av = lattice.getAnnotationItemManager().getValues(
////                    lattice.getEdgeAnnotationItem(edge)
////                    );
////        avi = av.begin();
////        TS_ASSERT_EQUALS((*avi).first, "discard");
////        TS_ASSERT_EQUALS((*avi).second, "0");
////        ++avi;
////        TS_ASSERT( avi == av.end() );
////        TS_ASSERT(!parseIter.hasNext());
////
////        delete puddle;
////    }
//
//    void testPuddleSyntok() {
//        //preparing lattice
//        Lattice lattice("coup d'état");
//        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
//        LayerTagCollection raw_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
//        LayerTagCollection token_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("token");
//        LayerTagCollection lemma_tag
//            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
//        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
//        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
//        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
//
//        Lattice::VertexDescriptor pre_coup = lattice.getFirstVertex();
//        Lattice::VertexDescriptor post_coup = lattice.getVertexForRawCharIndex(4);
//        Lattice::VertexDescriptor pre_d = lattice.getVertexForRawCharIndex(5);
//        Lattice::VertexDescriptor post_d = lattice.getVertexForRawCharIndex(7);
//        Lattice::VertexDescriptor post_etat = lattice.getLastVertex();
//
//        AnnotationItem word_token("word");
//        AnnotationItem blank_token("blank");
//
//        Lattice::EdgeSequence::Builder coup_builder;
//        for (int i = 0; i < 4; i ++) {
//            coup_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        ));
//        }
//        lattice.addEdge(pre_coup, post_coup, word_token, token_tag, coup_builder.build());
//
//        Lattice::EdgeSequence::Builder blank_builder;
//        blank_builder.addEdge(lattice.firstOutEdge(
//            lattice.getVertexForRawCharIndex(4),
//            rawMask
//        ));
//        lattice.addEdge(post_coup, pre_d, blank_token, token_tag, blank_builder.build());
//        Lattice::EdgeSequence::Builder d_builder;
//        for (int i = 5; i < 7; i ++) {
//            d_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        )
//                    );
//        }
//        lattice.addEdge(pre_d, post_d, word_token, token_tag, d_builder.build());
//        Lattice::EdgeSequence::Builder etat_builder;
//        for (int i = 7; i < 12; i ++) {
//            if (i == 8)
//                continue;
//            etat_builder.addEdge(lattice.firstOutEdge(
//                        lattice.getVertexForRawCharIndex(i),
//                        rawMask
//                        )
//                    );
//        }
//        lattice.addEdge(post_d, post_etat, word_token, token_tag, etat_builder.build());
//
//        Lattice::EdgeSequence::Builder coup_lemma_builder;
//        coup_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
//        AnnotationItem ai_coup("coup");
//        lattice.getAnnotationItemManager().setValue(ai_coup, "base", "coup");
//        lattice.getAnnotationItemManager().setValue(ai_coup, "morphology", "subst:sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_coup, "discard", "0");
//        lattice.addEdge(pre_coup, post_coup, ai_coup, lemma_tag, coup_lemma_builder.build());
//
//        Lattice::EdgeSequence::Builder d_lemma_builder;
//        d_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(5), tokenMask));
//        AnnotationItem ai_d_prep("d'");
//        lattice.getAnnotationItemManager().setValue(ai_d_prep, "base", "d'");
//        lattice.getAnnotationItemManager().setValue(ai_d_prep, "morphology", "prep");
//        lattice.getAnnotationItemManager().setValue(ai_d_prep, "discard", "0");
//        lattice.addEdge(pre_d, post_d, ai_d_prep, lemma_tag, d_lemma_builder.build());
//        AnnotationItem ai_d_subst_pl("d'");
//        lattice.getAnnotationItemManager().setValue(ai_d_subst_pl, "base", "d");
//        lattice.getAnnotationItemManager().setValue(ai_d_subst_pl, "morphology", "subst:pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_d_subst_pl, "discard", "0");
//        lattice.addEdge(pre_d, post_d, ai_d_subst_pl, lemma_tag, d_lemma_builder.build());
//        AnnotationItem ai_d_subst_sg("d'");
//        lattice.getAnnotationItemManager().setValue(ai_d_subst_sg, "base", "d");
//        lattice.getAnnotationItemManager().setValue(ai_d_subst_sg, "morphology", "subst:sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_d_subst_sg, "discard", "0");
//        lattice.addEdge(pre_d, post_d, ai_d_subst_sg, lemma_tag, d_lemma_builder.build());
//        AnnotationItem ai_d_det_m("d'");
//        lattice.getAnnotationItemManager().setValue(ai_d_det_m, "base", "de");
//        lattice.getAnnotationItemManager().setValue(ai_d_det_m, "morphology", "det:pl:m");
//        lattice.getAnnotationItemManager().setValue(ai_d_det_m, "discard", "0");
//        lattice.addEdge(pre_d, post_d, ai_d_det_m, lemma_tag, d_lemma_builder.build());
//        AnnotationItem ai_d_det_f("d'");
//        lattice.getAnnotationItemManager().setValue(ai_d_det_f, "base", "de");
//        lattice.getAnnotationItemManager().setValue(ai_d_det_f, "morphology", "det:pl:f");
//        lattice.getAnnotationItemManager().setValue(ai_d_det_f, "discard", "0");
//        lattice.addEdge(pre_d, post_d, ai_d_det_f, lemma_tag, d_lemma_builder.build());
//
//        Lattice::EdgeSequence::Builder etat_lemma_builder;
//        etat_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), tokenMask));
//        AnnotationItem ai_etat("état");
//        lattice.getAnnotationItemManager().setValue(ai_etat, "base", "état");
//        lattice.getAnnotationItemManager().setValue(ai_etat, "morphology", "subst:sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_etat, "discard", "0");
//        lattice.addEdge(post_d, post_etat, ai_etat, lemma_tag, etat_lemma_builder.build());
//
//        //preparing parser
//        std::string tagsetFilename = ROOT_DIR "tools/parsers/puddle/t/files/tagset.fr";
//        std::string rulesFilename = ROOT_DIR "tools/parsers/puddle/t/files/rules_syntok.fr";
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
//        TS_ASSERT_EQUALS(rules->size(), (size_t) 1);
//
//        //parsing
//        TS_ASSERT(puddle->parse(lattice));
//
//        Lattice::EdgeDescriptor edge;
//        Lattice::EdgesSortedBySourceIterator parseIter = lattice.edgesSortedBySource(lemmaMask);
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "coup"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            4
//        );
//        std::list< std::pair<std::string, std::string> > av
//            = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "coup");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "coup d'état"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            12
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "coup d'état");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "coup d'état"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            12
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "coup détat");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "coup d'état"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            0
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            12
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "coup deétat");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "d'"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            5
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            2
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "d'");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "prep");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "d'"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            5
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            2
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "d");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "d'"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            5
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            2
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "d");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "d'"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            5
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            2
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "de");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "det:pl:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "d'"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            5
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            2
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "de");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "det:pl:f");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(parseIter.hasNext());
//        edge = parseIter.next();
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeAnnotationItem(edge).getCategory(),
//            "état"
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeBeginIndex(edge),
//            7
//        );
//        TS_ASSERT_EQUALS(
//            lattice.getEdgeLength(edge),
//            5
//        );
//        av = lattice.getAnnotationItemManager().getValues(
//                    lattice.getEdgeAnnotationItem(edge)
//                    );
//        avi = av.begin();
//        TS_ASSERT_EQUALS((*avi).first, "base");
//        TS_ASSERT_EQUALS((*avi).second, "état");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "morphology");
//        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "discard");
//        TS_ASSERT_EQUALS((*avi).second, "0");
//        ++avi;
//        TS_ASSERT(avi == av.end());
//        TS_ASSERT(!parseIter.hasNext());
//
//        delete puddle;
//    }

};

