#include "pipe_runner.hpp"

#include <iostream>
#include <sstream>
#include <list>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "main_factories_keeper.hpp"
#include "console_help_formatter.hpp"

#include "logging.hpp"
#include "version_information.hpp"

PipeRunner::PipeRunner(const std::string& pipeline)
    : justInformation_(false), runnerOptionsDescription_("PipeRunner options") {

    parseIntoGraph_<std::istream, std::ostream>(splitPipeline_(pipeline), false);
}

PipeRunner::PipeRunner(int argc, char* argv[])
    : justInformation_(false), runnerOptionsDescription_("PipeRunner options") {

    std::vector<std::string> args(argv, argv + argc);
    parseIntoGraph_<std::istream, std::ostream>(args, true);
}

PipeRunner::PipeRunner(std::vector<std::string> args)
    : justInformation_(false), runnerOptionsDescription_("PipeRunner options") {

    parseIntoGraph_<std::istream, std::ostream>(args, false);
}

int PipeRunner::run(std::istream& in, std::ostream& out) {
    if (justInformation_)
        return 0;

    return run_<std::istream, std::ostream>(in, out);
}

const std::string PipeRunner::PIPELINE_SEPARATOR = "!";

ProcessorFactory& PipeRunner::getFactory_(const PipelineElementSpecification& elementSpec) {
    std::list<ProcessorFactory*> factories =
        MainFactoriesKeeper::getInstance().getProcessorFactoriesForName(
            elementSpec.processorName);

    size_t numberOfFactoriesFound = factories.size();

    if (numberOfFactoriesFound == 0)
        throw Exception(std::string("no processor found for `")
                        + elementSpec.processorName + "`");
    else if (numberOfFactoriesFound > 1)
        throw Exception(std::string("too many processors found for `")
                        + elementSpec.processorName
                        + "`, processor resolution not implemented yet");

    return *factories.front();
}

template<typename Source, typename Sink>
void PipeRunner::parseIntoGraph_(std::vector<std::string> args, bool isTheFirstArgProgramName) {

    parseRunnerProgramOptions_(args);
    if (stopAfterExecutingRunnerOptions_()) {
        justInformation_ = true;

        return;
    }

    if ( ! parseIntoPipelineSpecification_(
                                          args,
                                          isTheFirstArgProgramName,
                                          pipelineSpecification_) ) {
        return;
    }

    pipelineSpecification2Graph_(pipelineSpecification_, firstNode, lastNode);

    if (stopAfterParsingPipeline_()) {
        justInformation_ = true;

        return;
    }

    completeGraph_<Source, Sink>();
}

void PipeRunner::parseRunnerProgramOptions_(std::vector<std::string> &args) {
    setRunnerOptionsDescription_();

    boost::program_options::parsed_options opts =
        boost::program_options::command_line_parser(args).
        options(runnerOptionsDescription_).
        allow_unregistered().
        run();

    boost::program_options::store(opts, runnerOptions_);
    boost::program_options::notify(runnerOptions_);

    args = boost::program_options::collect_unrecognized(
        opts.options, boost::program_options::include_positional);
}

void PipeRunner::setRunnerOptionsDescription_() {
    runnerOptionsDescription_.add_options()
        ("help", "Produce help message for each processor")
        ("list-languages", "List languages handled for each processor specified")
        ("log-level", boost::program_options::value<std::string>(),
         "Set logging level")
        ("log-file", boost::program_options::value<std::string>(),
         "Filepath to store logs (if not set: standard error)")
        ("version", "output version information and exit")
        ;
}

bool PipeRunner::stopAfterExecutingRunnerOptions_() {
    if (runnerOptions_.count("help")) {
        std::cout << runnerOptionsDescription_ << std::endl;
        ConsoleHelpFormatter().formatHelps(std::cout);
        return true;
    }

    if (runnerOptions_.count("version")) {
        std::cout << get_psi_toolkit_version_string() << std::endl;
        return true;
    }

    if (runnerOptions_.count("log-file")) {
        SET_LOGGER_FILE(runnerOptions_["log-file"].as<std::string>());
    }

    if (runnerOptions_.count("log-level")) {
        SET_LOGGING_LEVEL(runnerOptions_["log-level"].as<std::string>());
    }

    return false;
}

