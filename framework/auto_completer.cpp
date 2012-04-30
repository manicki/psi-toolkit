#include "auto_completer.hpp"

#include <assert.h>
#include <boost/optional.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/scoped_array.hpp>
#include <boost/program_options/parsers.hpp>

#include "annotator_promise.hpp"
#include "logging.hpp"
#include "main_factories_keeper.hpp"

AutoCompleter::AutoCompleter(
    const ProcessorPromiseAlternativeSequence& sequence,
    std::list<std::string> langCodes)
    :originalSequence_(sequence), langCodes_(langCodes) {
}

boost::optional<ProcessorPromiseSequence> AutoCompleter::complete() {
    ProcessorPromiseAlternativeSequence sequence(originalSequence_);

    complete_(sequence);

    return bestFound_;
}

void AutoCompleter::complete_(ProcessorPromiseAlternativeSequence& sequence) {
    bool allFulfilled = true;
    ProcessorPromiseAlternativeSequence::iterator restartPosition = sequence.begin();

    do {
        allFulfilled = true;

        for (ProcessorPromiseAlternativeSequence::iterator iter = restartPosition;
             iter != sequence.end();
             ++iter) {

            INFO("processing alternative: "
                 << processorPromiseAlternative2String_(*iter));

            if (isMultilingualProcessorPromiseAlternative_(*iter)) {
                allFulfilled = false;

                INFO("multiplying by languages");

                bool firstTime = true;

                BOOST_FOREACH(std::string langCode, langCodes_) {
                    INFO("  resolving for " << langCode);
                    ProcessorPromiseAlternative langAlt
                        = filtreByLanguage_(*iter, langCode);

                    assert(!isMultilingualProcessorPromiseAlternative_(langAlt));

                    if (langAlt->empty())
                        return;

                    sequence.insert(iter, langAlt);
                    if (firstTime) {
                        firstTime = false;
                        restartPosition = iter;
                        --restartPosition;
                    }

                    iter = sequence.erase(iter);
                    --iter;
                }

                break;
            } else if ((*iter)->size() > 1) {
                BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *(*iter)) {

                    INFO("trying " << promise->getName());

                    ProcessorPromiseAlternativeSequence newSequence;

                    for (ProcessorPromiseAlternativeSequence::iterator preIter = sequence.begin();
                         preIter != iter;
                         ++preIter)
                        newSequence.push_back(*preIter);

                    newSequence.push_back(
                        ProcessorPromiseAlternative(
                            new std::list<ProcessorPromiseSharedPtr>(1, promise)));

                    ProcessorPromiseAlternativeSequence::iterator postIter = iter;
                    ++postIter;

                    while (postIter != sequence.end()) {
                        newSequence.push_back(*postIter);
                        ++postIter;
                    }

                    complete_(newSequence);
                }

                return;
            } else {
                ProcessorPromiseSharedPtr theOnlyOneProcessor = (*iter)->front();

                boost::optional<LayerTagList> unfulfilledRequiredTags;

                BOOST_FOREACH(LayerTagList requiredTags, theOnlyOneProcessor->requiredLayerTags()) {
                    if (!checkRequiredTags_(requiredTags, sequence.begin(), iter)) {
                        unfulfilledRequiredTags = requiredTags;
                        break;
                    }
                }

                if (unfulfilledRequiredTags) {
                    INFO("trying to find tags: " << boost::algorithm::join(
                             *unfulfilledRequiredTags, ", "));


                    boost::optional<std::string> langCode;

                    if (theOnlyOneProcessor->languagesHandling() == AnnotatorFactory::JUST_ONE_LANGUAGE) {
                        assert(theOnlyOneProcessor->languagesHandled().size() == 1);
                        langCode = theOnlyOneProcessor->languagesHandled().front();
                    }

                    ProcessorPromiseAlternative fulfillers = findFulfillers_(
                        *unfulfilledRequiredTags, langCode);

                    if (fulfillers->empty()) {
                        INFO(" not found!");
                        return;
                    }

                    sequence.insert(iter, fulfillers);
                    restartPosition = iter;
                    --restartPosition;
                    allFulfilled = false;

                    break;
                }
            }
        }

    } while(!allFulfilled);

    trySolution_(toPromiseSequence_(sequence));
}

