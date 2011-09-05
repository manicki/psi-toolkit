#include "file_fetcher.hpp"

#include "logging.hpp"
#include "config.h"

#include "ant_like_path_glob.hpp"

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

boost::filesystem::path FileFetcher::getRootDir_() {
    return boost::filesystem::path(ROOT_DIR);
}
