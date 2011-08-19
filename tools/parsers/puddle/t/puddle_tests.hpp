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
        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
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
        AnnotationItem ai_blanc_adj("blanc");
        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "base", "blanc");
        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "morphology", "adj:sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "pos", "adj");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "number", "sg");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "gender", "m");
        lattice.getAnnotationItemManager().setValue(ai_blanc_adj, "discard", "0");
        lattice.addEdge(pre_blanc, post_blanc, ai_blanc_adj, lemma_tag, blanc_lemma_builder.build());
        AnnotationItem ai_blanc_subst("blanc");
        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "base", "blanc");
        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "morphology", "subst:sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "pos", "subst");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "number", "sg");
//        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "gender", "m");
        lattice.getAnnotationItemManager().setValue(ai_blanc_subst, "discard", "0");
        lattice.addEdge(pre_blanc, post_blanc, ai_blanc_subst, lemma_tag, blanc_lemma_builder.build());

        Lattice::EdgeSequence::Builder chat_lemma_builder;
        chat_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), tokenMask));
        AnnotationItem ai_chat("chat");
        lattice.getAnnotationItemManager().setValue(ai_chat, "base", "chat");
        lattice.getAnnotationItemManager().setValue(ai_chat, "morphology", "subst:sg:m");
//        lattice.getAnnotationItemManager().setValue(ai_chat, "pos", "subst");
//        lattice.getAnnotationItemManager().setValue(ai_chat, "number", "sg");
//        lattice.getAnnotationItemManager().setValue(ai_chat, "gender", "m");
        lattice.getAnnotationItemManager().setValue(ai_chat, "discard", "0");
        lattice.addEdge(pre_chat, post_chat, ai_chat, lemma_tag, chat_lemma_builder.build());

        Lattice::EdgeDescriptor edge;
        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(lemmaMask);
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_adj.getCategory()
        );
        std::list< std::pair<std::string, std::string> > av
            = lattice.getAnnotationItemManager().getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "base");
        TS_ASSERT_EQUALS((*avi).second, "blanc");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "morphology");
        TS_ASSERT_EQUALS((*avi).second, "adj:sg:m");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "pos");
//        TS_ASSERT_EQUALS((*avi).second, "adj");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "number");
//        TS_ASSERT_EQUALS((*avi).second, "sg");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "gender");
//        TS_ASSERT_EQUALS((*avi).second, "m");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "discard");
        TS_ASSERT_EQUALS((*avi).second, "0");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_subst.getCategory()
        );
        av = lattice.getAnnotationItemManager().getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "base");
        TS_ASSERT_EQUALS((*avi).second, "blanc");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "morphology");
        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "pos");
//        TS_ASSERT_EQUALS((*avi).second, "subst");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "number");
//        TS_ASSERT_EQUALS((*avi).second, "sg");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "gender");
//        TS_ASSERT_EQUALS((*avi).second, "m");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "discard");
        TS_ASSERT_EQUALS((*avi).second, "0");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_chat.getCategory()
        );
        av = lattice.getAnnotationItemManager().getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "base");
        TS_ASSERT_EQUALS((*avi).second, "chat");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "morphology");
        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "pos");
//        TS_ASSERT_EQUALS((*avi).second, "subst");
//        ++avi;
//        TS_ASSERT_EQUALS((*avi).first, "number");
//        TS_ASSERT_EQUALS((*avi).second, "sg");
//        ++avi;
//        TS_ASSERT(avi != av.end());
//        TS_ASSERT_EQUALS((*avi).first, "gender");
//        TS_ASSERT_EQUALS((*avi).second, "m");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "discard");
        TS_ASSERT_EQUALS((*avi).second, "0");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(!tokenIter.hasNext());

        //preparing parser
        std::string tagsetFilename = "tools/parsers/puddle/t/files/tagset.fr";
        std::string rulesFilename = "tools/parsers/puddle/t/files/rules.fr";
        poleng::bonsai::puddle::TagsetLoader tagset_loader;
        poleng::bonsai::puddle::RuleLoader rule_loader;
        poleng::bonsai::puddle::TaggerPtr tagger = poleng::bonsai::puddle::TaggerPtr( new poleng::bonsai::puddle::Tagger());

        poleng::bonsai::puddle::Puddle *puddle = new poleng::bonsai::puddle::Puddle();

        poleng::bonsai::puddle::TagsetPtr tagset;
        tagset = tagset_loader.load(tagsetFilename);
        tagger->setTagset(tagset);
        puddle->setTagset(tagset);
        rule_loader.setTagset(tagset);
        puddle->setTagger(tagger);
        poleng::bonsai::puddle::RulesPtr rules =
            rule_loader.readFromFile(rulesFilename);//, puddle->getLatticeWrapper());
        puddle->setRules(rules);
        TS_ASSERT_EQUALS(rules->size(), 1);

        //parsing
        TS_ASSERT(puddle->parse(lattice));
    }
};

