#include "test_extractor.hpp"

#include <fstream>

#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "logging.hpp"
#include "main_factories_keeper.hpp"

TestExtractor::TestExtractor() : directoryPrefix_("") {
    //SET_LOGGING_LEVEL("INFO");
}

void TestExtractor::lookForTestBatches(std::vector<boost::filesystem::path> directories,
    const std::string& prefix) {

    directoryPrefix_ = prefix;
    INFO("test directory prefix is " << directoryPrefix_);

    BOOST_FOREACH(boost::filesystem::path path, directories) {
        lookForTestBatches_(path);
    }
}

void TestExtractor::clearTestBatches() {
    testBatches_.clear();
}
std::vector<TestBatch> & TestExtractor::getTestBatches() {
    return testBatches_;
}

void TestExtractor::lookForTestBatches_(const boost::filesystem::path& directory) {

    if (!boost::filesystem::is_directory(directory)) {
        WARN(directory.string() << "is not a directory");
        return;
    }

    INFO("looking in " << directory.string());

    boost::filesystem::recursive_directory_iterator end_itr;
    for (boost::filesystem::recursive_directory_iterator fiter(directory);
         fiter != end_itr;
         ++fiter) {
        boost::filesystem::path filename(fiter->path().filename());
        if (boost::filesystem::is_directory(fiter->path())
                && filename == TEST_BATCH_DIRECTORY_NAME)
            lookInDirectory_(fiter->path());
    }
}

void TestExtractor::lookInDirectory_(const boost::filesystem::path& directory) {
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator fiter(directory);
             fiter != end_iter;
             ++fiter)
    {
        // trick for boost 1.48
        boost::filesystem::path testDir(fiter->path().filename());

        if (directoryPrefix_ == "" || boost::starts_with(testDir.string(), directoryPrefix_))
            addTestBatch_(fiter->path());
    }
}


typedef std::map<std::string, boost::filesystem::path> file_map_type;

void TestExtractor::addTestBatch_(const boost::filesystem::path& directory) {
    INFO("registering test batch " << directory.string());

    bool isExampleOnly = false;

    boost::filesystem::path commandFileName = directory / TEST_COMMAND_FILE_NAME;
    if (!boost::filesystem::is_regular_file(commandFileName)) {
        WARN("no " << commandFileName.string() << " found");
    }

    boost::filesystem::path descriptionFileName = directory / TEST_DESCRIPTION_FILE_NAME;
    if (!boost::filesystem::is_regular_file(descriptionFileName)) {
        WARN("no " << descriptionFileName.string() << " found");
    }

    boost::filesystem::path requiresFileName = directory / TEST_REQUIRES_FILE_NAME;
    if (boost::filesystem::is_regular_file(requiresFileName)
        && !checkRequirements_(requiresFileName)) {
        WARN("SKIPPING as some requirements were not met");
        return;
    }

    boost::filesystem::path exampleOnlyFileName = directory / TEST_EXAMPLE_ONLY_FILE_NAME;
    if (boost::filesystem::is_regular_file(exampleOnlyFileName)) {
        isExampleOnly = true;
    }

    TestBatch testBatch(
        directory,
        readCommand_(commandFileName),
        readCommand_(descriptionFileName),
        isExampleOnly);

    std::map<std::string, boost::filesystem::path> inputFiles;
    std::map<std::string, boost::filesystem::path> outputFiles;

    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator fiter(directory);
             fiter != end_iter;
         ++fiter) {
        std::string suffix;
        std::string infix;
        std::string basename;

        parseFilePath_(fiter->path(), basename, infix, suffix);

        if (infix == ".in")
            checkFileMap_(inputFiles, fiter->path(), basename, "input");
        else if (infix == ".out")
            checkFileMap_(outputFiles, fiter->path(), basename, "output");
    }

    BOOST_FOREACH(const file_map_type::value_type& pair, inputFiles) {
        if (outputFiles.count(pair.first) > 0)
            testBatch.addTestRun(
                TestRun(
                    pair.second,
                    outputFiles[pair.first]));
        else
            WARN("no expected output for " << pair.first);
    }

    testBatches_.push_back(testBatch);
}

void TestExtractor::parseFilePath_(const boost::filesystem::path& filePath,
                                std::string& basename,
                                std::string& infix,
                                std::string& suffix) {

    boost::filesystem::path outerStem(filePath.stem());
    boost::filesystem::path outerExtension(filePath.extension());
    boost::filesystem::path innerStem(outerStem.stem());
    boost::filesystem::path innerExtension(outerStem.extension());

    basename = innerStem.string();
    infix = innerExtension.string();
    suffix = outerExtension.string();

    INFO("parsing file path " << filePath.string()
         << " " << basename << "+" << infix << "+" << suffix);
}

void TestExtractor::checkFileMap_(std::map<std::string, boost::filesystem::path>& filemap,
                               const boost::filesystem::path& path,
                               const std::string& key,
                               const std::string& infoString) {
    WARN("checking " << path.string() << " as " << infoString);

    if (filemap.count(key))
        WARN("cannot use " << path.string() << " test " << infoString << " file "
             << "as a file with the same basename (" << filemap[key].string() << ") exists");
    else
        filemap[key] = path;
}

std::string TestExtractor::readCommand_(boost::filesystem::path commandFileName) {
    boost::filesystem::ifstream commandStream(commandFileName);

    std::string command;

    while (commandStream) {
        std::string line;
        std::getline(commandStream, line);
        line = stripComments_(line);
        command += line;
    }

    return command;
}

std::string TestExtractor::stripComments_(const std::string& line) {
    size_t hashPosition = line.find('#');
    if (hashPosition != std::string::npos)
        return line.substr(hashPosition);
    return line;
}

bool TestExtractor::checkRequirements_(boost::filesystem::path requiresFileName) {
    boost::filesystem::ifstream requiresStream(requiresFileName);

    MainFactoriesKeeper& keeper = MainFactoriesKeeper::getInstance();

    while (requiresStream) {
        std::string line;
        std::getline(requiresStream, line);

        if (!line.empty()) {
            WARN("checking requirement: `" << line << "`");

            try {
                keeper.getProcessorFactoriesForName(line).empty();
            } catch (FactoriesKeeper::UnknownProcessorException& ex) {
                WARN("SKIPPING TEST (" << ex.what() << ")");

                return false;
            }
        }
    }

    return true;
}

const boost::filesystem::path TestExtractor::TEST_BATCH_DIRECTORY_NAME = "m";
const boost::filesystem::path TestExtractor::TEST_COMMAND_FILE_NAME = "WHAT";
const boost::filesystem::path TestExtractor::TEST_DESCRIPTION_FILE_NAME = "DESCRIPTION";
const boost::filesystem::path TestExtractor::TEST_REQUIRES_FILE_NAME = "REQUIRES";
const boost::filesystem::path TestExtractor::TEST_EXAMPLE_ONLY_FILE_NAME = "EXAMPLE_ONLY";
