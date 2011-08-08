#include "../morfologik.hpp"

class MorfologikTests : public CxxTest::TestSuite {

public:
	void testSingularStems() {
		Morfologik morf;
		std::vector<std::string> stems = morf.stem("dziecku");
	
		TS_ASSERT_EQUALS((int)stems.size(), 2);
		TS_ASSERT_EQUALS(stems.front(), "dziecko");
		TS_ASSERT_DIFFERS(stems.back(), "");
	}

	void testUnrecognizedWord() {
		Morfologik morf;
		std::vector<std::string> stems = morf.stem("Dziecko");
	
		TS_ASSERT_EQUALS((int)stems.size(), 0);
	}

	void testMultipleStems() {
		Morfologik morf;
		std::vector<std::string> stems = morf.stem("mam");

		TS_ASSERT_EQUALS((int)stems.size(), 6);
		TS_ASSERT_EQUALS(stems[0], "mama");
		TS_ASSERT_EQUALS(stems[2], "mamić");
		TS_ASSERT_EQUALS(stems[4], "mieć");
	}

	void testSplitTags() {
		Morfologik morf;
		std::vector<std::string> stems = morf.stem("jakie");

		TS_ASSERT_EQUALS((int)stems.size(), 4);
		TS_ASSERT_EQUALS(stems[0], "jaki");
		TS_ASSERT_EQUALS(stems[2], "jaki");
	}

};