bool PipeRunner::stopAfterParsingPipeline_() {
    if (runnerOptions_.count("list-languages")) {
        listLanguages_();
        return true;
    }

    return false;
}

void PipeRunner::listLanguages_() {
    PipelineGraph::vertex_descriptor current = firstNode;

    do {
        listLanguagesForPipelineNode_(current);
        if (!goToNextNode_(current))
            break;
    } while (1);
}

void PipeRunner::listLanguagesForPipelineNode_(PipelineGraph::vertex_descriptor current) {
    PipelineNode& currentPipelineNode = pipelineGraph_[current];

    const AnnotatorFactory* asAnnotatorFactory =
        dynamic_cast<const AnnotatorFactory*>(
            currentPipelineNode.getFactory());

    if (asAnnotatorFactory) {
        std::cout << currentPipelineNode.getFactory()->getName() << ":";

        BOOST_FOREACH(const std::string& langCode,
                      asAnnotatorFactory->languagesHandled(currentPipelineNode.options_))
            std::cout << " " << langCode;

        std::cout << "\n";
    }
}

bool PipeRunner::parseIntoPipelineSpecification_(
    std::vector<std::string> args, bool isTheFirstArgProgramName,
    PipelineSpecification& pipelineSpec) {

    bool nameExpected = true;

    size_t startingIndex = (isTheFirstArgProgramName ? 1 : 0);

    if (startingIndex >= args.size()) {
        showEmptyPipeWarningMessage_();
        return false;
    }

    for (size_t i = startingIndex; i < args.size(); ++i) {

        if (i == startingIndex || args[i] == PIPELINE_SEPARATOR) {
            nameExpected = true;
            pipelineSpec.elements.push_back(PipelineElementSpecification());
        }

        if (args[i] == PIPELINE_SEPARATOR) {
            ;
        }
        else if (nameExpected) {
            pipelineSpec.elements.back().processorName = args[i];
            nameExpected = false;
        }
        else {
            pipelineSpec.elements.back().processorArgs.push_back(args[i]);
        }
    }

    return true;
}

void PipeRunner::showEmptyPipeWarningMessage_() {
    std::cerr << "try this: cd build; "
              << "framework/psi-pipe tp-tokenizer --lang pl ! psi-writer"
              << std::endl;
    std::cerr << "see --help option for details..." << std::endl;
    exit(1);
}

void PipeRunner::pipelineSpecification2Graph_(
    PipelineSpecification& pipelineSpec,
    PipelineGraph::vertex_descriptor& firstVertex,
    PipelineGraph::vertex_descriptor& lastVertex) {

    bool isFirst = true;
    PipelineGraph::vertex_descriptor currentVertex;

    BOOST_FOREACH(const PipelineElementSpecification& element, pipelineSpec.elements) {
        PipelineGraph::vertex_descriptor newVertex =
            boost::add_vertex(
                pipelineElement2Node_(element),
                pipelineGraph_);

        if (isFirst) {
            isFirst = false;
            firstVertex = newVertex;
        }
        else {
            std::string emptyString;
            boost::add_edge(currentVertex, newVertex, emptyString, pipelineGraph_);
        }

        currentVertex = newVertex;
    }

    lastVertex = currentVertex;
}

template<typename Source, typename Sink>
void PipeRunner::completeGraph_() {
    checkReader_<Source>();
    checkWriter_<Sink>();
}

template<typename Source>
void PipeRunner::checkReader_() {
    const LatticeReaderFactory<Source>* reader =
        dynamic_cast<const LatticeReaderFactory<Source>*>(
            pipelineGraph_[firstNode].getFactory());

    if (!reader)
        prepend_("txt-reader --line-by-line");
}

