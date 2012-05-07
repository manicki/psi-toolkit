#ifndef PIPE_RUNNER_HDR
#define PIPE_RUNNER_HDR

#include "config.hpp"
#include <boost/shared_ptr.hpp>

#include "pipeline_specification.hpp"

#include "processor_factory.hpp"
#include "processor_promise.hpp"
#include "lattice_reader_factory.hpp"
#include "lattice_writer_factory.hpp"
#include "annotator_factory.hpp"
#include "psi_exception.hpp"

#include "auto_completer.hpp"

#if HAVE_PERL_BINDINGS
#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>
#endif

class PipeRunner {

public:
    explicit PipeRunner(const std::string& pipeline);
    PipeRunner(int argc, char* argv[]);
    PipeRunner(std::vector<std::string> args);

    int run(std::istream&, std::ostream&);
    int run(const std::string& inputFilePath, const std::string& outputFilePath);
    std::string run(const std::string & inputString);

#if HAVE_PERL_BINDINGS
    SV * run_for_perl(const std::string & inputString);
#endif

    class Exception : public PsiException  {
    public:
        Exception(const std::string& msg): PsiException(msg) {
        }

        virtual ~Exception() throw() {}
    };

    typedef ProcessorPromiseSequence FinalPipeline;

private:
    PipelineSpecification pipelineSpecification_;
    bool justInformation_;

    template<typename Source, typename Sink>
    void parseIntoFinalPipeline_(std::vector<std::string> args, bool isTheFirstArgProgramName);

    boost::program_options::options_description runnerOptionsDescription_;
    void setRunnerOptionsDescription_();
    boost::program_options::variables_map runnerOptions_;
    void parseRunnerProgramOptions_(std::vector<std::string> &args);
    bool stopAfterExecutingRunnerOptions_();
    bool stopAfterParsingPipeline_();
    void listLanguages_();

    bool parseIntoPipelineSpecification_(
        std::vector<std::string> args, bool isTheFirstArgProgramName,
        PipelineSpecification& pipelineSpec);
    void showEmptyPipeWarningMessage_();

    boost::program_options::variables_map tryOptions_(
        ProcessorFactory* factory,
        const std::list<std::string>& processorArgs);

    boost::program_options::variables_map parseOptions_(
        const boost::program_options::options_description& optionsDescription,
        const std::list<std::string>& processorArgs);

    std::vector<std::string> splitPipeline_(const std::string& pipeline);

    static const std::string PIPELINE_SEPARATOR;
    static const std::string PIPELINE_STANDARD_INPUT_OR_OUTPUT_FILE_NAME;

    boost::shared_ptr<ProcessorPromise> pipelineElement2Node_(
        const PipelineElementSpecification& element);

    boost::shared_ptr<std::list<boost::shared_ptr<ProcessorPromise> > > pipelineElement2Promises_(
        const PipelineElementSpecification& elementSpec);

    boost::shared_ptr<ProcessorPromise> createPromise_(
        ProcessorFactory* factory, const boost::program_options::variables_map& options);

    ProcessorPromiseAlternativeSequence promiseAlternativeSequence_;

    ProcessorPromiseAlternativeSequence pipelineSpecification2PromiseAlternativeSequence_(
        PipelineSpecification& pipelineSpec);

    FinalPipeline finalPipeline_;

    FinalPipeline pipelineSpecification2FinalPipeline_(
        PipelineSpecification& pipelineSpec);

    template<typename Source, typename Sink>
    void completeFinalPipeline_();

    template<typename Source>
    void checkReader_();

    template<typename Sink>
    void checkWriter_();
    void prepend_(const std::string& pipeline);
    void insertAfterReader_(const std::string& pipeline);
    void append_(const std::string& pipeline);

    std::list<std::string> getLangCodes_();

    void checkLangGuesser_(const std::list<std::string>& langCodes);
    bool isLangGuesserAlreadyThere_();
    void forceLanguage_(const std::string& langCode);
    void addLangGuesser_();

    template<typename Source, typename Sink>
    int run_(Source&, Sink&);

    template<typename Source, typename Sink>
    void runPipelineNode_(
        FinalPipeline::iterator current,
        Lattice& lattice,
        std::istream& in, Sink & out);

    void listLanguagesForPipelineNode_(FinalPipeline::iterator current);

    static std::istream* createInputStreamOrReturnStandardInput(const std::string & path);
    static std::ostream* createOutputStreamOrReturnStandardOutput(const std::string & path);

    static void closeStreamWithStandardInputOrOutputCheck(std::ios * stream,
                                                          const std::string & path);

    static bool isStandardInputOrOutputFileName(const std::string & path);
};

#endif
