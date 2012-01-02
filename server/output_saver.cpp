#include "output_saver.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <fstream>

std::string OutputSaver::fileExtension = ".txt";
std::string OutputSaver::storageDir = "storage/";

OutputSaver::OutputSaver(std::string rootPath)
    : guidGenerator(20, true), websiteRoot(rootPath + "/") { }

std::string OutputSaver::storeOutput(std::string content) {

    std::string newGuid = guidGenerator.getGUID();
    std::string filePathToSave(websiteRoot + storageDir + newGuid + fileExtension);
    std::string filePathToReturn(storageDir + newGuid + fileExtension);

    INFO("Output saved to file: " << filePathToSave);

    std::ofstream outfile(filePathToSave.c_str());
    outfile << content;
    outfile.close();

    return filePathToReturn;
}
