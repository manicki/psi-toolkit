#include "mass_tester.hpp"

#include <fstream>

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>

#include "configurator.hpp"
#include "batch_runner.hpp"
#include "logging.hpp"
#include "factories_keeper.hpp"

MassTester::MassTester(int argc, char* argv[]) {
    Configurator::getInstance().setRunAsInstalled(false);

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

    if (options.count("xml")) {
        std::string xml_file_name = options["xml"].as<std::string>();

        outStream_.reset(new std::ofstream(xml_file_name.c_str()));
        reporter_.reset(new JUnitMassTestsReporter(*outStream_.get()));
    }
    else
        reporter_.reset(new NoneMassTestsReporter(std::cout));
}

int MassTester::run() {
    BOOST_FOREACH(boost::filesystem::path path, directories_) {
        lookForTestBatches_(path);
    }

    runAllBatches_();

    reporter_->finish();

    return 0;
}

boost::program_options::options_description MassTester::optionsHandled() {

    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("directory", boost::program_options::value<std::vector<std::string> >(),
         "directories to traverse")
        ("xml", boost::program_options::value<std::string>(), "XML output file (if wanted)")
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


typedef std::map<std::string, boost::filesystem::path> file_map_type;

void MassTester::addTestBatch_(const boost::filesystem::path& directory) {
    INFO("registering test batch " << directory);

    boost::filesystem::path commandFileName = directory / TEST_COMMAND_FILE_NAME;

    if (!boost::filesystem::is_regular_file(commandFileName)) {
        WARN("no " << commandFileName << " found");
    }

    TestBatch testBatch(directory, readCommand_(commandFileName));

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

void MassTester::parseFilePath_(const boost::filesystem::path& filePath,
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

    INFO("parsing file path " << filePath << " " << basename << "+" << infix << "+" << suffix);
}

void MassTester::checkFileMap_(std::map<std::string, boost::filesystem::path>& filemap,
                               const boost::filesystem::path& path,
                               const std::string& key,
                               const std::string& infoString) {
    WARN("checking " << path << " as " << infoString);

    if (filemap.count(key))
        WARN("cannot use " << path << " test " << infoString << " file "
             << "as a file with the same basename (" << filemap[key] << ") exists");
    else
        filemap[key] = path;
}


void MassTester::runAllBatches_() {
    BOOST_FOREACH(TestBatch batch, testBatches_) {
        runBatch_(batch);
    }
}

void MassTester::runBatch_(const TestBatch& batch) {
    std::cout << "running " << batch.getDirectory()
              << " [" << batch.getPipeline() << "]" << std::endl;

    try {
        BatchRunner runner(batch, *reporter_);

        while (runner.runSingleTest());
    } catch (FactoriesKeeper::UnknownProcessorException& ex) {
        WARN("SKIPPING TEST (" << ex.what() << ")");
    }
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
