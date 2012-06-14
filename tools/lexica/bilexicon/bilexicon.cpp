#include "bilexicon.hpp"
#include "config.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>

#include "bi_lang_specific_processor_file_fetcher.hpp"
#include "bi_language_dependent_annotator_factory.hpp"
#include "its_data.hpp"

BiLexicon::BiLexicon(const boost::program_options::variables_map& options) {
    std::string lang = options["lang"].as<std::string>();
    langCode_ = lang;

    std::string trg_lang = options["trg-lang"].as<std::string>();

    createTags_(trg_lang);

    BiLangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang, trg_lang);

    if (options.count("plain-text-lexicon") > 0) {
        if (options.count("binary-lexicon") > 0
            && options["binary-lexicon"].as<std::string>() != DEFAULT_BINARY_LEXICON_SPEC)
            throw new Exception(
                "either --plain-text-lexicon or --binary-lexicon expected, not both");

        boost::filesystem::path plainTextLexiconPath =
            fileFetcher.getOneFile(
                options["plain-text-lexicon"].as<std::string>());

        readPlainText_(plainTextLexiconPath);
    } else if (options.count("binary-lexicon") > 0) {
        boost::filesystem::path binaryLexiconPath =
            fileFetcher.getOneFile(
                options["binary-lexicon"].as<std::string>());

        loadBinary_(binaryLexiconPath);
    }

    if (options.count("save-binary-lexicon") > 0) {
        if (lexiconBase_.isEmpty())
            throw new Exception("no data to save");

        boost::filesystem::path binaryLexiconPath(
            options["save-binary-lexicon"].as<std::string>());

        saveBinary_(binaryLexiconPath);
    }
}

std::string BiLexicon::getName() {
    return "bilexicon";
}

boost::filesystem::path BiLexicon::getFile() {
    return __FILE__;
}

AnnotatorFactory::LanguagesHandling BiLexicon::languagesHandling(
    const boost::program_options::variables_map& options) {

    return LanguageDependentAnnotatorFactory::checkLangOption(options);
}

std::list<std::string> BiLexicon::languagesHandled(
    const boost::program_options::variables_map& options) {

    if (LanguageDependentAnnotatorFactory::checkLangOption(options)
        == AnnotatorFactory::JUST_ONE_LANGUAGE)
        return boost::assign::list_of(options["lang"].as<std::string>());

    std::string trgLang = options["trg-lang"].as<std::string>();

    std::string fileSuffix = trgLang + ".bin";

    std::vector<std::string> langs;

    boost::filesystem::path dataDirectory = getItsData(getFile());

    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator fiter(dataDirectory);
         fiter != end_iter;
         ++fiter) {
            boost::filesystem::path seg(fiter->path().filename());
            std::string lexiconFileName = seg.string();

            if (lexiconFileName.length() > fileSuffix.length()
                && lexiconFileName.substr(
                    lexiconFileName.length() - fileSuffix.length())
                == fileSuffix)
                langs.push_back(lexiconFileName.substr(
                                    0, lexiconFileName.length() - fileSuffix.length()));
    }

    std::sort(langs.begin(), langs.end());

    return std::list<std::string>(langs.begin(), langs.end());
}

boost::program_options::options_description BiLexicon::optionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    BiLanguageDependentAnnotatorFactory::addBiLanguageDependentOptions(desc);

    desc.add_options()
        ("binary-lexicon",
         boost::program_options::value<std::string>()
         ->default_value(DEFAULT_BINARY_LEXICON_SPEC),
         "path to the lexicon in the binary format")
        ("plain-text-lexicon",
         boost::program_options::value<std::string>(),
         "path to the lexicon in the plain text format")
        ("save-binary-lexicon",
         boost::program_options::value<std::string>(),
         "as a side effect the lexicon in the binary format is generated");

    return desc;
}

std::list<std::string> BiLexicon::providedLayerTags() {
    return boost::assign::list_of
        (std::string("lexeme"))
        (std::string("bilexicon"))
        (std::string("!translation"));
}

std::list<std::list<std::string> > BiLexicon::requiredLayerTags() {
    return
        boost::assign::list_of(
            boost::assign::list_of(std::string("lexeme")));
}

std::list<std::string> BiLexicon::tagsToOperateOn() {
    return boost::assign::list_of
        (std::string("lexeme"))(LayerTagManager::getLanguageTag(langCode_));
}

void BiLexicon::processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge) {
    std::string edgeText = lattice.getAnnotationText(edge);

    std::vector<std::string> records = lexiconBase_.getRecords(edgeText);

    BOOST_FOREACH(std::string& record, records) {
        addEntry_(lattice, edge, record);
    }
}

void BiLexicon::addEntry_(
    Lattice& lattice, Lattice::EdgeDescriptor edge, const std::string& record) {

    LayerTagCollection tags = lattice.getLayerTagManager().createTagCollectionFromList(
        tags_);

    Lattice::EdgeSequence::Builder builder(lattice);
    builder.addEdge(edge);

    lattice.addEdge(
        lattice.getEdgeSource(edge),
        lattice.getEdgeTarget(edge),
        parseRecord_(record),
        tags,
        builder.build());
}

AnnotationItem BiLexicon::parseRecord_(const std::string& record) {
    size_t separatorPos = record.find_first_of(LEMMA_CATEGORY_SEPARATOR);

    std::string category =
        (separatorPos == std::string::npos
         ? record
         : record.substr(separatorPos + 1));

    return AnnotationItem(category, StringFrag(record));
}

void BiLexicon::readPlainText_(const boost::filesystem::path& plainTextLexicon) {
    lexiconBase_.readPlainText(plainTextLexicon);
}

void BiLexicon::saveBinary_(const boost::filesystem::path& binaryLexiconPath) {
    lexiconBase_.saveBinary(binaryLexiconPath);
}

void BiLexicon::loadBinary_(const boost::filesystem::path& binaryLexiconPath) {
    lexiconBase_.loadBinary(binaryLexiconPath);
}

void BiLexicon::createTags_(const std::string& trg_lang) {
    tags_ = providedLayerTags();
    tags_.push_back(LayerTagManager::getLanguageTag(trg_lang));
}

const std::string BiLexicon::DEFAULT_BINARY_LEXICON_SPEC
= "%ITSDATA%/%LANG%%TRGLANG%.bin";
