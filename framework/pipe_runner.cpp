#include "pipe_runner.hpp"

#include <iostream>
#include <sstream>
#include <list>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>

#include "main_factories_keeper.hpp"
#include "console_help_formatter.hpp"

#include "annotator_promise.hpp"
#include "non_annotator_promise.hpp"

#include "logging.hpp"
#include "version_information.hpp"

PipeRunner::PipeRunner(const std::string& pipeline)
    : justInformation_(false), runnerOptionsDescription_("PipeRunner options") {

    parseIntoFinalPipeline_<std::istream, std::ostream>(splitPipeline_(pipeline), false);
}

PipeRunner::PipeRunner(int argc, char* argv[])
    : justInformation_(false), runnerOptionsDescription_("PipeRunner options") {

    std::vector<std::string> args(argv, argv + argc);
    parseIntoFinalPipeline_<std::istream, std::ostream>(args, true);
}

PipeRunner::PipeRunner(std::vector<std::string> args)
    : justInformation_(false), runnerOptionsDescription_("PipeRunner options") {

    parseIntoFinalPipeline_<std::istream, std::ostream>(args, false);
}

int PipeRunner::run(std::istream& in, std::ostream& out) {
    if (justInformation_)
        return 0;

    return run_<std::istream, std::ostream>(in, out);
}

const std::string PipeRunner::PIPELINE_SEPARATOR = "!";

template<typename Source, typename Sink>
void PipeRunner::parseIntoFinalPipeline_(
    std::vector<std::string> args, bool isTheFirstArgProgramName) {

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

    promiseAlternativeSequence_
        = pipelineSpecification2PromiseAlternativeSequence_(
            pipelineSpecification_);

    checkReader_<Source>();
    checkWriter_<Sink>();
    std::list<std::string> langCodes = getLangCodes_();
    checkLangGuesser_(langCodes);

    INFO("the following languages are taken into account: "
         << boost::algorithm::join(langCodes, ", "));

    AutoCompleter autoCompleter(promiseAlternativeSequence_, langCodes);

    boost::optional<ProcessorPromiseSequence> returnedSeq
        = autoCompleter.complete();

    if (!returnedSeq)
        throw Exception("cannot be resolved");
    else
        finalPipeline_ = *returnedSeq;

    if (stopAfterParsingPipeline_()) {
        justInformation_ = true;

        return;
    }
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
        ("aliases", "Show aliases, i.e. alternative names for processors")
        ("list-languages", "List languages handled for each processor specified")
        ("log-level", boost::program_options::value<std::string>(),
         "Set logging level")
        ("log-file", boost::program_options::value<std::string>(),
         "Filepath to store logs (if not set: standard error)")
        ("version", "output version information and exit");
}

bool PipeRunner::stopAfterExecutingRunnerOptions_() {
    if (runnerOptions_.count("help")) {
        ConsoleHelpFormatter().formatHelpIntroduction(std::cout);
        std::cout << runnerOptionsDescription_ << std::endl;
        ConsoleHelpFormatter().formatHelps(std::cout);
        return true;
    }

    if (runnerOptions_.count("aliases")) {
        std::cout << "Aliases:" << std::endl;
        ConsoleHelpFormatter().formatAliases(std::cout);
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
    for (FinalPipeline::iterator current = finalPipeline_.begin();
         current != finalPipeline_.end();
         ++current) {
        listLanguagesForPipelineNode_(current);
    }
}

void PipeRunner::listLanguagesForPipelineNode_(FinalPipeline::iterator current) {

    if ((*current)->isAnnotator()) {
        std::cout << (*current)->getName() << ":";

        BOOST_FOREACH(const std::string& langCode,
                      (*current)->languagesHandled())
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

ProcessorPromiseAlternativeSequence
PipeRunner::pipelineSpecification2PromiseAlternativeSequence_(
    PipelineSpecification& pipelineSpec) {

    ProcessorPromiseAlternativeSequence sequence;
    BOOST_FOREACH(const PipelineElementSpecification& element, pipelineSpec.elements)
        sequence.push_back(pipelineElement2Promises_(element));

    return sequence;
}


PipeRunner::FinalPipeline PipeRunner::pipelineSpecification2FinalPipeline_(
    PipelineSpecification& pipelineSpec) {

    FinalPipeline fpipeline;

    BOOST_FOREACH(const PipelineElementSpecification& element, pipelineSpec.elements) {
        fpipeline.push_back(pipelineElement2Node_(element));
    }

    return fpipeline;
}

template<typename Source, typename Sink>
void PipeRunner::completeFinalPipeline_() {
    checkReader_<Source>();
    checkWriter_<Sink>();
}

template<typename Source>
void PipeRunner::checkReader_() {
    bool foundReader = false;

    BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *promiseAlternativeSequence_.front()) {
        if (promise->isReader<Source>())
            foundReader = true;
    }

    if (!foundReader)
        prepend_("txt-reader --line-by-line");
}

template<typename Sink>
void PipeRunner::checkWriter_() {

    std::string continuation;

    BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *promiseAlternativeSequence_.back()) {
        continuation = promise->getContinuation();
    }

    if (!continuation.empty())
        append_(continuation);

    bool foundWriter = false;

    BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *promiseAlternativeSequence_.back()) {
        if (promise->isWriter<Sink>())
            foundWriter = true;
    }

    if (!foundWriter)
        ERROR("no writer found");
}

