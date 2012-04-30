#ifndef AUTO_COMPLETER_HDR
#define AUTO_COMPLETER_HDR

#include <list>
#include <boost/shared_ptr.hpp>

#include "processor_promise.hpp"

typedef std::list<std::string> LayerTagList;

typedef boost::shared_ptr<ProcessorPromise> ProcessorPromiseSharedPtr;

typedef std::list<ProcessorPromiseSharedPtr> ProcessorPromiseSequence;

typedef boost::shared_ptr<std::list<ProcessorPromiseSharedPtr> > ProcessorPromiseAlternative;

typedef std::list<ProcessorPromiseAlternative> ProcessorPromiseAlternativeSequence;


class AutoCompleter {
public:
    AutoCompleter(
        const ProcessorPromiseAlternativeSequence& sequence,
        std::list<std::string> langCodes);

    boost::optional<ProcessorPromiseSequence> complete();

private:
    void complete_(ProcessorPromiseAlternativeSequence& sequence);

    bool isMultilingualProcessorPromiseAlternative_(ProcessorPromiseAlternative alt);

    bool checkRequiredTags_(LayerTagList requiredList,
                            ProcessorPromiseAlternativeSequence::iterator beg,
                            ProcessorPromiseAlternativeSequence::iterator end);

    bool isSubset_(LayerTagList sub, LayerTagList sup);

    ProcessorPromiseAlternative findFulfillers_(
        LayerTagList requiredTags, boost::optional<std::string> langCode);

    bool isViciousCircle_(
        AnnotatorFactory* factory,
        LayerTagList requiredTags);

    boost::optional<ProcessorPromiseSharedPtr> getDefaultPromise_(
        AnnotatorFactory& factory, boost::optional<std::string> langCode);

    boost::program_options::variables_map getDefaultOptions_(
        ProcessorFactory& factory);

    boost::program_options::variables_map getDefaultOptionsWithLang_(
        ProcessorFactory& factory, std::string langCode);

    ProcessorPromiseSequence toPromiseSequence_(const ProcessorPromiseAlternativeSequence& seq);

    void trySolution_(const ProcessorPromiseSequence& promiseSequence);

    double calculateQualityScore_(const ProcessorPromiseSequence& promiseSequence);
    double calculateEstimatedTime_(const ProcessorPromiseSequence& promiseSequence);

    ProcessorPromiseAlternative filtreByLanguage_(
        const ProcessorPromiseAlternative& alt, const std::string& langCode);
    boost::optional<ProcessorPromiseSharedPtr> canHandleLanguage_(
        ProcessorPromiseSharedPtr promise, const std::string& langCode);

    std::string processorPromiseAlternative2String_(ProcessorPromiseAlternative alt);

    const ProcessorPromiseAlternativeSequence& originalSequence_;
    std::list<std::string> langCodes_;

    boost::optional<ProcessorPromiseSequence> bestFound_;
    boost::optional<double> bestQualityScore_;
    boost::optional<double> bestEstimatedTime_;
};

#endif
