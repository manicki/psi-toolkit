#include "file_storage.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include <fstream>

const int FileStorage::GUID_LENGTH = 20;

const std::string FileStorage::DEFAULT_FILE_EXTENSION = "txt";

const std::string FileStorage::STORAGE_DIR = "storage";

FileStorage::FileStorage(const std::string& rootPath)
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

std::string FileStorage::storeFile(const std::string& content) {
    std::string ext = fileRecognizer_.recognizeFileExtension(content);

    if (ext == FileRecognizer::UNKNOWN_TYPE) {
        ext = DEFAULT_FILE_EXTENSION;
    }

    return storeFile(content, ext);
}

std::string FileStorage::storeFile(const std::string& content, const std::string& ext) {
    std::string newGuid = guidGenerator_.getGUID();

    return storeFile_(content, newGuid, ext);
}

std::string FileStorage::storeFile_(
    const std::string& content,
    const std::string& fileName,
    const std::string& ext) {

    std::string pathToStore = filePathToStore_(fileName, ext);
    std::string pathForHtml = filePathForHtml_(fileName, ext);

    INFO("File stored in file: " << pathToStore);

    std::ofstream outfile(pathToStore.c_str());
    outfile << content;
    outfile.close();

    return pathForHtml;
}

std::string FileStorage::filePathToStore_(const std::string& fileName, const std::string& ext) {
    return std::string(websiteRoot_ + STORAGE_DIR + '/' + fileName + '.' + ext);
}

std::string FileStorage::filePathForHtml_(const std::string& fileName, const std::string& ext) {
    return std::string('/' + STORAGE_DIR + '/' + fileName + '.' + ext);
}

std::string FileStorage::storeFileByMD5(const std::string& content, const std::string& ext) {
    std::string checksum = md5(content);

    if (!isFileStored_(checksum, ext)) {
        return storeFile_(content, checksum, ext);
    }

    return filePathForHtml_(checksum, ext);
}

bool FileStorage::isFileStored_(const std::string& fileName, const std::string& ext) {
    return boost::filesystem::exists(filePathToStore_(fileName, ext));
}
