#include "file_fetcher.hpp"

#include <boost/algorithm/string.hpp>

#include "logging.hpp"
#include "config.h"

#include "ant_like_path_glob.hpp"

FileFetcher::FileFetcher(boost::filesystem::path sourceFilePath) {
    initDirectoryParams_(sourceFilePath);
}

void FileFetcher::addParam(const std::string& param, const std::string& value) {
    if (params_.count(param))
        WARN("param `" << param << "' already defined with value `" << value << "'");

    params_[param] = value;
}

boost::filesystem::path FileFetcher::getOneFile(const std::string& fileSpec) {
    std::list<boost::filesystem::path> files;

    getFiles(fileSpec, std::back_inserter(files));

    if (files.empty())
        throw Exception("no file `" + fileSpec + "'");
    else if (files.size() > 1)
        throw Exception("too many files matching `" + fileSpec + "'"
                        + "specification");
    else
        return *(files.begin());
}

template<typename Out>
void FileFetcher::getFiles(const std::string& fileSpec, Out outputIterator) {
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

std::string FileFetcher::replaceParams_(const std::string& fileSpec) {
    size_t percentPos = 0;
    std::string finalFileSpec = fileSpec;

    while ((percentPos = finalFileSpec.find('%', percentPos)) != std::string::npos) {
        size_t nextPercentPos = finalFileSpec.find('%', percentPos + 1);

        if (nextPercentPos == std::string::npos)
            throw Exception("single % in file specification `"
                            + fileSpec + "'");

        std::string param = finalFileSpec.substr(percentPos + 1, nextPercentPos - percentPos - 1);

        if (params_.count(param) == 0)
            throw Exception(std::string("unknown param `") + param + "' in file specification `"
                            + fileSpec + "'");
        else {
            std::string value = params_[param];

            finalFileSpec.replace(percentPos,
                                  nextPercentPos - percentPos + 1,
                                  value);
            percentPos += value.length();
        }
    }

    return finalFileSpec;
}

void FileFetcher::initDirectoryParams_(boost::filesystem::path sourceFilePath) {

    if (boost::filesystem::is_regular_file(sourceFilePath))
        sourceFilePath = sourceFilePath.parent_path();

    sourceFilePath = sourceFilePath.relative_path();

    boost::filesystem::path sourcePath;
    bool found = false;

    for (boost::filesystem::path::iterator it = sourceFilePath.begin();
         it != sourceFilePath.end();
         ++it) {
        if ((*it).string() == "tools"
            || (*it).string() == "formatters"
            || (*it).string() == "server") {
            sourcePath /= (*it);
            found = true;
        }
        else if (found)
            sourcePath /= (*it);
    }

    if (!found)
        throw Exception(std::string("unexpected source path `") + sourceFilePath.string() + "'");

    boost::filesystem::path itsData = getRootDir_() / sourcePath / "data";

    params_["ITSDATA"] = itsData.string();
}

boost::filesystem::path FileFetcher::getRootDir_() {
    return boost::filesystem::path(ROOT_DIR);
}
