#include "../morfologik.hpp"
#include "../../common/t/by_spaces_cutter.hpp"

#include "lemmatizer_annotator.hpp"

class MorfologikTests : public CxxTest::TestSuite {

public:
	void testSimpleStem() {
		Morfologik morf;

		std::vector<std::string> stems = morf.simpleStem("dziecku");
		TS_ASSERT_EQUALS((int)stems.size(), 1);

		std::vector<std::string>::iterator s = stems.begin();
		TS_ASSERT_EQUALS(*s, "dziecko");
	}

	void testUnrecognizedWord() {
		Morfologik morf;

		std::vector<std::string> stems = morf.simpleStem("Dziecko");	
		TS_ASSERT_EQUALS((int)stems.size(), 0);
	}

	void testStems() {
		Morfologik morf;

		std::multimap<std::string, std::vector<std::string> > stems = 
			morf.stem("jeden");
		TS_ASSERT_EQUALS((int)stems.size(), 2);

		std::multimap<std::string, std::vector<std::string> >::iterator i =
			stems.begin();

		std::string s1 = i->first;
		std::vector<std::string> v1 = i->second;
		i++;
		std::string s2 = i->first;
		std::vector<std::string> v2 = i->second;

		TS_ASSERT_EQUALS(s1, s2);
		TS_ASSERT_DIFFERS((int)v1.size(), (int)v2.size());
		TS_ASSERT_DIFFERS(v1, v2);
	}

	void testLexemeLevel() {
	
		Lattice lattice;
        lattice.appendStringWithSymbols("prowokacjami");

        BySpacesCutter cutter;
        LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");

        lattice.runCutter(cutter, symbolMask);

        LemmatizerAnnotator<Morfologik> annotator;
        annotator.annotate(lattice);

        // now checking
        {
            LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lexeme");
            Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

            TS_ASSERT(lemmaIter.hasNext());

            Lattice::EdgeDescriptor prowokacjamiLemma = lemmaIter.next();
            AnnotationItem prowokacjamiItem = lattice.getEdgeAnnotationItem(prowokacjamiLemma);

            TS_ASSERT_EQUALS(prowokacjamiItem.getCategory(), "subst");
            TS_ASSERT_EQUALS(prowokacjamiItem.getText(), "prowokacja_subst");

//			AnnotationItemManager annItmMng = lattice.getAnnotationItemManager();

            TS_ASSERT(!lemmaIter.hasNext());
        }

        {
            LayerTagMask formMask_ = lattice.getLayerTagManager().getMask("form");
            Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);

            TS_ASSERT(formIter.hasNext());
            Lattice::EdgeDescriptor prowokacjamiForm = formIter.next();
            AnnotationItem prowokacjamiItem = lattice.getEdgeAnnotationItem(prowokacjamiForm);

            TS_ASSERT_EQUALS(prowokacjamiItem.getCategory(), "subst");
            TS_ASSERT_EQUALS(prowokacjamiItem.getText(), "prowokacja_subst");

            TS_ASSERT(!formIter.hasNext());
        }

	}


};

