
#include "rule_loader.hpp"

#include <iostream>
#include <fstream>
#include <locale>
#include <algorithm>
#include <cctype>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

RuleLoader::RuleLoader() {
    rules = RulesPtr( new Rules());

    initProperties();
}

RuleLoader::~RuleLoader() { }

        void RuleLoader::initProperties() {
            verbose = false;
            syntok = false; //czy true?
        }

void RuleLoader::setTagset(TagsetPtr aTagset) {
    tagset = aTagset;
}

RulesPtr RuleLoader::readFromFile(std::string &filename) {
    Pattern regComment("#.*");
    Pattern regWhite("\\s+");

    rules->clear();

    std::ifstream ruleFile(filename.c_str());
    if (!ruleFile) {
        std::cerr << "Could not open rules file: " <<
            filename << "." << std::endl;
        return rules;
    }

    std::string ruleString = "";

    while (!ruleFile.eof()) {
        std::string line;
        getline(ruleFile, line);

        RegExp::Replace(&line, regComment, "");
        if (line == "")
            continue;
        if (RegExp::FullMatch(line, regWhite)) {
            continue;
        }

        line = boost::algorithm::trim_copy(line);
        if (line.find("Rule") == 0) {
            if (ruleString != "") {
                bool wrong = false;
                RulePtr rule = parseRuleString(ruleString, wrong);
                if (!wrong)
                    rules->push_back(rule);
                ruleString = line;
            }
        } else {
            if (ruleString != "")
                ruleString += "\n";
            ruleString += line;
        }
    }
    if (ruleString != "") {
        bool wrong = false;
        RulePtr rule = parseRuleString(ruleString, wrong);
        if (! wrong)
            rules->push_back(rule);
    }
    return rules;
}

std::string RuleLoader::compileNonTokens(std::string &matched) {
    Pattern regWhite("\\s+");
    Pattern regSpecialNeg("!(sb|se|ns)");
    Pattern regSpecial("(^|[ \\(\\)\\|])(sb|se|ns)");

    RegExp::GlobalReplace(&matched, regSpecialNeg,
            "((<<t<[^<>]+<[^<>]+<[^>]+>)|(<<g<[^<>+]<[^<>]+<[^<>]+<[^>]+))");
    RegExp::GlobalReplace(&matched, regSpecial, "\\1(<<s<[^<>]+<\\2<>)");
    RegExp::GlobalReplace(&matched, regWhite, "");
    return matched;
}

std::string RuleLoader::compileRuleName(std::string matched) {
    Pattern regRuleName("Rule\\s*\"\\s*(.*)\\s*\"\\s*;?\\s*");
    std::string container;
    if (RegExp::FullMatch(matched, regRuleName, &container)) {
        return container;
    } else {
        std::cerr << "Wrong rule declaration: " << matched << "." << std::endl;
        return "";
    }
}

