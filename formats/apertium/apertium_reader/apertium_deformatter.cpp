#include <boost/foreach.hpp>

#include "regexp.hpp"
#include "apertium_deformatter.hpp"
#include "file_recognizer.hpp"
#include "file_extractor.hpp"
#include "logging.hpp"


ApertiumDeformatter::ApertiumDeformatter(
    const boost::filesystem::path& specFilePath,
    bool unzipData)
    : formatSpecification_(initializeFormatSpecification_(specFilePath)),
    initialInputSize_(0), unzipData_(unzipData) {

    perlRegexpOptions_.set_utf8(true);
    perlRegexpOptions_.set_caseless(!formatSpecification_.getOptions().isCaseSensitive());
}

FormatSpecification ApertiumDeformatter::initializeFormatSpecification_(
    const boost::filesystem::path& path) {

    FormatSpecificationReader formatSpecRdr(path);
    return formatSpecRdr.readFormatSpecification();
}

std::string ApertiumDeformatter::deformat(const std::string& input) {
    initialInputSize_ = 0;

    std::string text = processReplacementRules(input);
    std::vector<DeformatIndex> deformatIndexes = processFormatRules(text);
    std::string output = clearFromDeformatData_(text, deformatIndexes);

    return output;
}

std::vector<DeformatIndex> ApertiumDeformatter::processFormatRules(const std::string& input) {
    DEBUG("is compressed format: " << formatSpecification_.getOptions().isCompressed());

    PerlStringPiece currentInput(
        (unzipData_ && formatSpecification_.getOptions().isCompressed()) ?
            decompressFiles_(input) : input);
    initialInputSize_ = currentInput.size();

    std::vector<std::string> regexps = formatSpecification_.formatRulesRegexp();
    std::vector<PerlRegExp> res;

    std::vector<int> levels = formatSpecification_.getLevels();

    const PerlArg *args[levels.size()][FormatSpecification::MAX_RULES_PER_LEVEL];
    std::string matches[levels.size()][FormatSpecification::MAX_RULES_PER_LEVEL];

    for (unsigned int i = 0; i < levels.size(); i++) {
        res.push_back(PerlRegExp(regexps[i], pcrecpp::UTF8()));

        for (int j = 0; j < levels[i]; j++) {
            args[i][j] = new PerlArg(&matches[i][j]);
        }
    }

    std::vector<DeformatIndex> deformatIndexes;

    while (PerlRegExp::FindAndConsumeN(&currentInput, res[0], args[0], levels[0])) {
        unsigned int lvl = 0;
        std::string text = matches[0][levels[lvl] - 1];

        while (!text.empty() && (lvl + 1) < levels.size()) {
            lvl++;
            PerlRegExp::FullMatchN(text, res[lvl], args[lvl], levels[lvl]);
            text = matches[lvl][levels[lvl] - 1];
        }

        PerlRegExp::FullMatchN(text, res[lvl], args[lvl], levels[lvl]);

        for (int i = 0; i < levels[lvl]; i++) {
            if (!matches[lvl][i].empty()) {
                int ruleNr = lvl * FormatSpecification::MAX_RULES_PER_LEVEL + i - lvl;

                deformatIndexes.push_back(DeformatIndex(
                    getMatchedStringIndexes_(currentInput, matches[lvl][i]),
                    formatSpecification_.getFormatRule(ruleNr).getType(),
                    formatSpecification_.getFormatRule(ruleNr).getEos()
                ));

                DEBUG("rule " << ruleNr << " found " << matches[lvl][i]);
            }
        }
    }


    return deformatIndexes;
}

std::string ApertiumDeformatter::processReplacementRules(const std::string& input) {
    std::string text = input;

    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string> rules = formatSpecification_.replacementRulesRegexp();

    for (it = rules.begin(); it != rules.end(); ++it) {
        PerlStringPiece target(it->first);
        PerlRegExp re(it->second, perlRegexpOptions_);
        PerlRegExp::GlobalReplace(&text, re, target);
    }

    return text;
}

std::string ApertiumDeformatter::clearFromDeformatData_(const std::string& input,
    std::vector<DeformatIndex>& indexes) {

    std::string clearInput = input;

    int length = 0;
    for (unsigned int i = 0; i < indexes.size(); i++) {
        clearInput.erase(indexes[i].begin - length, indexes[i].length());
        length += indexes[i].length();
    }

    return clearInput;
}

std::pair<int, int> ApertiumDeformatter::getMatchedStringIndexes_(
     PerlStringPiece currentInput, std::string matchedString) {

    return std::pair<int, int>(initialInputSize_ - currentInput.size() - matchedString.size(),
        initialInputSize_ - currentInput.size());
}

std::string ApertiumDeformatter::decompressFiles_(const std::string &compressed) {
    DEBUG("apertium-reader file decompression...");

    FileRecognizer fileRecognizer;
    std::string filetype = fileRecognizer.recognizeCompressedFileFormat(compressed);

    typedef std::map<std::string, std::string> string_to_string_map;

    FileExtractor fileExtractor;
    string_to_string_map files = fileExtractor.extractFilesFromData(
        compressed,
        formatSpecification_.getOptions().getCompressionRegexp());

    std::string decompressed;

    BOOST_FOREACH(const string_to_string_map::value_type& fileAndContent, files) {
        decompressed += fileAndContent.second;
        DEBUG("apertium-reader decompresses file: " << fileAndContent.first);
    }

    return decompressed;
}

void ApertiumDeformatter::setUnzipData(bool onOff) {
    unzipData_ = onOff;
}
