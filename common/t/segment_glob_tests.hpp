#include "segment_glob.hpp"

class SegmentGlobTests : public CxxTest::TestSuite
{
public:
    void testSimple() {
        SegmentGlob glob("*.txt");

        TS_ASSERT(!glob.isFixed());
        TS_ASSERT(glob.matches("file.txt"));
        TS_ASSERT(glob.matches(".txt"));
        TS_ASSERT(glob.matches("ź.txt"));
        TS_ASSERT(!glob.matches("a-txt"));
        TS_ASSERT(!glob.matches("file.txt.bak"));
        TS_ASSERT(!glob.matches("txt.file"));
        TS_ASSERT(!glob.matches(".tx"));
        TS_ASSERT(!glob.matches(""));
        TS_ASSERT(!glob.matches("txt"));

        TS_ASSERT_EQUALS(glob.asString(), "*.txt");
    }

    void testQuestionMark() {
        SegmentGlob glob("file?.txt");

        TS_ASSERT(!glob.isFixed());
        TS_ASSERT(glob.matches("filex.txt"));
        TS_ASSERT(glob.matches("file1.txt"));
        TS_ASSERT(!glob.matches("file.txt"));
        TS_ASSERT(glob.matches("file?.txt"));
        TS_ASSERT(!glob.matches(".txt"));
    }

    void testQuestionMarkExtended() {
        SegmentGlob glob("file?.tx?");

        TS_ASSERT(!glob.isFixed());
        TS_ASSERT(glob.matches("filex.txt"));
        TS_ASSERT(glob.matches("filex.txb"));
        TS_ASSERT(!glob.matches("filex.txbb"));
        TS_ASSERT(glob.matches("file1.txt"));
        TS_ASSERT(!glob.matches("file.txt"));
        TS_ASSERT(glob.matches("file?.txt"));
        TS_ASSERT(!glob.matches(".txt"));
    }

    void testFixed() {
        SegmentGlob glob("test.gif");

        TS_ASSERT(glob.isFixed());
        TS_ASSERT(glob.matches("test.gif"));
        TS_ASSERT(!glob.matches("test"));
        TS_ASSERT(!glob.matches("bla-test.gif"));
        TS_ASSERT(!glob.matches(""));
    }

    void testHard() {
        SegmentGlob glob("ab*ab*b*c?");

        TS_ASSERT(!glob.isFixed());

        TS_ASSERT(glob.matches("ababbcx"));
        TS_ASSERT(glob.matches("abbbbbabbbbbbbbcccc"));
        TS_ASSERT(glob.matches("abbbbbabbbbbbbbccccd"));
        TS_ASSERT(glob.matches("ababbbbbc0"));
        TS_ASSERT(!glob.matches("abbbbbbcd"));
        TS_ASSERT(!glob.matches(""));
    }

    void testUtf8() {
        SegmentGlob glob("a??.txt");

        TS_ASSERT(!glob.isFixed());

        TS_ASSERT(glob.matches("abc.txt"));
        TS_ASSERT(glob.matches("aąć.txt"));
        TS_ASSERT(glob.matches("aąc.txt"));
        TS_ASSERT(!glob.matches("aą.txt"));

        SegmentGlob globB("a*b??.txt");

        TS_ASSERT(globB.matches("abźż.txt"));
        TS_ASSERT(globB.matches("abćą.txt"));
        TS_ASSERT(!globB.matches("abć.txt"));
    }

    void testAll() {
        SegmentGlob glob("*");

        TS_ASSERT(!glob.isFixed());

        TS_ASSERT(glob.matches("a.txt"));
        TS_ASSERT(glob.matches(""));
        TS_ASSERT(glob.matches("a"));
    }

};
