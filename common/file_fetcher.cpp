#include "file_fetcher.hpp"

#include "logging.hpp"
#include "config.hpp"

#include "ant_like_path_glob.hpp"

void FileFetcher::addParam(const std::string& param, const std::string& value) {
    if (params_.count(param))
        WARN("param `" << param << "' already defined with value `" << value << "'");

    params_[param] = value;
}

boost::filesystem::path FileFetcher::getOneFile(
    const std::string& fileSpec) const {
    std::list<boost::filesystem::path> files;

    getFiles(fileSpec, std::back_inserter(files));

    if (files.empty())
        throw Exception("no file `" + fileSpec + "' [" + paramsToString_() + "]");
    else if (files.size() > 1)
        throw Exception("too many files matching `" + fileSpec + "'"
                        + "specification [" + paramsToString_() + "]");
    else
        return *(files.begin());
}

std::string FileFetcher::replaceParams_(const std::string& fileSpec) const {
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
            std::map<std::string,std::string>::const_iterator foundParam
                = params_.find(param);
            std::string value = foundParam->second;

            finalFileSpec.replace(percentPos,
                                  nextPercentPos - percentPos + 1,
                                  value);
            percentPos += value.length();
        }
    }

    return finalFileSpec;
}

std::string FileFetcher::paramsToString_() const {
    typedef std::map<std::string,std::string> this_map_type;

    std::string r;

    BOOST_FOREACH(const this_map_type::value_type& paramPair, params_) {
        if (!r.empty())
            r += ' ';

        r += paramPair.first + '=' + paramPair.second;
    }

    return r;
}
