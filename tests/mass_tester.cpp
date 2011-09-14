#include "mass_tester.hpp"

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>


#include "logging.hpp"

MassTester::MassTester(int argc, char* argv[]) {
    boost::program_options::options_description optionsDescription = optionsHandled();

    boost::program_options::positional_options_description positionalDescription;
    positionalDescription.add("directory", -1);

    boost::program_options::variables_map options;


    boost::program_options::command_line_parser parser(argc, argv);
    parser.options(optionsDescription).positional(positionalDescription);

    boost::program_options::store(parser.run(), options);
    boost::program_options::notify(options);

    std::vector<std::string> directories = options["directory"].as<std::vector<std::string> >();

    BOOST_FOREACH(std::string directory, directories) {
        directories_.push_back(directory);
    }
}

int MassTester::run() {
    BOOST_FOREACH(boost::filesystem::path path, directories_) {
        lookForTestBatches_(path);
    }

    runAllBatches_();

    return 0;
}

boost::program_options::options_description MassTester::optionsHandled() {

    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("directory", boost::program_options::value<std::vector<std::string> >(),
         "directories to traverse")
        ;

    return optionsDescription;
}

void MassTester::lookForTestBatches_(const boost::filesystem::path& directory) {
    if (!boost::filesystem::is_directory(directory)) {
        WARN(directory << "is not a directory");
        return;
    }

    INFO("looking in " << directory);

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

void MassTester::lookInDirectory_(const boost::filesystem::path& directory) {
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator fiter(directory);
             fiter != end_iter;
             ++fiter)
        addTestBatch_(fiter->path());
}

void MassTester::addTestBatch_(const boost::filesystem::path& directory) {
    INFO("registering test batch " << directory);

    boost::filesystem::path commandFileName = directory / TEST_COMMAND_FILE_NAME;

    if (!boost::filesystem::is_regular_file(commandFileName)) {
        WARN("no " << commandFileName << " found");
    }

    TestBatch testBatch(directory, readCommand_(commandFileName));
    testBatches_.push_back(testBatch);
}

void MassTester::runAllBatches_() {
    BOOST_FOREACH(TestBatch batch, testBatches_) {
        runBatch_(batch);
    }
}

void MassTester::runBatch_(const TestBatch& batch) {
    INFO("running " << batch.getDirectory() << " [" << batch.getPipeline() << "]");
}

std::string MassTester::readCommand_(boost::filesystem::path commandFileName) {
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

std::string MassTester::stripComments_(const std::string& line) {
    size_t hashPosition = line.find('#');
    if (hashPosition != std::string::npos)
        return line.substr(hashPosition);
    return line;
}

const boost::filesystem::path MassTester::TEST_BATCH_DIRECTORY_NAME = "m";
const boost::filesystem::path MassTester::TEST_COMMAND_FILE_NAME = "WHAT";

