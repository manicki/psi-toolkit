#ifndef FILE_FETCHER_HDR
#define FILE_FETCHER_HDR

#include <string>
#include <map>
#include <exception>

#include <boost/filesystem.hpp>

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

    explicit FileFetcher(boost::filesystem::path sourceFilePath);

    void addParam(const std::string& param, const std::string& value);

    boost::filesystem::path getOneFile(const std::string& fileSpec);

    template<typename Out>
    void getFiles(const std::string& fileSpec, Out outputIterator);

private:
    std::string replaceParams_(const std::string& fileSpec);
    void initDirectoryParams_(boost::filesystem::path sourceFilePath);

    std::map<std::string, std::string> params_;

    static boost::filesystem::path getRootDir_();
};

#endif
