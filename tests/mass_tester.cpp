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

MassTester::MassTester(int argc, char* argv[]) : testExtractor_(TestExtractor()) {
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
    testExtractor_.lookForTestBatches(directories_, "");

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

void MassTester::runAllBatches_() {
    BOOST_FOREACH(TestBatch batch, testExtractor_.getTestBatches()) {
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