template<typename Sink>
void PipeRunner::checkWriter_() {
    PipelineNode& lastPipelineNode = pipelineGraph_[lastNode];

    std::string continuation = lastPipelineNode.getContinuation();

    if (!continuation.empty())
        append_(continuation);

    const LatticeWriterFactory<Sink>* writer =
        dynamic_cast<const LatticeWriterFactory<Sink> *>(
            pipelineGraph_[lastNode].getFactory());

    if (!writer)
        ERROR("no writer specified");

}

void PipeRunner::prepend_(const std::string& pipeline) {
    PipelineSpecification prependedSpec;
    parseIntoPipelineSpecification_(splitPipeline_(pipeline), false, prependedSpec);

    PipelineGraph::vertex_descriptor prevFirst = firstNode;
    PipelineGraph::vertex_descriptor prepLast;

    pipelineSpecification2Graph_(prependedSpec, firstNode, prepLast);

    std::string emptyString;
    boost::add_edge(prepLast, prevFirst, emptyString, pipelineGraph_);
}

void PipeRunner::append_(const std::string& pipeline) {
    PipelineSpecification appendedSpec;
    parseIntoPipelineSpecification_(splitPipeline_(pipeline), false, appendedSpec);

    PipelineGraph::vertex_descriptor prevLast = lastNode;
    PipelineGraph::vertex_descriptor appFirst;

    pipelineSpecification2Graph_(appendedSpec, appFirst, lastNode);

    std::string emptyString;
    boost::add_edge(prevLast, appFirst, emptyString, pipelineGraph_);
}

template<typename Source, typename Sink>
int PipeRunner::run_(Source& in, Sink& out) {
    Lattice lattice;

    PipelineGraph::vertex_descriptor current = firstNode;

    do {
        runPipelineNode_<Source, Sink>(current, lattice, in, out);
        if (!goToNextNode_(current))
            break;
    } while (1);

    return 0;
}

template<typename Source, typename Sink>
void PipeRunner::runPipelineNode_(
    PipelineGraph::vertex_descriptor current,
    Lattice& lattice, std::istream& in, Sink & out) {

    PipelineNode& currentPipelineNode = pipelineGraph_[current];
    currentPipelineNode.createProcessor();

    if (current == firstNode) {
        boost::shared_ptr<LatticeReader<Source> > reader =
            boost::dynamic_pointer_cast<LatticeReader<Source> >(
                currentPipelineNode.getProcessor());

        if (!reader)
            throw Exception("first element of the pipeline should be a writer");

        reader->readIntoLattice(in, lattice);
    }
    else if (current == lastNode) {
        boost::shared_ptr<LatticeWriter<Sink> > writer =
            boost::dynamic_pointer_cast<LatticeWriter<Sink> >(
                currentPipelineNode.getProcessor());

                if (!writer)
                    throw Exception("last element of the pipeline should be a writer");

        writer->writeLattice(lattice, out);
    }
    else {
        boost::shared_ptr<Annotator> annotator =
            boost::dynamic_pointer_cast<Annotator>(
                currentPipelineNode.getProcessor());

        if (!annotator)
            throw Exception("unexpected reader or writer in the middle of the pipeline");

        annotator->annotate(lattice);
    }
}

bool PipeRunner::goToNextNode_(PipelineGraph::vertex_descriptor& current) {
    std::pair<boost::graph_traits<PipelineGraph>::out_edge_iterator,
              boost::graph_traits<PipelineGraph>::out_edge_iterator> iterPair
        = boost::out_edges(current, pipelineGraph_);

    if (iterPair.first == iterPair.second)
        return false;
    else {
        current = boost::target(*iterPair.first, pipelineGraph_);

        ++iterPair.first;
        if (iterPair.first != iterPair.second)
            WARN("unexpected fork in pipeline graph");
    }

    return true;
}