bool AutoCompleter::isMultilingualProcessorPromiseAlternative_(
    ProcessorPromiseAlternative alt) {

    boost::optional<std::string> theOneLanguage;

    BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *alt) {
        if (promise->languagesHandling() == AnnotatorFactory::LANGUAGE_DEPENDENT)
            return true;
        else if (promise->languagesHandling() == AnnotatorFactory::JUST_ONE_LANGUAGE) {
            assert(promise->languagesHandled().size() == 1);

            if (!theOneLanguage)
                theOneLanguage = promise->languagesHandled().front();
            else if (*theOneLanguage != promise->languagesHandled().front())
                return true;
        }
    }

    return false;
}

bool AutoCompleter::checkRequiredTags_(
    LayerTagList requiredList,
    ProcessorPromiseAlternativeSequence::iterator beg,
    ProcessorPromiseAlternativeSequence::iterator end) {

    ProcessorPromiseAlternativeSequence::iterator iter = beg;
    while (iter != end) {
        assert((*iter)->size() == 1);
        ProcessorPromiseSharedPtr somePrecedingPromise = (*iter)->front();
        if (isSubset_(requiredList, somePrecedingPromise->providedLayerTags()))
            return true;

        ++iter;
    }

    return false;
}

bool AutoCompleter::isSubset_(LayerTagList sub, LayerTagList sup) {
    BOOST_FOREACH(std::string tag, sub) {
        bool tagFound = false;

        BOOST_FOREACH(std::string stag, sup) {
            if (tag == stag) {
                tagFound = true;
                break;
            }
        }

        if (!tagFound)
            return false;
    }

    return true;
}

ProcessorPromiseAlternative AutoCompleter::findFulfillers_(
    LayerTagList requiredTags, boost::optional<std::string> langCode) {
    MainFactoriesKeeper& keeper = MainFactoriesKeeper::getInstance();

    ProcessorPromiseAlternative fulfillersAlternative(
        new std::list<ProcessorPromiseSharedPtr>());

    BOOST_FOREACH(std::string processorName, keeper.getProcessorNames()) {
        ProcessorFactory& factory = keeper.getProcessorFactory(processorName);

        AnnotatorFactory* asAnnotatorFactory =
            dynamic_cast<AnnotatorFactory*>(&factory);

        if (asAnnotatorFactory
            && !isViciousCircle_(asAnnotatorFactory, requiredTags)
            && isSubset_(requiredTags, asAnnotatorFactory->providedLayerTags())) {

            boost::optional<ProcessorPromiseSharedPtr> defaultPromise
                = getDefaultPromise_(*asAnnotatorFactory, langCode);

            if (defaultPromise)
                fulfillersAlternative->push_back(*defaultPromise);
        }
    }

    return fulfillersAlternative;
}

bool AutoCompleter::isViciousCircle_(
    AnnotatorFactory* factory,
    LayerTagList requiredTags) {

    BOOST_FOREACH(LayerTagList itsRequiredTags, factory->requiredLayerTags()) {
        if (isSubset_(requiredTags, itsRequiredTags))
            return true;
    }

    return false;
}

boost::optional<ProcessorPromiseSharedPtr> AutoCompleter::getDefaultPromise_(
    AnnotatorFactory& factory, boost::optional<std::string> langCode) {

    boost::program_options::variables_map defaultOptions = getDefaultOptions_(factory);

    boost::program_options::variables_map optionsToSet = defaultOptions;

    switch (factory.languagesHandling(defaultOptions)) {
    case AnnotatorFactory::LANGUAGE_INDEPENDENT:
    case AnnotatorFactory::LANGUAGE_GUESSER:
        break;
    case AnnotatorFactory::JUST_ONE_LANGUAGE:
        if (langCode && factory.languagesHandled(defaultOptions).front() != *langCode)
            return boost::optional<ProcessorPromiseSharedPtr>();
        break;
    case AnnotatorFactory::LANGUAGE_DEPENDENT:
    {
        bool langFound = false;
        BOOST_FOREACH(std::string handledLangCode, factory.languagesHandled(defaultOptions)) {
            if (langCode && handledLangCode == *langCode) {
                langFound = true;
                optionsToSet = getDefaultOptionsWithLang_(factory, *langCode);
                break;
            }
        }

        if (!langFound)
            return boost::optional<ProcessorPromiseSharedPtr>();
    }
    }

    return
        ProcessorPromiseSharedPtr(
            new AnnotatorPromise(&factory, optionsToSet));
}

boost::program_options::variables_map AutoCompleter::getDefaultOptions_(
    ProcessorFactory& factory) {
    boost::program_options::variables_map options;
    boost::scoped_array<char*> argv(new char* [2]);
    argv[0] = const_cast<char*>("");
    argv[1] = 0;

    boost::program_options::store(
        boost::program_options::parse_command_line(
            1, argv.get(), factory.optionsHandled()),
        options);
    boost::program_options::notify(options);

    return options;
}

