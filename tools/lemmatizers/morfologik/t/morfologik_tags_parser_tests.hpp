#include "../morfologik_tags_parser.hpp"

class MorfologikTagsParserTests : public CxxTest::TestSuite {

public:
	void testSimpleTag() {
		MorfologikTagsParser mrfTagPrs;
		std::string tag = "subst:sg:inst:n"; // For word "dzieckiem"
		std::vector<std::map<std::string, std::string> > result = 
			mrfTagPrs.parse(tag);

		TS_ASSERT_EQUALS((int)result.size(), 1);
		TS_ASSERT_EQUALS((int)result[0].size(), 4);
	}

	void testComplexTag() {
		MorfologikTagsParser mrfTagPrs;
		std::string tag = "adj:pl:acc.nom.voc:f.m2.m3.n:pos"; // For word "jakie"
		std::vector<std::map<std::string, std::string> > result = 
			mrfTagPrs.parse(tag);

		TS_ASSERT_EQUALS((int)result.size(), 12);
	}

};