void PipeRunner::prepend_(const std::string& pipeline) {
    PipelineSpecification prependedSpec;
    parseIntoPipelineSpecification_(splitPipeline_(pipeline), false, prependedSpec);

    ProcessorPromiseAlternativeSequence newSequence
        = pipelineSpecification2PromiseAlternativeSequence_(prependedSpec);

    newSequence.splice(newSequence.end(), promiseAlternativeSequence_);
    promiseAlternativeSequence_ = newSequence;
}

void PipeRunner::insertAfterReader_(const std::string& pipeline) {
    PipelineSpecification insertedSpec;
    parseIntoPipelineSpecification_(splitPipeline_(pipeline), false, insertedSpec);

    ProcessorPromiseAlternativeSequence newSequence
        = pipelineSpecification2PromiseAlternativeSequence_(insertedSpec);

    ProcessorPromiseAlternativeSequence::iterator iter = promiseAlternativeSequence_.begin();
    ++iter;

    promiseAlternativeSequence_.splice(iter, newSequence);
}

void PipeRunner::append_(const std::string& pipeline) {
    PipelineSpecification appendedSpec;
    parseIntoPipelineSpecification_(splitPipeline_(pipeline), false, appendedSpec);

    ProcessorPromiseAlternativeSequence newSequence
        = pipelineSpecification2PromiseAlternativeSequence_(appendedSpec);

    promiseAlternativeSequence_.splice(
        promiseAlternativeSequence_.end(),
        newSequence);
}

std::list<std::string> PipeRunner::getLangCodes_() {
    std::list<std::string> langCodes;
    std::list<std::string> justOneLangCodes;

    BOOST_FOREACH(ProcessorPromiseAlternative alt, promiseAlternativeSequence_) {
        BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *alt) {
            if (promise->languagesHandling() == AnnotatorFactory::JUST_ONE_LANGUAGE)
                justOneLangCodes.push_back(
                    promise->languagesHandled().front());

            BOOST_FOREACH(std::string langCode, promise->languagesHandled())
                langCodes.push_back(langCode);
        }
    }

    if (!justOneLangCodes.empty())
        langCodes = justOneLangCodes;

    langCodes.sort();
    langCodes.unique();

    return langCodes;
}

void PipeRunner::checkLangGuesser_(const std::list<std::string>& langCodes) {
    if (!isLangGuesserAlreadyThere_()) {
        if (langCodes.size() == 1)
            forceLanguage_(langCodes.front());
        else if (langCodes.size() > 1)
            addLangGuesser_();
    }
}

bool PipeRunner::isLangGuesserAlreadyThere_() {
    BOOST_FOREACH(ProcessorPromiseAlternative alt, promiseAlternativeSequence_) {
        BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *alt) {
            if (promise->languagesHandling() == AnnotatorFactory::LANGUAGE_GUESSER)
                return true;
        }
    }

    return false;
}

void PipeRunner::forceLanguage_(const std::string& langCode) {
    insertAfterReader_(std::string("lang-guesser --force --only-langs ") + langCode);
}

void PipeRunner::addLangGuesser_() {
    insertAfterReader_(std::string("lang-guesser"));
}

template<typename Source, typename Sink>
int PipeRunner::run_(Source& in, Sink& out) {
    AnnotationItemManager aim;
    Lattice lattice(aim);

    for (FinalPipeline::iterator current = finalPipeline_.begin();
         current != finalPipeline_.end();
         ++current)
        runPipelineNode_<Source, Sink>(current, lattice, in, out);

    return 0;
}

template<typename Source, typename Sink>
void PipeRunner::runPipelineNode_(
    FinalPipeline::iterator current,
    Lattice& lattice, std::istream& in, Sink & out) {

    std::stringstream messageStream;
    if (!(*current)->checkRequirements(messageStream)) {

        ERROR("Cannot run node, because requirements are not met:\n"
             << messageStream.str());
        return;
    }

    boost::shared_ptr<Processor> processor = (*current)->createProcessor();

    FinalPipeline::iterator lastElementIter = finalPipeline_.end();
    --lastElementIter;

    if (current == finalPipeline_.begin()) {
        boost::shared_ptr<LatticeReader<Source> > reader =
            boost::dynamic_pointer_cast<LatticeReader<Source> >(
                processor);

        if (!reader)
            throw Exception("first element of the pipeline should be a writer");

        reader->readIntoLattice(in, lattice);
    }
    else if (current == lastElementIter) {
        boost::shared_ptr<LatticeWriter<Sink> > writer =
            boost::dynamic_pointer_cast<LatticeWriter<Sink> >(
                processor);

        if (!writer)
            throw Exception("last element of the pipeline should be a writer");

        writer->writeLattice(lattice, out);
    }
    else {
        boost::shared_ptr<Annotator> annotator =
            boost::dynamic_pointer_cast<Annotator>(
                processor);

        if (!annotator)
            throw Exception("unexpected reader or writer in the middle of the pipeline");

        annotator->annotate(lattice);
    }
}

