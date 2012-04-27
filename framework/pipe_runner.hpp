#ifndef PIPE_RUNNER_HDR
#define PIPE_RUNNER_HDR

#include "config.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "pipeline_specification.hpp"

#include "processor_factory.hpp"
#include "processor_promise.hpp"
#include "lattice_reader_factory.hpp"
#include "lattice_writer_factory.hpp"
#include "annotator_factory.hpp"
#include "psi_exception.hpp"

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

private:
    PipelineSpecification pipelineSpecification_;
    bool justInformation_;

    template<typename Source, typename Sink>
    void parseIntoGraph_(std::vector<std::string> args, bool isTheFirstArgProgramName);

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

    ProcessorFactory& getFactory_(const PipelineElementSpecification& elementSpec);

    boost::program_options::variables_map parseOptions_(
        const boost::program_options::options_description& optionsDescription,
        const PipelineElementSpecification& pipelineElement);

    std::vector<std::string> splitPipeline_(const std::string& pipeline);

    static const std::string PIPELINE_SEPARATOR;
    static const std::string PIPELINE_STANDARD_INPUT_OR_OUTPUT_FILE_NAME;

    struct PipelineNode {
        ProcessorFactory* factory_;
        boost::program_options::variables_map options_;
        boost::shared_ptr<Processor> processor_;

        PipelineNode(ProcessorFactory& factory, boost::program_options::variables_map options)
            : factory_(&factory), options_(options) {
        }

        void createProcessor() {
            if (!processor_)
                processor_.reset(factory_->createProcessor(options_));
        }

        boost::shared_ptr<Processor> getProcessor() const {
            return processor_;
        }

        const ProcessorFactory* getFactory() const {
            return factory_;
        }

        boost::program_options::variables_map getOptions() const {
            return options_;
        }

        std::string getContinuation() const {
            return getFactory()->getContinuation(getOptions());
        }

    };

    PipelineNode pipelineElement2Node_(const PipelineElementSpecification& element);

    std::list<boost::shared_ptr<ProcessorPromise> > pipelineElement2Promises_(
        const PipelineElementSpecification& elementSpec);

    boost::shared_ptr<ProcessorPromise> createPromise_(
        ProcessorFactory* factory, const boost::program_options::variables_map& options);

    typedef boost::adjacency_list<
        boost::vecS,
        boost::listS,
        boost::bidirectionalS,
        PipelineNode,
        std::string
    > PipelineGraph;

    PipelineGraph pipelineGraph_;

    PipelineGraph::vertex_descriptor firstNode;
    PipelineGraph::vertex_descriptor lastNode;

    void pipelineSpecification2Graph_(
        PipelineSpecification& pipelineSpec,
        PipelineGraph::vertex_descriptor& firstVertex,
        PipelineGraph::vertex_descriptor& lastVertex);

    template<typename Source, typename Sink>
    void completeGraph_();

    void checkLangOption_();
    std::string getJustOneLanguage_();
    void setOnlyOneLanguage_(const std::string& langCode);

    template<typename Source>
    void checkReader_();

    template<typename Sink>
    void checkWriter_();
    void prepend_(const std::string& pipeline);
    void append_(const std::string& pipeline);

    template<typename Source, typename Sink>
    int run_(Source&, Sink&);

    template<typename Source, typename Sink>
    void runPipelineNode_(
        PipelineGraph::vertex_descriptor current,
        Lattice& lattice,
        std::istream& in, Sink & out);

    void listLanguagesForPipelineNode_(PipelineGraph::vertex_descriptor current);
    std::string getNodeJustOneLanguage_(PipelineGraph::vertex_descriptor);
    void setOnlyOneLanguageForNode_(
        const std::string& langCode, PipelineGraph::vertex_descriptor node);

    bool goToNextNode_(PipelineGraph::vertex_descriptor& current);

    static std::istream* createInputStreamOrReturnStandardInput(const std::string & path);
    static std::ostream* createOutputStreamOrReturnStandardOutput(const std::string & path);

    static void closeStreamWithStandardInputOrOutputCheck(std::ios * stream,
                                                          const std::string & path);

    static bool isStandardInputOrOutputFileName(const std::string & path);
};

#endif
