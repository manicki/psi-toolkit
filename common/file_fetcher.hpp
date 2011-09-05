#ifndef FILE_FETCHER_HDR
#define FILE_FETCHER_HDR

#include <string>
#include <map>
#include <exception>
#include <set>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "ant_like_path_glob.hpp"

class FileFetcher {

public:
    class Exception : public std::exception {
    public:
        Exception(const std::string& msg)
            :msg_(msg) {
        }

        virtual ~Exception() throw() {
        }

        virtual const char* what() const throw() {
            return msg_.c_str();
        }
    private:
        std::string msg_;
    };

    FileFetcher() {
    }

    void addParam(const std::string& param, const std::string& value);

    boost::filesystem::path getOneFile(const std::string& fileSpec);

    template<typename Out>
    void getFiles(const std::string& fileSpec, Out outputIterator) {
        std::string finalFileSpec = replaceParams_(fileSpec);

        std::set<boost::filesystem::path> files;

        std::vector<std::string> fileGlobs;
        boost::split(fileGlobs, finalFileSpec, boost::is_any_of(",;"));

        for (std::vector<std::string>::const_iterator giter = fileGlobs.begin();
             giter != fileGlobs.end();
             ++giter) {
            AntLikePathGlob glob(*giter);
            boost::filesystem::path here;
            glob.allMatchingFiles(here, files);
        }

        for (std::set<boost::filesystem::path>::iterator fiter = files.begin();
             fiter != files.end();
             ++fiter)
            *outputIterator++ = *fiter;
    }

protected:
    static boost::filesystem::path getRootDir_();

private:
    std::string replaceParams_(const std::string& fileSpec);

    std::map<std::string, std::string> params_;

};

#endif
