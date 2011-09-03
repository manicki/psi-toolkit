#ifndef SEGMENT_GLOB_HDR
#define SEGMENT_GLOB_HDR

#include <string>

/*!
  Class representing globs (i.e. filename pattern with `*' and '?' wildcards
  characters).

  Only `*' and '?' are recognized (in particular [...] is NOT
  handled). This class should be used only for globs representing
  single filename (NOT a directory path).
*/

class SegmentGlob {

public:
    explicit SegmentGlob(const std::string& glob);

    /**
       Checks whether a path segment matches the glob.
    */
    bool matches(const std::string& segment);

    /**
       Checks whether the glob is fixed (i.e. does not contain wildcard characters.
    */
    bool isFixed();

    const std::string& asString() const;

private:
    std::string glob_;

    bool isFixed_;

    void checkIfFixed_();
};

#endif