boost::program_options::variables_map AutoCompleter::getDefaultOptionsWithLang_(
    ProcessorFactory& factory, std::string langCode) {

    boost::program_options::variables_map options;
    boost::scoped_array<char*> argv(new char* [4]);
    argv[0] = const_cast<char*>("");
    argv[1] = const_cast<char*>("--lang");
    argv[2] = const_cast<char*>(langCode.c_str());
    argv[3] = 0;

    boost::program_options::store(
        boost::program_options::parse_command_line(
            3, argv.get(), factory.optionsHandled()),
        options);
    boost::program_options::notify(options);

    return options;
}


ProcessorPromiseSequence AutoCompleter::toPromiseSequence_(
    const ProcessorPromiseAlternativeSequence& seq) {

    ProcessorPromiseSequence promiseSeq;

    BOOST_FOREACH(ProcessorPromiseAlternative promiseAlt, seq) {
        assert(promiseAlt->size() == 1);

        promiseSeq.push_back(promiseAlt->front());
    }

    return promiseSeq;
}

void AutoCompleter::trySolution_(const ProcessorPromiseSequence& promiseSequence) {
    double seqScore = calculateQualityScore_(promiseSequence);
    double seqTime = calculateEstimatedTime_(promiseSequence);

    if (!bestQualityScore_
        || seqScore > *bestQualityScore_
        || (seqScore == *bestQualityScore_ && seqTime < *bestEstimatedTime_)) {
        bestQualityScore_ = seqScore;
        bestEstimatedTime_ = seqTime;
        bestFound_ = promiseSequence;
    }
}

double AutoCompleter::calculateQualityScore_(const ProcessorPromiseSequence& promiseSequence) {
    double sum = 0.0;

    BOOST_FOREACH(ProcessorPromiseSharedPtr promise, promiseSequence) {
        sum += promise->getQualityScore();
    }

    return sum;
}

double AutoCompleter::calculateEstimatedTime_(const ProcessorPromiseSequence& promiseSequence) {
    double sum = 0.0;

    BOOST_FOREACH(ProcessorPromiseSharedPtr promise, promiseSequence) {
        sum += promise->getEstimatedTime();
    }

    return sum;
}

ProcessorPromiseAlternative AutoCompleter::filtreByLanguage_(
    const ProcessorPromiseAlternative& alt, const std::string& langCode) {
    ProcessorPromiseAlternative returnedSeq(
        new std::list<ProcessorPromiseSharedPtr>());

    BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *alt) {
        boost::optional<ProcessorPromiseSharedPtr> returnedPromise
            = canHandleLanguage_(promise, langCode);

        if (returnedPromise)
            returnedSeq->push_back(*returnedPromise);
    }

    return returnedSeq;
}

boost::optional<ProcessorPromiseSharedPtr> AutoCompleter::canHandleLanguage_(
    ProcessorPromiseSharedPtr promise, const std::string& langCode) {
    switch (promise->languagesHandling()) {
    case AnnotatorFactory::LANGUAGE_INDEPENDENT:
    case AnnotatorFactory::LANGUAGE_GUESSER:
        return boost::optional<ProcessorPromiseSharedPtr>(promise);
    case AnnotatorFactory::LANGUAGE_DEPENDENT:
    case AnnotatorFactory::JUST_ONE_LANGUAGE:
        BOOST_FOREACH(std::string handledLangCode, promise->languagesHandled()) {
            if (handledLangCode == langCode) {
                if (promise->languagesHandling() == AnnotatorFactory::LANGUAGE_DEPENDENT)
                    return boost::optional<ProcessorPromiseSharedPtr>(
                        promise->cloneWithLanguageSet(langCode));
                else
                    return boost::optional<ProcessorPromiseSharedPtr>(promise);
            }
        }
        return boost::optional<ProcessorPromiseSharedPtr>();
    default:
        assert(0);
    }

    return boost::optional<ProcessorPromiseSharedPtr>();
}

std::string AutoCompleter::processorPromiseAlternative2String_(ProcessorPromiseAlternative alt) {
    std::string ret;

    bool first = true;

    BOOST_FOREACH(ProcessorPromiseSharedPtr promise, *alt) {
        if (first)
            first = false;
        else
            ret += std::string("/");

        ret += promise->getName();
    }

    return ret;
}
