#ifndef ANT_LIKE_PATH_GLOB_HDR
#define ANT_LIKE_PATH_GLOB_HDR

#include <string>
#include <set>
#include <list>
#include <boost/filesystem.hpp>

#include "segment_glob.hpp"

class AntLikePathGlob {

public:
    AntLikePathGlob(const std::string& globSpec);

    void allMatchingFiles(
        boost::filesystem::path currentPath,
        std::set<boost::filesystem::path>& matchedFiles);

private:
    void findMatchingFiles_(
        boost::filesystem::path currentPath,
        std::list<SegmentGlob>::iterator globIter,
        std::set<boost::filesystem::path>& matchedFiles);

    void checkFile_(
        boost::filesystem::path filePath,
        std::set<boost::filesystem::path>& matchedFiles,
        bool printWarning);

    boost::filesystem::path fixedPrefix_;
    std::list<SegmentGlob> segmentGlobs_;

};

#endif
