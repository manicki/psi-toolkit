#include "../morfologik.hpp"

class MorfologikTests : public CxxTest::TestSuite {

public:
	void testSingularStems() {
		Morfologik morf;
		std::multimap<std::string, std::string> stems = morf.stem("dziecku");
	
		TS_ASSERT_EQUALS((int)stems.size(), 1);

		std::multimap<std::string, std::string>::iterator firstStem = stems.begin();

		TS_ASSERT_EQUALS(firstStem->first, "dziecko");
		TS_ASSERT_DIFFERS(firstStem->second, "");
	}

	void testUnrecognizedWord() {
		Morfologik morf;
		std::multimap<std::string, std::string> stems = morf.stem("Dziecko");
	
		TS_ASSERT_EQUALS((int)stems.size(), 0);
	}

	void testMultipleStems() {
		Morfologik morf;
		std::multimap<std::string, std::string> stems = morf.stem("mam");

		TS_ASSERT_EQUALS((int)stems.size(), 3);

		std::multimap<std::string, std::string>::iterator stem = stems.begin();

		TS_ASSERT_EQUALS(stem->first, "mama");
		TS_ASSERT_EQUALS((++stem)->first, "mamić");
		TS_ASSERT_EQUALS((++stem)->first, "mieć");
	}

	void testSplitTags() {
		Morfologik morf;
		std::multimap<std::string, std::string> stems = morf.stem("jakie");

		TS_ASSERT_EQUALS((int)stems.size(), 2);

		std::multimap<std::string, std::string>::iterator stem = stems.begin();

		TS_ASSERT_EQUALS(stem->first, "jaki");
		TS_ASSERT_EQUALS((++stem)->first, "jaki");
	}

};