#if HAVE_RE2
std::string RuleLoader::compileRulePattern(std::string &matched, int &size,
        std::vector<std::string> &tokensPatterns,
        std::vector<std::string> &tokensModifiers,
        std::vector<bool> &tokensRequired,
        std::vector<int> &matchedIndices, int &bracketCount,
        NegativePatternStrings &negativePatterns) {
#else
std::string RuleLoader::compileRulePattern(std::string &matched, int &size,
        std::vector<std::string> &tokensPatterns,
        std::vector<std::string> &tokensModifiers,
        std::vector<bool> &tokensRequired,
        std::vector<int> &matchedIndices, int &bracketCount) {
#endif
    Pattern regMatch("^(Match|Left|Right)\\s*:\\s*");
    Pattern regWhite("\\s+");

    RegExp::Replace(&matched, regMatch, "");

    if ((matched == "") || (RegExp::FullMatch(matched, regWhite)))  {
        std::cerr << "No patterns defined in section." << std::endl;
        return "null";
    }

    if (matched[matched.size() - 1] == ';') {
        matched = matched.substr(0, matched.size() - 1);
    }  else {
        //@todo: tu komunikat, ze blad skladni, nie ma srednika na koncu!
    }

    size = countTokens(matched);
    std::string compiledMatch = "";
    std::string token = "";
    std::string before = "";
    while ((token = getToken(matched, before)) != "") {
#if HAVE_RE2
        std::string compiledToken = compileToken(token, negativePatterns);
#else
        std::string compiledToken = compileToken(token);
#endif

        if (matched != "") {
            if (matched[0] == '+' || matched[0] == '*') {
                compiledToken = "(" + compiledToken + matched[0] + ")";
                matched = matched.substr(1, std::string::npos);
            }
        }

        std::string compiled = compileNonTokens(before) + compiledToken;
        compiledMatch += compiled;
    }
    if (matched != "")
        compiledMatch += compileNonTokens(matched);
    //if (compiledMatch[0] != '(' || compiledMatch[compiledMatch.size() - 1] != ')') {
    //    std::cerr << "DOPISUJE A NAWIASOW MAM: " << bracketCount << std::endl;
        compiledMatch = "(" + compiledMatch + ")";
    //}

    size_t i = 0;
    int mindex = bracketCount;
    std::string s = compiledMatch;
    int brackets = 0;
    int nonMatching = 0;
    while ( (s != "") && (i < s.length()) ) {
        if (s[i] == '(') {
            if ((i > 0) && (s[i - 1] == '\\')) {
                i ++;
                continue;
            }
            if ( ((i+1) < s.length()) && (s[i + 1] == '?')) {
                nonMatching ++;
                i ++;
                continue;
            }
            if (brackets == 0) {
                matchedIndices.push_back(mindex);
            }
            mindex ++;
            brackets ++;
        } else if (s[i] == ')') {
            if ((i > 0) && (s[i - 1] == '\\'))
            {
                i ++;
                continue;
            }
            if (nonMatching > 0) {
                nonMatching --;
                i ++;
                continue;
            }

            brackets --;
            if (brackets == 0) {
                std::string pattern = s.substr(0, i + 1);
                if ((i + 1) < s.length())
                    s = s.substr(i + 1, std::string::npos);
                else
                    s = "";
                char tested;
                if (s != "" && (s[0] == '?' || s[0] == '*' || s[0] == '+') ) {
                    tested = s[0];
                } else {
                    std::string pattern_test =
                        pattern.substr(1, pattern.length() - 2);
                    tested = pattern_test[pattern_test.length() - 1];
                }
                if (tested == '?' || tested == '*' || tested == '+') {
                    std::string tmp = "";
                    tmp += tested;
                    tokensModifiers.push_back(tmp);
                    if (tested != '+')
                        tokensRequired.push_back(false);
                    else
                        tokensRequired.push_back(true);
                    if (s != "" && s[0] == tested) {
                        s = s.substr(1, std::string::npos);
                    }
                } else {
                    tokensModifiers.push_back("");
                    tokensRequired.push_back(true);
                }
                tokensPatterns.push_back(pattern);
                i = 0;
                continue;
            }
        }
        i ++;
    }

    bracketCount = mindex;
    return compiledMatch;
}

std::string RuleLoader::getToken(std::string &matched, std::string &before) {
    std::string opening = "[";
    std::string::iterator opening_begin_it = opening.begin();
    std::string::iterator opening_end_it = opening.end();
    std::string closing = "]";
    std::string::iterator closing_begin_it = closing.begin();
    std::string::iterator closing_end_it = closing.end();

    int brackets = 0;
    std::string s = "";
    utf8iterator begin(matched.begin(), matched.begin(), matched.end());
    utf8iterator end(matched.end(), matched.begin(), matched.end());
    utf8iterator start = std::find_first_of(begin, end, opening_begin_it,
            opening_end_it);
    if (start == end) { //no [] rule token found
        return "";
    }

    bool found = false;
    utf8iterator it = start;
    while (it != end) {
        if (*it == '[') {
            if (it != begin) {
                utf8iterator prev_it = it;
                prev_it --;
                if (*prev_it != '\\') {
                    s = "";
                    brackets ++;
                }
            } else
                brackets ++;
        } else if (*it == ']') {
            if (it != begin) {
                utf8iterator prev_it = it;
                prev_it --;
                if (*prev_it != '\\') {
                    brackets --;
                    if (brackets == 0) {
                        found = true;
                        break;
                    }
                }
            } else {
                brackets --;
                if (brackets == 0) {
                    found = true;
                    break;
                }
            }
        }
        it ++;
    }
    if (found) {
        before = std::string(begin.base(), start.base());
        it ++;
        s = std::string(start.base(), it.base());
        matched = std::string(it.base(), end.base());
        return s;
    } else {
        return "";
    }
}

int RuleLoader::countTokens(std::string &matched) {
    int count = 0;
    int parenthesis = 0;
    int brackets = 0;
    std::string s = "";
    if (matched == "sb" || matched == "se" || matched == "ns" ||
            matched == "!sb" || matched == "!se" || matched == "!ns")
        return 1;

    utf8iterator begin(matched.begin(), matched.begin(), matched.end());
    utf8iterator end(matched.end(), matched.begin(), matched.end());
    utf8iterator it = begin;

    while (it != end) {
        utf8iterator prev_it = it;
        if (it != begin)
            prev_it --;
        if (*it == '[') {
            if (it != begin) {
                if (*prev_it != '\\') {
                    if ((brackets == 0) && (parenthesis == 0))
                        if ((s == "ns") || (s == "sb") || (s == "se") ||
                                (s == "!ns") || (s == "!sb") || (s == "!se"))
                            count ++;
                    s = "";
                    brackets ++;
                }
            } else
                brackets ++;
        } else if (*it == '(') {
            if (it != begin) {
                if (*prev_it != '\\') {
                    if ((brackets == 0) && (parenthesis == 0))
                        if ((s == "ns") || (s == "sb") || (s == "se") ||
                                (s == "!ns") || (s == "!sb") || (s == "!se"))
                            count ++;
                    s = "";
                    parenthesis ++;
                }
            } else
                parenthesis ++;
        } else if (*it == ']') {
            if (it != begin) {
                if (*prev_it != '\\') {
                    brackets --;
                    if ((brackets == 0) && (parenthesis == 0))
                        count ++;
                }
            } else {
                brackets --;
                if ((brackets == 0) && (parenthesis == 0))
                    count ++;
            }
        } else if (*it == ')') {
            if (it != begin) {
                if (*prev_it != '\\') {
                    parenthesis --;
                    if ((brackets == 0) && (parenthesis == 0))
                        count ++;
                }
            } else {
                parenthesis --;
                if ((brackets == 0) && (parenthesis == 0))
                    count ++;
            }
        } else {
            if ((brackets == 0) && (parenthesis == 0)) {
                if (*it == ' ') {
                    if ((s == "ns") || (s == "sb") || (s == "se") ||
                            (s == "!ns") || (s == "!sb") || (s == "!se"))
                        count ++;
                    s = "";
                } else {
                    if ((*it != '!') && (*it != '?') && (*it != '*')
                            && (*it != '+'))
                        s += *it;
                }
            }
        }
        ++ it;
    }
    return count;
}

#if HAVE_RE2
std::string RuleLoader::compileToken(std::string &token,
        NegativePatternStrings &negativePatterns, bool no_prefix) {
#else
std::string RuleLoader::compileToken(std::string &token,
        bool no_prefix) {
#endif
    if ((token[0] == '[') && (token[token.size() - 1] == ']')) {
        token = token.substr(1, token.size() - 2);
    } else {
        std::cerr << "Illegal token: " << token << std::endl;
        return "";
    }
    bool tokenMatch = false;
    std::string orth = "";
    TokenPatternPart emptyPatternPart;
    TokenPattern emptyPattern;
    emptyPattern.parts.assign(tagset->getNumberOfAttributes() + 1,
            emptyPatternPart);
    TokenPatterns tokenPatterns;
    tokenPatterns.push_back(emptyPattern);
    bool groupMatch = false;
    std::string compiledHead = "";
    std::string type = "";

    while (token != "") {
        bool icase = false;
        std::string key = getKey(token);
        std::string compOperator = getOperator(token);
        if ((key != "head") && (key != "type")) {
            tokenMatch = true;
            groupMatch = false;
        } else {
            groupMatch = true;
            tokenMatch = false;
        }

        std::string value;
        if (key != "head") {
            icase = false;
            value = getValue(token);
            if ((key == "base") || (key == "orth")) {
                if (token.find("/i") == 0) {
                    token = token.substr(2, std::string::npos);
                    icase = true;
                }
            }
        }

        if (key == "head") {
            std::string head = getHead(token);
            if (head != "[]") {
#if HAVE_RE2
                compiledHead = compileToken(head, negativePatterns, true);
#else
                compiledHead = compileToken(head, true);
#endif
            } else
                compiledHead = "<[^<>]+<[^<>]+<[^>]+>";
        } else if (key == "type") {
            type = value;
        } else if (key == "pos") {
            compilePosCondition(compOperator, value, tokenPatterns);
        } else if (key == "base") {
            compileBaseCondition(compOperator, value, tokenPatterns, icase);
        } else if (key == "orth") {
#if HAVE_RE2
            orth = compileOrthCondition(compOperator, value, icase, negativePatterns);
#else
            orth = compileOrthCondition(compOperator, value, icase);
#endif
            if (orth == "") {
                std::cerr << "EROR!" << std::endl;
            }
        } else {  //attribute condition, e.g. number~"pl"
            compileAttributeCondition(key, compOperator, value, tokenPatterns);
        }
        if (token.find("&&") == 0) { // cut out && operator
            token = token.substr(2, std::string::npos);

        }
    }

#if HAVE_RE2
    std::string compiledToken = generateCompiledTokenString(tokenMatch, type,
            compiledHead, orth, tokenPatterns, negativePatterns, no_prefix);
#else
    std::string compiledToken = generateCompiledTokenString(tokenMatch, type,
            compiledHead, orth, tokenPatterns, no_prefix);
#endif
    return "(" + compiledToken + ")";
}

std::string RuleLoader::getKey(std::string &token) {
    std::string comp_op = "!~=";
    std::string::iterator op_begin_it = comp_op.begin();
    std::string::iterator op_end_it = comp_op.end();
    utf8iterator begin(token.begin(), token.begin(), token.end());
    utf8iterator end(token.end(), token.begin(), token.end());
    utf8iterator it = std::find_first_of(begin, end, op_begin_it, op_end_it);
    utf8iterator prev_it = it;
    if (it != begin)
        prev_it --;
    while ((it != begin)          //not the first code point
            && (it != end)        //not the last code point
            && (*prev_it == '\\') //comparison operator is escaped
          ) {
        it = std::find_first_of(++it, end, op_begin_it, op_end_it);
        prev_it = it;
        prev_it --;
    }
    if (it == end) {
        std::cerr << "Illegal token specification: " << token << std::endl;
        return "";
    }
    std::string key(begin.base(), it.base());
    boost::algorithm::trim(key);
    token = std::string(it.base(), end.base());
    return key;
}

std::string RuleLoader::getValue(std::string &token) {
    size_t start_pos = token.find_first_not_of(" "); //skip spaces at the begining
    if (start_pos == std::string::npos)
        start_pos = 0;
    int quotes = 0;
    bool icase = false;
    utf8iterator begin(token.begin() + start_pos, token.begin(), token.end());
    utf8iterator end(token.end(), token.begin(), token.end());

    utf8iterator it = begin;
    while (it != end) {
        if ( (*it == ' ') || (*it == '&') ) {
            if (quotes == 0) {
                break;
            } else {
                it ++;
                continue;
            }
        }
        if (*it == '"') {
            utf8iterator prev_it = it;
            if (it != begin)
                prev_it --;

            if ( (it == begin)
                    || (*prev_it != '\\') ) {
                if (quotes == 0)
                    quotes ++;
                else
                    quotes --;
            }
        }
        it ++;
    }

    if (it == end) { //if reached the end of the string,
                     //check whether the case-insensitive match modifier is present
       utf8iterator tmp_it = it;
       if (tmp_it != begin)
           tmp_it --;
       if (tmp_it != begin)
           tmp_it --;
       if (tmp_it != begin) {
           std::string tmp_s(tmp_it.base(), it.base());
           if (tmp_s == "/i") {
               it --;
               it --;
               icase = true;
           }
       }
    }

    quotes = 0;
    utf8iterator prev_it = it;
    if (it != begin)
        prev_it --;
    if ( (*begin == '"') && ( (it == begin) || (*prev_it == '"') )  ) //@todo: czy jeden przed it?
        quotes = 1;
    std::string value( begin.base() + quotes, it.base() - quotes );
    if (it != end) {
        int inc = 1;
        if (icase)
            inc --;
        token = std::string( it.base() + inc, end.base() ); //@todo: nie lepiej it zwiekszyc wpierw?
        boost::algorithm::trim(token);
    } else {
        token = "";
    }

    //@todo: moze tu zaistniec potrzeba wyrzucenia z napisu 'value' nadmiarowych znakow '\', ktore
    //sluza do 'eskejpowania' nawiasow i \. tak przynajmniej bylo z boostowymi wyrazeniami reg.

    return value;
}

std::string RuleLoader::getOperator(std::string &token) {
    utf8iterator begin(token.begin(), token.begin(), token.end());
    utf8iterator end(token.end(), token.begin(), token.end());
    utf8iterator it = begin;
    while (it != end) {   //find the position of the code point
                          //following the comparison operator
        if ( (*it != '~') && (*it != '=') && (*it != '!') )
            break;
        it ++;
    }
    if (it == end) {
        std::cerr << "Illegal token specification: " << token << std::endl;
        return "";
    }
    std::string op(begin.base(), it.base());
    token = std::string( it.base(), end.base() );
    boost::algorithm::trim(token);
    return op;
}

void RuleLoader::compilePosCondition(std::string &comparisonOperator,
        std::string &value, TokenPatterns &tokenPatterns) {
    std::string posString;
    if (value.find_first_of(".|*+?[]") != std::string::npos) {
        std::vector<std::string> values = tagset->getPosMatching(value);
        if (values.empty()) {
            std::cerr << "No parts of speech identifiers matching regular expression: \""
                << value << "\"." << std::endl;
            return;
        }
        posString = boost::join(values, "|");
        posString = "(" + posString + ")";
    } else {
        if (!tagset->checkPos(value)) {
            std::cerr << "Unknown part of speech: " << value << "." << std::endl;
            return;
        }
        posString = value;
    }
    addConditionToPatterns(tokenPatterns, posString, 0, comparisonOperator);
}

void RuleLoader::compileBaseCondition(std::string &comparisonOperator,
                    std::string &value, TokenPatterns &tokenPatterns,
                    bool icase) {
    value = escapeSpecialChars(value);

    if (icase) {
        value = "(?i:" + value + ")";
    }

    if (value.find_first_of(".|*+?") != std::string::npos){
        std::string tmpVal = "";
        size_t idx = 0;
        size_t pos;
        while ((pos = value.find(".", idx)) != std::string::npos) {
            if (pos > 0)
                if (value[pos - 1] == '\\')
                {
                    tmpVal += value.substr(idx, pos - idx);
                    tmpVal += ".";
                    idx = pos + 1;
                    continue;
                }
            tmpVal += value.substr(idx, pos - idx);
            tmpVal += "[^<>]";
            idx = pos + 1;;
        }
        if (idx < value.length())
            tmpVal += value.substr(idx, std::string::npos);
        value = tmpVal;
    }

    //@todo: don't know what does this part do, so comment it out temporarily
    //Pattern regLpar("\\\\\\\\\\\\\\(");
    //Pattern regRpar("\\\\\\\\\\\\\\)");
    //RegExp::GlobalReplace(&value, regLpar, "\\&#40;");
    //RegExp::GlobalReplace(&value, regRpar, "\\&#41;");

    if (value.find("|") != std::string::npos) {
        value = "(" + value + ")";
    }
    addConditionToPatterns(tokenPatterns, value, -1, comparisonOperator);
}

void RuleLoader::compileAttributeCondition(std::string &key,
        std::string &comparisonOperator, std::string &value,
        TokenPatterns &tokenPatterns) {
    int attributeIndex = tagset->getAttributeIndex(key);
    if (attributeIndex == -1) {
        std::cerr << "No such attribute: " << key << "." << std::endl;
        return;
    }
    attributeIndex += 1; //part of speech preceds attributes in token pattern parts vector
    std::string attributeString;
    if (value.find_first_of(".|*+?") != std::string::npos) {
        std::vector<std::string> values =
            tagset->getAttributeValuesMatching(key, value);
        if (values.empty()) {
            std::cerr << "No values of " << key <<
                " matching regular expression: '" << value << "'." << std::endl;
            return;
        }
        attributeString = boost::algorithm::join(values, "|");
    } else {
        attributeString = value;
    }
    addConditionToPatterns(tokenPatterns, attributeString, attributeIndex,
            comparisonOperator);
}

#if HAVE_RE2
std::string RuleLoader::compileOrthCondition(std::string &comparisonOperator,
        std::string &value, bool icase, NegativePatternStrings &negativePatterns) {
#else
std::string RuleLoader::compileOrthCondition(std::string &comparisonOperator,
        std::string &value, bool icase) {
#endif
    value = escapeSpecialChars(value);

    if (icase) {
        value = "(?i:" + value + ")";
    }

    if (value.find_first_of(".|*+?") != std::string::npos)
    {
        std::string tmpVal = "";
        size_t idx = 0;
        size_t pos;
        while ((pos = value.find(".", idx)) != std::string::npos)
        {
            if (pos > 0)
                if (value[pos - 1] == '\\')
                {
                    tmpVal += value.substr(idx, pos - idx);
                    tmpVal += ".";
                    idx = pos + 1;
                    continue;
                }
            tmpVal += value.substr(idx, pos - idx);
            tmpVal += "[^<>]";
            idx = pos + 1;;
        }
        if (idx < value.length())
            tmpVal += value.substr(idx, std::string::npos);
        value = tmpVal;
    }

    //@todo: don't know what does this part do, so comment it out temporarily
    //Pattern regLpar("\\\\\\\\\\\\\\(");
    //Pattern regRpar("\\\\\\\\\\\\\\)");
    //RegExp::GlobalReplace(&value, regLpar, "\\&lpar;");
    //RegExp::GlobalReplace(&value, regRpar, "\\&rpar;");

    std::string result;
    if ((comparisonOperator == "~") || (comparisonOperator == "~~"))
        result = value;
    else if (comparisonOperator == "!~") {
#if HAVE_RE2
        result = "(?P<";
        std::string negativePatternName = "neg";
        negativePatternName += boost::lexical_cast<std::string>(
                negativePatterns.size() );
        result += negativePatternName;
        result += ">";
        result += "[^<>]+";
        result += ")";
        negativePatterns.insert(
                std::pair<std::string, std::string>(
                    negativePatternName, value
                    ) );
#else
        result = "(?!" + value + ")[^<>]+";
#endif
    }
    else
    {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return "";
    }
    return result;
}

#if HAVE_RE2
std::string RuleLoader::generateTokenPatternsString(TokenPatterns tokenPatterns,
        NegativePatternStrings &negativePatterns) {
#else
std::string RuleLoader::generateTokenPatternsString(TokenPatterns tokenPatterns) {
#endif
    std::stringstream ss;
    for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
            patternIt != tokenPatterns.end(); ++ patternIt) {
        std::string basePattern = "[^<>]+";
        std::string morphoPattern = "";
        std::string pattern = "";
        if (patternIt->base.condition != "") {
            if (patternIt->base.negative) {
#if HAVE_RE2
                basePattern = "(?P<";
                std::string negativePatternName = "neg";
                negativePatternName += boost::lexical_cast<std::string>(
                        negativePatterns.size() );
                basePattern += negativePatternName;
                basePattern += ">";
                basePattern += "[^<>]+";
                basePattern += ")";
                negativePatterns.insert(
                        std::pair<std::string, std::string>(
                            negativePatternName, patternIt->base.condition
                            ) );
#else
                basePattern = "(?!" + patternIt->base.condition + ")[^<>]+";
#endif
            } else {
                basePattern = patternIt->base.condition; //@todo: nie trzeba owinac w nawiasy jeszcze tego?
            }
        }
        for (std::vector<TokenPatternPart>::iterator partIt =
                patternIt->parts.begin();
                partIt != patternIt->parts.end(); ++ partIt) {
            if (partIt->condition != "") {
                if (morphoPattern != "") {
                    morphoPattern += "(:[^:<>]+)*";
                } else {
                    if (partIt != patternIt->parts.begin()) //if not the first part = not the part of speech
                        morphoPattern = "[^:<>]+";
                }
                if (partIt->negative) {
#if HAVE_RE2
                    if (partIt == patternIt->parts.begin() ) //first part = part of speech
                        morphoPattern = "(?P<";
                    else
                        morphoPattern += ":(?P<"; //@todo: jak sie da :?(?P< to wtedy lapie takie, ktore nie maj danego atrybutu w ogole. ta wersja z obowiazkowym : wymusza wartosc atrybutu. pytanie, ktore dzialanie jest podejrzane. moze ustawic to po prostu jako parametr parsera, tak jak 'null agreement' dla unify czy etykiete dla nierozpoznanej czesci mowy ('ign')
                    std::string negativePatternName = "neg";
                    negativePatternName += boost::lexical_cast<std::string>(
                            negativePatterns.size() );
                    morphoPattern += negativePatternName;
                    morphoPattern += ">";
                    morphoPattern += "[^<>]+";
                    morphoPattern += ")";
                    negativePatterns.insert(
                            std::pair<std::string, std::string>(
                                negativePatternName, partIt->condition
                                ) );
#else
                    if (partIt == patternIt->parts.begin() ) //first part = part of speech
                        morphoPattern = "(?!";
                    else
                        morphoPattern += ":(?!"; //@todo: jak sie da :?(?P< to wtedy lapie takie, ktore nie maj danego atrybutu w ogole. ta wersja z obowiazkowym : wymusza wartosc atrybutu. pytanie, ktore dzialanie jest podejrzane. moze ustawic to po prostu jako parametr parsera, tak jak 'null agreement' dla unify czy etykiete dla nierozpoznanej czesci mowy ('ign')
                    morphoPattern += partIt->condition;
                    morphoPattern += ")[^<>]+";
#endif
                } else {
                    morphoPattern += partIt->condition; //@todo: nie trzeba owinac w nawiasy jeszcze tego?
                }
            }
        }
        if (morphoPattern == "")
            morphoPattern = "[^<>]+";
        else
            morphoPattern += "(:([^:<>]+))*";
        pattern += "<";
        pattern += basePattern;
        pattern += "<";
        pattern += morphoPattern;
        if (patternIt->modifier != "") {
            if (patternIt->modifier == "()")
                pattern = "(" + pattern + ")";
            else if (patternIt->modifier == "*")
                pattern = "(" + pattern + ")*";
            else if (patternIt->modifier == "+")
                pattern = "(" + pattern + ")+";
            else if (patternIt->modifier == "?")
                pattern = "(" + pattern + ")?";
        }
        ss << pattern;
    }
    std::string result = ss.str();
    if (tokenPatterns.size() == 1) {
        result = "(" + result + ")";
        //make (<[^<>]+<[^<>]+)* of the (<[^<>]+<[^<>]+) pattern
        //without the final asterisk, the pattern would only match tokens
        //with single interpretations
        if (result == "(<[^<>]+<[^<>]+)")
            result += "*";
    }
    return result;
}

ActionsPtr RuleLoader::compileRuleAction(std::string &matched, int ruleLeftSize,
        int ruleMatchSize, int, std::string ruleName, bool &repeat) {
    Pattern regEval("^Eval\\s*:\\s*");
    // group(NP, 1);
    Pattern regGroup("group\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //delete(pos!~"adj", 1)
    Pattern regDelete("delete\\s*\\(\\s*([^,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //add("adj:sg:m", "new_base", 1);
    Pattern regAdd("add\\s*\\(\\s*\"([^\\s\",]+)\"\\s*,\\s*((\"[^\\s\",]+\")|(base))\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //unify(gender number case, 1, 2, 3);
    Pattern regUnify("unify\\s*\\(\\s*([^,]+)\\s*((,\\s*(\\d+)\\s*)+)\\)\\s*");
    //syntok("coup d'état");
    Pattern regSyntok("syntok\\s*\\(\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
    //join(NP, 1);
    Pattern regJoin("join\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //attach(NP, 1);
    Pattern regAttach("attach\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //transform(1, VP);
    Pattern regTransform("transform\\s*\\(\\s*(\\d+)\\s*,\\s*([^\\s,]+)\\s*\\)\\s*");
    //repeat
    Pattern regRepeat("repeat");
    Pattern regWhite("\\s+");
    std::string actionSeparator = ";";

    RegExp::Replace(&matched, regEval, "");

    ActionsPtr actions = ActionsPtr( new Actions() );

    if ((matched == "") || (RegExp::FullMatch(matched, regWhite))) {
        std::cerr << "No actions defined in Eval section." << std::endl;
        return actions;
    }

    int ruleMatchWidth = ruleLeftSize + ruleMatchSize - 1;

    std::vector<std::string> actionStrings;
    boost::split(actionStrings, matched, boost::is_any_of(actionSeparator));
    for (std::vector<std::string>::iterator asit = actionStrings.begin();
            asit != actionStrings.end(); ++ asit) {
        std::string actionString = boost::algorithm::trim_copy(*asit);
        if ((actionString == "") || (RegExp::FullMatch(actionString, regWhite)))
            continue;
        std::string groupType;
        int groupHead;
        int tokenIndex;
        std::string conditionsString;
        std::string attrs;
        std::string toks;
        std::string baseString;
        std::string interpretation;
        std::string mask;
        std::string slot1, slot2; //@todo: typy!
        if (RegExp::FullMatch(actionString, regRepeat)) {
            repeat = true;
        } else if (RegExp::FullMatch(actionString, regGroup, &groupType, &groupHead)) {
            GroupActionPtr action = GroupActionPtr( new GroupAction(
                        groupType, ruleLeftSize, ruleMatchWidth,
                        groupHead, ruleName ) );
            actions->push_back(action);
        } else if (RegExp::FullMatch(actionString, regJoin, &groupType, &groupHead)) {
            JoinActionPtr action = JoinActionPtr( new JoinAction(
                        groupType, ruleLeftSize, ruleMatchWidth,
                        groupHead, ruleName ) );
            actions->push_back(action);
        } else if (RegExp::FullMatch(actionString, regAttach, &groupType, &groupHead)) {
            AttachActionPtr action = AttachActionPtr( new AttachAction(
                        groupType, ruleLeftSize, ruleMatchWidth,
                        groupHead, ruleName ) );
            actions->push_back(action);
        } else if (RegExp::FullMatch(actionString, regTransform, &groupHead, &groupType)) {
            TransformActionPtr action = TransformActionPtr( new TransformAction(
                        groupType, groupHead, ruleName ) );
            actions->push_back(action);
        } else if (RegExp::FullMatch(actionString, regDelete,
                    &conditionsString, &tokenIndex)) {
            DeleteActionPtr action = createDeleteAction(conditionsString, tokenIndex);
            actions->push_back(action);
        } else if (RegExp::FullMatch(actionString, regAdd, &interpretation,
                    &baseString, &slot1, &slot2, &tokenIndex)) { //@todo: dlaczemu nie mozna uzyc NULL zamiast slotX?
            AddActionPtr action = createAddAction(interpretation, baseString,
                    tokenIndex);
            actions->push_back(action);
        } else if (RegExp::FullMatch(actionString, regUnify, &attrs, &toks)) {
            UnifyActionPtr action = createUnifyAction(attrs, toks);
            actions->push_back(action);
        } else if (RegExp::FullMatch(actionString, regSyntok, &mask)) {
            SyntokActionPtr action = createSyntokAction(mask, ruleLeftSize,
                    ruleMatchSize, ruleName);
            actions->push_back(action);
        } else {
            std::cerr << "Unknown action: " << actionString << "." << std::endl;
        }
    }
    return actions;
}

bool RuleLoader::compileDeleteCondition(std::string &key,
        std::string &comparisonOperator, std::string &value,
        DeleteConditions &conditions) {
    std::string conditionPattern;
    if (key == "pos") {
        std::string regexpString = "";
        if (value.find_first_of(".|*+?") != std::string::npos) {
            std::vector<std::string> values;
            if (comparisonOperator != "!~")
                values = tagset->getPosMatching(value);
            else
                values = tagset->getPosNotMatching(value);
            if (values.empty()) {
                std::cerr << "No parts of speech identifiers matching the regular expression: \""
                    << value << "\"." << std::endl;
                return false;
            }
            regexpString = boost::algorithm::join(values, "|");
        } else {
            if (comparisonOperator != "!~") {
                regexpString = value;
            } else {
                std::vector<std::string> values =
                    tagset->getPosExcept(value);
                regexpString = boost::algorithm::join(values, "|");
            }
        }
        conditionPattern = "^(" + regexpString + ")(:[^:]+)*$";
    } else if (key == "base") {
        conditionPattern = "(" + value + ")";
    } else if (key == "orth") {
        //@todo: od kiedy moze byc delete z warunkiem na postac ortograficzna?
    } else {
        std::string regexpString = "";
        if (value.find_first_of(".|*+?") != std::string::npos) {
            std::vector<std::string> values;
            if (comparisonOperator != "!~")
                values = tagset->getAttributeValuesMatching(key, value);
            else
                values = tagset->getAttributeValuesNotMatching(key, value);
            if (values.empty()) {
                std::cerr << "No values of " << key << " matching the regular expression: \""
                    << value << "\"." << std::endl;
                return false;
            }
            regexpString = boost::algorithm::join(values, "|");
        } else {
            if (comparisonOperator != "!~") {
                regexpString = value;
            } else {
                std::vector<std::string> values =
                    tagset->getAttributeValuesExcept(key, value);
                regexpString = boost::algorithm::join(values, "|");
            }
        }
        conditionPattern = "^[^:]+(:[^:]+)*:(" + regexpString + ")(:[^:]+)*$";
    }
    DeleteCondition condition;
    if (key == "base")
        condition.type = BASE_CONDITION;
    else if (key == "orth")
        ;//@todo: nie moze byc takiej sytuacji?
    else
        condition.type = MORPHOLOGY_CONDITION;
    if (comparisonOperator == "!~")
        condition.negation = true;
    else
        condition.negation = false;
    condition.pattern = conditionPattern;
    conditions.push_back(condition);

    return true;
}

std::vector<Morphology> RuleLoader::compileAddInterpretation(std::string &pattern) {
    std::vector<Morphology> interpretations;
    std::string fieldSeparator = ":";
    std::vector<std::string> partsVector;
    boost::split(partsVector, pattern, boost::is_any_of(fieldSeparator));
    int index = 0;
    std::string pos;
    std::vector<Morphology> morphologies;
    for (std::vector<std::string>::iterator it = partsVector.begin();
            it != partsVector.end(); ++ it) {
        std::string value = *it;
        if (index == 0) {
            if (tagset->checkPos(value)) {
                Morphology morphology;
                morphology.insert(std::pair<std::string, std::string>(
                            "pos", value));
                morphologies.push_back(morphology);
                //morphologies.push_back(value);
                pos = value;
            } else {
                std::cerr << "Unknown part of speech: " << value
                    << "." << std::endl;
                return interpretations;
            }
        } else {
            std::string attribute = tagset->getAttributeAtIndex(pos, index - 1);
            std::vector<std::string> values;
            if (value == "*")
            {
                values = tagset->getAttributeValues(attribute);
            } else if (value.find(".") != std::string::npos) {
                std::vector<std::string> valuesVector;
                boost::split(valuesVector, value, boost::is_any_of("."));
                for (std::vector<std::string>::iterator valueIt = valuesVector.begin();
                        valueIt != valuesVector.end(); ++valueIt) {
                    if (tagset->checkAttributeValue(attribute, *valueIt))
                        values.push_back(*valueIt);
                    else
                    {
                        std::cerr << "Unknown value: " << *valueIt
                            << " of attribute: " << attribute << "." << std::endl;
                        return interpretations;
                    }
                }
            } else {
                if (tagset->checkAttributeValue(attribute, value))
                    values.push_back(value);
                else {
                    std::cerr << "Unknown value: " << value <<
                        " of attribute: " << attribute << "." << std::endl;
                    return interpretations;
                }
            }
            std::vector<Morphology> newMorphologies;
            for (std::vector<std::string>::iterator valueIt = values.begin();
                    valueIt != values.end(); ++ valueIt) {
                for (std::vector<Morphology>::iterator morphoIt =
                        morphologies.begin();
                        morphoIt != morphologies.end(); ++ morphoIt) {
                    Morphology morphology = *morphoIt;
                    morphology.insert(std::pair<std::string, std::string>(
                                attribute, *valueIt));
                    newMorphologies.push_back(morphology);
                    //newMorphologies.push_back(*morphoIt + ":" + *valueIt);
                }
            }
            morphologies = newMorphologies;
        }
        index ++;
    }

    for (std::vector<Morphology>::iterator m = morphologies.begin();
            m != morphologies.end(); ++ m) {
        std::string morphoString = util::getMorphologyString(*m);
        if (tagset->checkMorphology(morphoString)) {
            interpretations.push_back(*m);
        } else {
            std::cerr << "The morphology is not valid: " << morphoString
                << std::endl;
            return interpretations;
        }
    }
    return interpretations;
}

void RuleLoader::setVerbose() {
    verbose = true;
}

void RuleLoader::setSyntok() {
    syntok = true;
}


RulePtr RuleLoader::compileRule(std::string ruleString) {
    bool wrong = false;
    return parseRuleString(ruleString, wrong);
}

ActionPtr RuleLoader::compileAction(std::string actionString, RulePtr rule) {
    bool repeat = rule->getRepeat();
    ActionsPtr actions = compileRuleAction( actionString, rule->getLeftCount(),
            rule->getMatchCount(), rule->getRightCount(), rule->getName(),
            repeat );
    return actions->front();
}

std::string RuleLoader::getHead(std::string &token) {
    std::string opening = "[";
    std::string::iterator opening_begin_it = opening.begin();
    std::string::iterator opening_end_it = opening.end();
    std::string closing = "]";
    std::string::iterator closing_begin_it = closing.begin();
    std::string::iterator closing_end_it = closing.end();
    std::string quote = "\"";
    std::string::iterator quote_begin_it = quote.begin();
    std::string::iterator quote_end_it = quote.end();
    utf8iterator begin(token.begin(), token.begin(), token.end());
    utf8iterator end(token.end(), token.begin(), token.end());

    utf8iterator head_begin = std::find_first_of(begin, end, opening_begin_it,
            opening_end_it);
    utf8iterator head_end = std::find_first_of(head_begin, end,
            closing_begin_it, closing_end_it);
    utf8iterator quote_it = std::find_first_of(head_begin, end,
            quote_begin_it, quote_end_it);
    if (quote_it != end) {
        utf8iterator head_begin2 = std::find_first_of(head_begin, end,
                opening_begin_it, opening_end_it);
        utf8iterator next = quote_it;
        next ++;
        if (head_begin2.base() > quote_it.base()) {
            quote_it = std::find_first_of(next, end,
                    quote_begin_it, quote_end_it);
            if (quote_it != end) {
                head_end = std::find_first_of(next, end,
                        closing_begin_it, closing_end_it);
            }
        }
    }
    if (head_end != end)
        head_end ++;
    std::string head(head_begin.base(), head_end.base());
    if (head_end != end) {
        token = std::string(head_end.base(), end.base());
    } else {
        token = "";
    }

    return head;
}

//uniform token compiled pattern format
#if HAVE_RE2
std::string RuleLoader::generateCompiledTokenString(bool tokenMatch,
        std::string &type, std::string &compiledHead, std::string &orth,
        TokenPatterns tokenPatterns, NegativePatternStrings &negativePatterns,
        bool no_prefix) {
#else
std::string RuleLoader::generateCompiledTokenString(bool tokenMatch,
        std::string &type, std::string &compiledHead, std::string &orth,
        TokenPatterns tokenPatterns, bool no_prefix) {
#endif
    std::stringstream ss;
    if (! no_prefix) {
        if (tokenMatch)
            ss << "<<t";
        else
            ss << "<<g";
    }
    ss << "<[^<>]+";  //from
    ss << "<[^<>]+";  //to
    if (type != "")
        ss << "<" << type;
    else
        ss << "<[^<>]+";
    if (compiledHead != "")
        ss << compiledHead;
    else {
        //@todo czy trzeba sprawdzic czy to nie group match i wstawic puste dopasowanie? czy sprawdzenie jakiegos warunku tu nie wystarczy?
        if (orth != "")
            ss << "<" << orth;
        else
            ss << "<[^<>]+";
#if HAVE_RE2
        ss << generateTokenPatternsString(tokenPatterns, negativePatterns);
#else
        ss << generateTokenPatternsString(tokenPatterns);
#endif
    }
    ss << ">";

    return ss.str();
}

std::string RuleLoader::escapeSpecialChars(std::string text) {
    Pattern regAmp("&");
    Pattern regLt("<");
    Pattern regGt(">");
    Pattern regLPar("\\\\\\(");
    Pattern regRPar("\\\\\\)");
    Pattern regAlt("\\\\\\|");
    Pattern regPlus("\\\\\\+");
    Pattern regAsterisk("\\\\\\*");
    Pattern regOpt("\\\\\\?");

    RegExp::GlobalReplace(&text, regAmp, "\\&amp;");
    RegExp::GlobalReplace(&text, regLt, "\\&lt;");
    RegExp::GlobalReplace(&text, regGt, "\\&gt;");
    RegExp::GlobalReplace(&text, regLPar, "&#40;");
    RegExp::GlobalReplace(&text, regRPar, "&#41;");
    RegExp::GlobalReplace(&text, regAlt, "\\&#124;");
    RegExp::GlobalReplace(&text, regPlus, "\\&#42;");
    RegExp::GlobalReplace(&text, regAsterisk, "\\&#43;");
    RegExp::GlobalReplace(&text, regOpt, "\\&#63;");

    return text;
}

void RuleLoader::addConditionToPatterns(TokenPatterns &tokenPatterns,
        std::string conditionString, int attributeIndex,
        std::string comparisonOperator) {
    if (comparisonOperator == "~~") {
        for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
                patternIt != tokenPatterns.end(); ++ patternIt) {
            if (attributeIndex == -1) {
                patternIt->base.condition = conditionString;
                patternIt->base.negative = false;
            } else {
                patternIt->parts[attributeIndex].condition = conditionString;
                patternIt->parts[attributeIndex].negative = false;
            }
        }
    } else if (comparisonOperator == "~") {
        if (tokenPatterns.size() == 1) {
            TokenPattern formerPattern = tokenPatterns.front();
            TokenPattern newPattern = formerPattern;
            if (attributeIndex == -1) {
                newPattern.base.condition = conditionString;
                newPattern.base.negative = false;
            } else {
                newPattern.parts[attributeIndex].condition = conditionString;
                newPattern.parts[attributeIndex].negative = false;
            }
            newPattern.modifier = "()";
            formerPattern.modifier = "*";
            tokenPatterns[0] = formerPattern;
            tokenPatterns.push_back(newPattern);
            tokenPatterns.push_back(formerPattern);
        } else {
            if (attributeIndex == -1) {
                tokenPatterns[1].base.condition = conditionString;
                tokenPatterns[1].base.negative = false;
            } else {
                tokenPatterns[1].parts[attributeIndex].condition = conditionString;
                tokenPatterns[1].parts[attributeIndex].negative = false;
            }
        }
    } else if (comparisonOperator == "!~") {
        for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
                patternIt != tokenPatterns.end(); ++ patternIt) {
            if (attributeIndex == -1) {
                patternIt->base.condition = conditionString;
                patternIt->base.negative = true;
            } else {
                patternIt->parts[attributeIndex].condition = conditionString;
                patternIt->parts[attributeIndex].negative = true;
            }
        }
    } else {
        std::cerr << "Unknown comparison operator: " << comparisonOperator
            << "." << std::endl;
        return;
    }
}

DeleteActionPtr RuleLoader::createDeleteAction(std::string &conditionsString,
        int tokenIndex) {
    std::string uncompiled = conditionsString;
    DeleteConditions conditions;

    while (conditionsString != "") {
        std::string key = getKey(conditionsString);
        std::string compOperator = getOperator(conditionsString);
        std::string value = getValue(conditionsString);
        compileDeleteCondition(key, compOperator, value, conditions);
        if (conditionsString.find("&&") == 0) {
            conditionsString = conditionsString.substr(2, std::string::npos);
        }
    }
    DeleteActionPtr action = DeleteActionPtr( new DeleteAction(
                conditions, tokenIndex, uncompiled ) );
    return action;
}

AddActionPtr RuleLoader::createAddAction(std::string &interpretation,
        std::string &baseString, int tokenIndex) {
    std::string base = baseString;
    if (base == "base") {
        base = "[^<>]+";
    } else {
        base = base.substr(1, base.length() - 2); //removes quotes surrounding the base form
    }

    std::vector<Morphology> interpretations =
        compileAddInterpretation(interpretation);

    AddActionPtr action = AddActionPtr( new AddAction(
                interpretations, base, tokenIndex, interpretation ) );
    return action;
}

UnifyActionPtr RuleLoader::createUnifyAction(std::string &attributesString,
        std::string &tokensString) {
    std::vector<std::string> patterns;
    std::vector<std::string> attributes;
    std::vector<int> tokens;
    std::vector<std::string> attributes_;

    std::vector<std::string> attributesVector;
    boost::split(attributesVector, attributesString, boost::is_any_of(" "));
    for (std::vector<std::string>::iterator ait = attributesVector.begin();
            ait != attributesVector.end(); ++ ait) {
        std::string aa = boost::algorithm::trim_copy(*ait);
        if (tagset->checkAttribute(aa)) {
            attributes.push_back(aa);
            std::vector<std::string> values =
                tagset->getAttributeValues(aa);
            std::string valuesString = boost::algorithm::join(values, "|");
            std::string pattern = "^[^:]+(:[^:]+)*:(" + valuesString
                + ")(:[^:]+)*$";
            patterns.push_back(pattern);
            attributes_.push_back(aa);
        } else {
            std::cout << "Unknown attribute: " << aa << "." << std::endl;
        }
    }
    std::string tokenSeparator = ",";
    std::vector<std::string> tokensVector;
    boost::split(tokensVector, tokensString, boost::is_any_of(tokenSeparator));
    for (std::vector<std::string>::iterator tit = tokensVector.begin();
            tit != tokensVector.end(); ++ tit) {
        std::string tokenString = boost::algorithm::trim_copy(*tit);
        if (tokenString != "") {
            tokens.push_back(boost::lexical_cast<int>(tokenString));
        }
    }
    UnifyActionPtr action = UnifyActionPtr( new UnifyAction(
                patterns, attributes, tokens, attributes_ ) );
    return action;
}

SyntokActionPtr RuleLoader::createSyntokAction(std::string &interpretationString,
        int ruleLeftSize, int ruleMatchSize, std::string &ruleName) {
    std::vector<int> tokens;
    for (int x = ruleLeftSize; x < (ruleLeftSize + ruleMatchSize); x ++) {
        tokens.push_back(x + 1);
    }
    std::vector<Morphology> interpretations =
        compileAddInterpretation(interpretationString);

    SyntokActionPtr action = SyntokActionPtr( new SyntokAction(
                ruleLeftSize, ruleLeftSize + ruleMatchSize - 1, tokens,
                interpretations, ruleName, interpretationString ) );
    return action;
}

RulePtr RuleLoader::parseRuleString(std::string &ruleString, bool &wrong) {
    std::string ruleName;
    std::string rulePattern;
    std::string rulePatternLeft, rulePatternMatch, rulePatternRight;
    int ruleLeftSize, ruleMatchSize, ruleRightSize;
    ruleLeftSize = ruleMatchSize = ruleRightSize = 0;
    bool hasLeft = false;
    bool hasRight = false;
    int bracketCount = 0;
    wrong = false;

    std::vector<std::string> tokensPatterns;
    std::vector<std::string> tokensModifiers;
    std::vector<bool> tokensRequired;
    std::vector<int> matchedIndices;

#if HAVE_RE2
    NegativePatternStrings negativePatterns;
#endif
    std::string chars = "";

    rulePattern = "";
    rulePatternLeft = "";
    rulePatternMatch = "";
    rulePatternRight = "";

    std::vector<std::string> linesVector;
    boost::split(linesVector, ruleString, boost::is_any_of("\n"));
    for (std::vector<std::string>::iterator lineIt = linesVector.begin();
            lineIt != linesVector.end(); ++ lineIt) {
        std::string line = *lineIt;
        if (line.find("Rule") == 0) {
            ruleName = this->compileRuleName(line);
        }
        else if (line.find("Left:") == 0) {
            if (chars != "") {
                std::cerr << "Unexpected characters before Left context declaration: "
                    << chars << std::endl;
                chars = "";
            }
            chars += line;
            hasLeft = true;
        } else if (line.find("Match:") == 0) {
            if (chars != "") {
                if (hasLeft) {
                    rulePatternLeft = chars;
#if HAVE_RE2
                    rulePattern = this->compileRulePattern(chars, ruleLeftSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
#else
                    rulePattern = this->compileRulePattern(chars, ruleLeftSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount);
#endif
                    chars = "";
                } else {
                    std::cerr << "Unexpected characters before Match declaration: "
                        << chars << std::endl;
                    chars = "";
                }
            }
            chars += line;
        } else if (line.find("Right:") == 0) {
            if (chars != "") {
                rulePatternMatch = chars;
#if HAVE_RE2
                rulePattern += this->compileRulePattern(chars, ruleMatchSize,
                        tokensPatterns, tokensModifiers, tokensRequired,
                        matchedIndices, bracketCount, negativePatterns);
#else
                rulePattern += this->compileRulePattern(chars, ruleMatchSize,
                        tokensPatterns, tokensModifiers, tokensRequired,
                        matchedIndices, bracketCount);
#endif
                chars = "";
            }
            chars += line;
            hasRight = true;
        } else if (line.find("Eval:") == 0) {
            if (chars != "") {
                if (!hasRight) {
                    rulePatternMatch = chars;
#if HAVE_RE2
                    rulePattern += this->compileRulePattern(chars, ruleMatchSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
#else
                    rulePattern += this->compileRulePattern(chars, ruleMatchSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount);
#endif
                } else {
                    rulePatternRight = chars;
#if HAVE_RE2
                    rulePattern += this->compileRulePattern(chars, ruleRightSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
#else
                    rulePattern += this->compileRulePattern(chars, ruleRightSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount);
#endif
                }
                chars = "";
            }
            chars += line;
        } else {
            chars += line;
        }
    }
    if (bracketCount > 16) {
        wrong = true;
    }
    bool repeat = false;
    ActionsPtr actions = this->compileRuleAction(chars,
            ruleLeftSize, ruleMatchSize, ruleRightSize, ruleName,
            repeat);
#if HAVE_RE2
    RulePtr rule = RulePtr( new Rule(ruleName, rulePattern,
                ruleLeftSize, ruleMatchSize, ruleRightSize,
                actions, tokensPatterns, tokensModifiers,
                tokensRequired, matchedIndices, repeat,
                rulePatternLeft, rulePatternMatch, rulePatternRight,
                negativePatterns) );
#else
    RulePtr rule = RulePtr( new Rule(ruleName, rulePattern,
                ruleLeftSize, ruleMatchSize, ruleRightSize,
                actions, tokensPatterns, tokensModifiers,
                tokensRequired, matchedIndices, repeat,
                rulePatternLeft, rulePatternMatch, rulePatternRight) );
#endif
    return rule;
}


}

}

}

