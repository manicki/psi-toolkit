#include "output_saver.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include <fstream>

const int OutputSaver::GUID_LENGTH = 20;

const std::string OutputSaver::DEFAULT_FILE_EXTENSION = "txt";

const std::string OutputSaver::STORAGE_DIR = "storage";

OutputSaver::OutputSaver(const std::string& rootPath)
    : guidGenerator_(GUID_LENGTH, true), websiteRoot_(rootPath + '/') {

    boost::filesystem::path storageDir(websiteRoot_ + STORAGE_DIR);

    if (!boost::filesystem::is_directory(storageDir)) {
        WARN("The " << storageDir.string() << " does not exist.");

        if (!boost::filesystem::create_directory(storageDir)) {
            WARN("The storage directory can not be created.");
        }
        else {
            INFO("The storage directory is created successfully.");
        }
    }
}

std::string OutputSaver::storeOutput(const std::string& content) {
    std::string ext = fileRecognizer_.recognizeExtension(content);

    if (ext == FileRecognizer::UNKNOWN_TYPE) {
        ext = DEFAULT_FILE_EXTENSION;
    }

    return storeOutput(content, ext);
}

std::string OutputSaver::storeOutput(const std::string& content, const std::string& ext) {
    std::string newGuid = guidGenerator_.getGUID();
    std::string pathToStore = filePathToStore_(newGuid, ext);
    std::string pathForHtml = filePathForHtml_(newGuid, ext);

    INFO("Output saved to file: " << pathToStore);

    std::ofstream outfile(pathToStore.c_str());
    outfile << content;
    outfile.close();

    return pathForHtml;
}

std::string OutputSaver::filePathToStore_(const std::string& guid, const std::string& ext) {
    return std::string(websiteRoot_ + STORAGE_DIR + '/' + guid + '.' + ext);
}

std::string OutputSaver::filePathForHtml_(const std::string& guid, const std::string& ext) {
    return std::string(STORAGE_DIR + '/' + guid + '.' + ext);
}