boost::shared_ptr<ProcessorPromise> PipeRunner::pipelineElement2Node_(
    const PipelineElementSpecification& element) {

    boost::shared_ptr<std::list<boost::shared_ptr<ProcessorPromise> > > promises
        = pipelineElement2Promises_(element);

    size_t numberOfPromisesFound = promises->size();

    if (numberOfPromisesFound == 0)
        throw Exception(std::string("no processor found for `")
                        + element.processorName + "`");
    else if (numberOfPromisesFound > 1)
        throw Exception(std::string("too many processors found for `")
                        + element.processorName
                        + "`, processor resolution not implemented yet");

    return promises->front();
}

boost::shared_ptr<std::list<boost::shared_ptr<ProcessorPromise> > >
PipeRunner::pipelineElement2Promises_(
    const PipelineElementSpecification& elementSpec) {

    boost::shared_ptr<std::list<boost::shared_ptr<ProcessorPromise> > > promises(
        new std::list<boost::shared_ptr<ProcessorPromise> >());

    std::list<ProcessorFactory*> factories =
        MainFactoriesKeeper::getInstance().getProcessorFactoriesForName(
            elementSpec.processorName);

    BOOST_FOREACH(ProcessorFactory* factory, factories) {

        boost::program_options::variables_map options;

        bool optionsMatched = true;

        try {
            options
                = tryOptions_(
                    factory,
                    elementSpec.processorArgs);
        } catch (boost::program_options::error& optionsError) {
            optionsMatched = false;

            if (factories.size() == 1)
                throw;

            INFO("skipping " << factory->getName() << " "
                 << optionsError.what());
        }

        if (optionsMatched)
            promises->push_back(createPromise_(factory, options));
    }

    if (promises->empty())
        throw Exception("none of the alias alternatives matched the options");

    return promises;
}

boost::shared_ptr<ProcessorPromise> PipeRunner::createPromise_(
    ProcessorFactory* factory, const boost::program_options::variables_map& options) {

    AnnotatorFactory* asAnnotatorFactory =
        dynamic_cast<AnnotatorFactory*>(factory);

    return asAnnotatorFactory
        ? boost::shared_ptr<ProcessorPromise>(new AnnotatorPromise(asAnnotatorFactory, options))
        : boost::shared_ptr<ProcessorPromise>(new NonAnnotatorPromise(factory, options));
}

boost::program_options::variables_map PipeRunner::tryOptions_(
    ProcessorFactory* factory,
    const std::list<std::string>& processorArgs) {

    boost::program_options::variables_map options;

    try {
        options = parseOptions_(factory->optionsHandled(), processorArgs);
    } catch (boost::program_options::error& optionsError) {

        AnnotatorFactory* asAnnotatorFactory =
            dynamic_cast<AnnotatorFactory*>(factory);

        if (asAnnotatorFactory) {
            std::list<std::string> processorArgsWithoutLang = processorArgs;

            std::list<std::string>::iterator iter = processorArgsWithoutLang.begin();
            while (iter != processorArgsWithoutLang.end()
                   && (*iter) != "--lang")
                ++iter;

            if (iter != processorArgsWithoutLang.end()) {
                std::list<std::string>::iterator b_iter = iter;
                ++b_iter;
                std::string langCode = *b_iter;

                if (b_iter != processorArgsWithoutLang.end()) {
                    ++b_iter;
                    processorArgsWithoutLang.erase(iter, b_iter);

                    bool failed = false;

                    boost::program_options::variables_map optionsWithoutLang;

                    try {
                        optionsWithoutLang
                            = parseOptions_(factory->optionsHandled(), processorArgsWithoutLang);
                    } catch (boost::program_options::error& optionsError) {
                        failed = true;
                    }

                    if (failed)
                        throw;
                    else {
                        if (asAnnotatorFactory->languagesHandling(optionsWithoutLang)
                            == AnnotatorFactory::JUST_ONE_LANGUAGE) {
                            if (asAnnotatorFactory->languagesHandled(optionsWithoutLang).front()
                                == langCode)
                                return optionsWithoutLang;
                            else
                                throw;
                        }
                        else
                            throw;
                    }
                }
            }
        }

        throw;
    }

    return options;
}


boost::program_options::variables_map PipeRunner::parseOptions_(
    const boost::program_options::options_description& optionsDescription,
    const std::list<std::string>& processorArgs) {

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
