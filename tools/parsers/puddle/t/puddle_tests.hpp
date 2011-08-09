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
        AnnotationItemManager aim;
        AnnotationItem ai("base");
        aim.setValue(ai, "base", "xxx");
        aim.setValue(ai, "number", "xxx");
        aim.setValue(ai, "gender", "xxx");

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

        Lattice::Partition blanc_partition;
        for (int i = 0; i < 5; i ++) {
            blanc_partition.links.push_back(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }
        lattice.addEdge(pre_blanc, post_blanc, word_token, token_tag, 0, blanc_partition);
        Lattice::Partition blank_partition;
        blank_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(6),
            rawMask
        ));
        lattice.addEdge(post_blanc, pre_chat, blank_token, token_tag, 0, blank_partition);
        Lattice::Partition chat_partition;
        for (int i = 6; i < 10; i ++) {
            chat_partition.links.push_back(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }
        lattice.addEdge(pre_chat, post_chat, word_token, token_tag, 0, chat_partition);

        Lattice::Partition blanc_lemma_partition;
        blanc_lemma_partition.links.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
        AnnotationItem ai_blanc_adj("adj");
        aim.setValue(ai_blanc_adj, "base", "blanc");
        aim.setValue(ai_blanc_adj, "number", "sg");
        aim.setValue(ai_blanc_adj, "gender", "m");
        lattice.addEdge(pre_blanc, post_blanc, ai_blanc_adj, lemma_tag, 0, blanc_lemma_partition);
        AnnotationItem ai_blanc_subst("subst");
        aim.setValue(ai_blanc_subst, "base", "blanc");
        aim.setValue(ai_blanc_subst, "number", "sg");
        aim.setValue(ai_blanc_subst, "gender", "m");
        lattice.addEdge(pre_blanc, post_blanc, ai_blanc_subst, lemma_tag, 0, blanc_lemma_partition);

        Lattice::Partition chat_lemma_partition;
        chat_lemma_partition.links.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), tokenMask));
        AnnotationItem ai_chat("subst");
        aim.setValue(ai_chat, "base", "chat");
        aim.setValue(ai_chat, "number", "sg");
        aim.setValue(ai_chat, "gender", "m");
        lattice.addEdge(pre_chat, post_chat, ai_chat, lemma_tag, 0, chat_lemma_partition);

        Lattice::EdgeDescriptor edge;
        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(lemmaMask);
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_adj.getCategory()
        );
        std::list< std::pair<std::string, std::string> > av
            = aim.getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "base");
        TS_ASSERT_EQUALS((*avi).second, "blanc");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "number");
        TS_ASSERT_EQUALS((*avi).second, "sg");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "gender");
        TS_ASSERT_EQUALS((*avi).second, "m");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_blanc_subst.getCategory()
        );
        av = aim.getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "base");
        TS_ASSERT_EQUALS((*avi).second, "blanc");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "number");
        TS_ASSERT_EQUALS((*avi).second, "sg");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "gender");
        TS_ASSERT_EQUALS((*avi).second, "m");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_chat.getCategory()
        );
        av = aim.getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "base");
        TS_ASSERT_EQUALS((*avi).second, "chat");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "number");
        TS_ASSERT_EQUALS((*avi).second, "sg");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "gender");
        TS_ASSERT_EQUALS((*avi).second, "m");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(!tokenIter.hasNext());

        //preparing parser
        std::string tagsetFilename = "../tools/parsers/puddle/t/files/tagset.fr";
        std::string rulesFilename = "../tools/parsers/puddle/t/files/rules.fr";
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
        poleng::bonsai::puddle::RulesPtr rules = rule_loader.readFromFile(rulesFilename);
        puddle->setRules(rules);
        TS_ASSERT_EQUALS(rules->size(), 3);

        //parsing
        TS_ASSERT(puddle->parse(lattice));
    }
};

