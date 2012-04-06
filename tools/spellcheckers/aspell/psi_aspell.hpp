#ifndef PSI_ASPELL_HDR
#define PSI_ASPELL_HDR

#include <aspell.h>

#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "annotator_factory.hpp"

typedef std::list<std::string> SuggestionsList;

class PSIAspell : public Annotator {

public:
    class Factory : public LanguageDependentAnnotatorFactory {
    private:
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options);

        virtual void doAddLanguageIndependentOptionsHandled(
            boost::program_options::options_description& optionsDescription);

        virtual std::string doGetName();
        virtual boost::filesystem::path doGetFile();

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();

        virtual std::string doGetContinuation(
            const boost::program_options::variables_map& options) const;

        static const unsigned int DEFAULT_LIMIT;
    };

    PSIAspell(const std::string & langCode);
    PSIAspell(const std::string & langCode,
              const boost::program_options::variables_map& options);
    ~PSIAspell();

    bool isWordCorrect(const std::string & word);
    void getSuggestionsForLastWord(
                                   SuggestionsList & suggestionsList,
                                   const std::string & word
                                   );

    static const std::list<std::string> tagsToOperateOn;
    static const std::list<std::string> tagsToPut;
private:

    class Worker : public LatticeWorker {
    public:
        Worker(Processor& processor, Lattice& lattice);
    private:
        virtual void doRun();
        bool processCheckEdgeIsIncorrect_(const Lattice::EdgeDescriptor & edgeToCheck);
        bool processCheckMultiEdgesAreIncorrect_(const Lattice::EdgeDescriptor & firstEdgeToCheck,
                                     const Lattice::EdgeDescriptor & separatingEdge,
                                     const Lattice::EdgeDescriptor & secondEdgeToCheck);
        bool processCheckMultiEdgesAreIncorrect_(const Lattice::EdgeDescriptor & firstEdgeToCheck,
                                     const Lattice::EdgeDescriptor & secondEdgeToCheck);
        bool processAspellCheckOnText_(const std::string & textToCheck,
                                       const Lattice::VertexDescriptor & sourceVertex,
                                       const Lattice::VertexDescriptor & targetVertex);

        SuggestionsList * checkWordInAspell_(const std::string & text);
        
        Processor& processor_;
        LayerTagCollection textTags_;
        
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);
    void initPSIAspell_(const std::string & langCode);
    void passOptionsToAspellConfig_(
         const boost::program_options::variables_map& options);
    void createAspellInstance_();

    virtual std::string doInfo();

    std::string langCode_;
    unsigned int limitCandidates_;
    
    AspellConfig * aspellConfig_;
    AspellSpeller * aspellSpeller_;
};

#endif
