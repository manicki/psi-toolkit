#include "../morfologik.hpp"

class MorfologikTests : public CxxTest::TestSuite {

public:
	void testSingularStems() {
		AnnotationItemManager annMng;
		Morfologik morf(annMng);
		std::multimap<std::string, std::string> stems = morf.stem("dziecku");
	
		TS_ASSERT_EQUALS((int)stems.size(), 1);

		std::multimap<std::string, std::string>::iterator stem = 
			stems.begin();

		TS_ASSERT_EQUALS(stem->first, "dziecko");
		TS_ASSERT_DIFFERS(stem->second, "");
	}

	void testUnrecognizedWord() {
		AnnotationItemManager annMng;
		Morfologik morf(annMng);
		std::multimap<std::string, std::string> stems = morf.stem("Dziecko");
	
		TS_ASSERT_EQUALS((int)stems.size(), 0);
	}

	void testMultipleStems() {
		AnnotationItemManager annMng;
		Morfologik morf(annMng);
		std::multimap<std::string, std::string> stems = morf.stem("mam");

		TS_ASSERT_EQUALS((int)stems.size(), 3);

		std::multimap<std::string, std::string>::iterator stem = stems.begin();

		TS_ASSERT_EQUALS(stem->first, "mama");
		TS_ASSERT_EQUALS((++stem)->first, "mamić");
		TS_ASSERT_EQUALS((++stem)->first, "mieć");
	}

	void testSplitTags() {
		AnnotationItemManager annMng;
		Morfologik morf(annMng);
		std::multimap<std::string, std::string> stems = morf.stem("jakie");

		TS_ASSERT_EQUALS((int)stems.size(), 2);

		std::multimap<std::string, std::string>::iterator stem = stems.begin();

		TS_ASSERT_EQUALS(stem->first, "jaki");
		TS_ASSERT_EQUALS((++stem)->first, "jaki");
	}

	void testAnnotationItemCreation() {
		AnnotationItemManager annMng;
		Morfologik morf(annMng);

		std::string word = "winne";
		std::list<AnnotationItem> stems = morf.stems(word);
		annMng = morf.annotationManager;

		TS_ASSERT_EQUALS((int)stems.size(), 18);

		std::list<AnnotationItem>::iterator annItm;
		for (annItm = stems.begin(); annItm != stems.end(); annItm++) {
			std::string cat = annMng.getCategory(*annItm);
			
			TS_ASSERT(word.compare(0,3,cat,0,3) == 0);

			std::list< std::pair<std::string, std::string> > values =
				annMng.getValues(*annItm);

			TS_ASSERT((int)values.size() > 3);
		}
	}

};

