#include "output_saver.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include <fstream>

std::string OutputSaver::FILE_EXTENSION = ".txt";
std::string OutputSaver::STORAGE_DIR = "storage";

OutputSaver::OutputSaver(std::string rootPath)
    : guidGenerator_(20, true), websiteRoot_(rootPath + '/') {

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

std::string OutputSaver::storeOutput(std::string content) {

    std::string newGuid = guidGenerator_.getGUID();
    std::string filePathToSave(websiteRoot_ + STORAGE_DIR + '/' + newGuid + FILE_EXTENSION);
    std::string filePathToReturn(STORAGE_DIR + '/' + newGuid + FILE_EXTENSION);

    INFO("Output saved to file: " << filePathToSave);

    std::ofstream outfile(filePathToSave.c_str());
    outfile << content;
    outfile.close();

    return filePathToReturn;
}