PipeRunner::PipelineNode PipeRunner::pipelineElement2Node_(
    const PipelineElementSpecification& element) {

    ProcessorFactory& factory = getFactory_(element);

    return PipelineNode(
        factory,
        parseOptions_(factory.optionsHandled(), element));
}

boost::program_options::variables_map PipeRunner::parseOptions_(
    const boost::program_options::options_description& optionsDescription,
    const PipelineElementSpecification& pipelineElement) {

    const std::list<std::string>& processorArgs = pipelineElement.processorArgs;

    int argc = processorArgs.size() + 1;
    boost::scoped_array<char*> argv(new char* [argc + 1]);

    size_t i = 1;
    for (std::list<std::string>::const_iterator iter = processorArgs.begin();
         iter != processorArgs.end();
         ++iter, ++i)
        argv[i] = const_cast<char*>((*iter).c_str()); //trick necessary because of Boost 1.42 bug
    argv[argc] = 0;

    boost::program_options::variables_map options;
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv.get(),
                                                   optionsDescription), options);
    boost::program_options::notify(options);

    return options;
}

std::vector<std::string> PipeRunner::splitPipeline_(const std::string& pipeline) {
    std::vector<std::string> strs;
    // the first element should be the name of the program
    boost::split(strs, pipeline, boost::is_any_of(" "));
    return strs;
}

const std::string PipeRunner::PIPELINE_STANDARD_INPUT_OR_OUTPUT_FILE_NAME = "-";

int PipeRunner::run(const std::string& inputFilePath, const std::string& outputFilePath) {

    int resultStatus;

    std::istream * inputStream =
        createInputStreamOrReturnStandardInput(inputFilePath);
    if (!inputStream) {
        return 1;
    }

    std::ostream * outputStream =
        createOutputStreamOrReturnStandardOutput(outputFilePath);
    if (!outputStream) {
        return 1;
    }

    resultStatus = run(*inputStream, *outputStream);

    closeStreamWithStandardInputOrOutputCheck(inputStream, inputFilePath);
    closeStreamWithStandardInputOrOutputCheck(outputStream, outputFilePath);

    return resultStatus;
}

std::istream * PipeRunner::createInputStreamOrReturnStandardInput(const std::string & path) {
    if (isStandardInputOrOutputFileName(path)) {
        return &(std::cin);
    } else {
        std::ifstream * fileStream = new std::ifstream();
        fileStream->open(path.c_str());
        if (fileStream->is_open()) {
            return fileStream;
        } else {
            return NULL;
        }
    }
}

std::ostream * PipeRunner::createOutputStreamOrReturnStandardOutput(const std::string & path) {
    if (isStandardInputOrOutputFileName(path)) {
        return &(std::cout);
    } else {
        std::ofstream * fileStream = new std::ofstream();
        fileStream->open(path.c_str());
        if (fileStream->is_open()) {
            return fileStream;
        } else {
            return NULL;
        }
    }
}


void PipeRunner::closeStreamWithStandardInputOrOutputCheck(
    std::ios * stream, const std::string & path) {

    if (!isStandardInputOrOutputFileName(path)) {
        delete stream;
    }
}


bool PipeRunner::isStandardInputOrOutputFileName(const std::string & path) {
    return (path == PIPELINE_STANDARD_INPUT_OR_OUTPUT_FILE_NAME);
}

std::string PipeRunner::run(const std::string & inputString) {
    std::istringstream inputStream (inputString, std::istringstream::in);
    std::ostringstream outputStream;

    run(inputStream, outputStream);

    return outputStream.str();
}

#if HAVE_PERL_BINDINGS
SV * PipeRunner::run_for_perl(const std::string & inputString) {
    AV * outputArrayPointer = newAV();

    std::istringstream inputStream (inputString, std::istringstream::in);
    run_<std::istream, AV *>(inputStream, outputArrayPointer);
    return newRV_inc((SV *) outputArrayPointer);
}
#endif
