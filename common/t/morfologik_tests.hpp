#include "../tools/lemmatizers/morfologik/morfologik.hpp"

class MorfologikTests : public CxxTest::TestSuite {

	public:
		void testSingularStems() {
			Morfologik morf;

			std::vector<string>::iterator it;
			std::vector<string> stems = morf.stem("dziecku");
		
			TS_ASSERT_EQUALS((int)stems.size(), 2);
			TS_ASSERT_EQUALS(stems.front(), "dziecko");
		}


};

