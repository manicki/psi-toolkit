
#include "rule_loader.hpp"

#include <iostream>
#include <fstream>
#include <locale>
#include <algorithm>
#include <cctype>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
//#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
//#define BOOST_UTF8_BEGIN_NAMESPACE namespace mynamespace {
//#define BOOST_UTF8_END_NAMESPACE }
//#define BOOST_UTF8_DECL

//#include <boost/detail/utf8_codecvt_facet.hpp>

//#include "icu_converter.hpp"
#include <boost/algorithm/string.hpp>
//#include <unicode/schriter.h>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

RuleLoader::RuleLoader() {
//    tagset = new Tagset;
//    rules = new Rules;
//    rules->clear();
    rules = RulesPtr( new Rules());

    initProperties();
    initPatterns();
//    verbose = false;
//
//    regAmp = "&";
//    regLt = "<";
//    regGt = ">";
//    regLPar = "\\\\\\(";
//    regRPar = "\\\\\\)";
//    regAlt = "\\\\\\|";
//    regPlus = "\\\\\\+";
//    regAsterisk = "\\\\\\*";
//    regOpt = "\\\\\\?";
//
//    syntok = false; //czy true?
}

/*RuleLoader::RuleLoader(std::string &filename) {
//    verbose = false;
    std::string tagsetFilename = "tagset.pl.cfg"; //TODO: to jakos parser laduje, albo cos?
    //tagset = new Tagset(tagsetFilename);
//    rules = new Rules;
//    rules->clear();
    numAttributes = tagset->getNumberOfAttributes();
    baseMask.assign(numAttributes + 3, ".");
//    baseMask.assign(numAttributes + 3, "[^<>]");
    baseIndex = numAttributes + 1;
    baseMask[baseIndex] = "[^<>]+";
    modifierIndex = numAttributes + 2;
    baseMask[modifierIndex] = "";
    nothingSet.assign(numAttributes + 1, '.');
    nothingSet += "[^<>]+";
    int i = 0;
    std::string tmpNothing;
    while (i < numAttributes + 1)
    {
        tmpNothing += "\\.";
//        tmpNothing += "\\[\\^<>\\]";
        i ++;
    }
    tmpNothing += "\\[\\^<>\\]\\+";
    regNothingSet = boost::make_u32regex(tmpNothing);
    //regNothingSet = boost::make_u32regex(utf8converter::ToUtf8(utf8converter::FromUtf8(tmpNothing)));
    //64: regNothingSet = boost::make_u32regex(tmpNothing);
    //rules = this->readFromFile(filename);

    regAmp = "&";
    regLt = "<";
    regGt = ">";
    regLPar = "\\(";
    regRPar = "\\)";
    regAlt = "\\|";
    regPlus = "\\+";
    regAsterisk = "\\*";
    regOpt = "\\?";

//    syntok = false; //czy true?
}*/

RuleLoader::~RuleLoader() {
//    delete tagset;
    //for (Rules::iterator r = rules->begin(); r!=rules->end(); r++)
    //    delete *r;
//    rules->clear();
    //delete rules;
}

        void RuleLoader::initProperties() {
            verbose = false;
            syntok = false; //czy true?
        }

        void RuleLoader::initPatterns() {
            regAmp = PatternPtr(new RE2("&"));
            regLt = PatternPtr(new RE2("<"));
            regGt = PatternPtr(new RE2(">"));
            regLPar = PatternPtr(new RE2("\\\\\\("));
            regRPar = PatternPtr(new RE2("\\\\\\)"));
            regAlt = PatternPtr(new RE2("\\\\\\|"));
            regPlus = PatternPtr(new RE2("\\\\\\+"));
            regAsterisk = PatternPtr(new RE2("\\\\\\*"));
            regOpt = PatternPtr(new RE2("\\\\\\?"));

        }

void RuleLoader::setTagset(TagsetPtr aTagset)
{
    //delete tagset;
    //tagset = new Tagset(aTagset);
    tagset = aTagset;
    numAttributes = tagset->getNumberOfAttributes();
    //baseMask.assign(numAttributes + 3, ".");
//    baseMask.assign(numAttributes + 3, "[^<>]");
    //baseIndex = numAttributes + 1;
    //baseMask[baseIndex] = "[^<>]+";
    //modifierIndex = numAttributes + 2;
    //baseMask[modifierIndex] = "";
    //nothingSet.assign(numAttributes + 1, '.');
    //nothingSet += "[^<>]+";
    //int i = 0;
    //std::string tmpNothing;
    //while (i < numAttributes + 1)
    //{
    //    tmpNothing += "\\.";
//        tmpNothing += "\\[\\^<>\\]";
    //    i ++;
    //}
    //tmpNothing += "\\[\\^<>\\]\\+";
    //regNothingSet = boost::make_u32regex(tmpNothing);
    //regNothingSet = PatternPtr(new RE2(tmpNothing));
//    regNothingSet = boost::make_u32regex(utf8converter::ToUtf8(utf8converter::FromUtf8(tmpNothing)));
    //64: regNothingSet = boost::make_u32regex(tmpNothing);
}

RulesPtr RuleLoader::readFromFile(std::string &filename) //, LatticeWrapperPtr latticeWrapper)
{
    //boost::u32regex regComment = boost::make_u32regex("#.*");
    //boost::u32regex regWhite = boost::make_u32regex("\\s+");

    RE2 regComment("#.*");
    RE2 regWhite("\\s+");

//    Rules rules;

    rules->clear();

    std::ifstream ruleFile;
    //ruleFile.exceptions ( ifstream::failbit | ifstream::badbit );
//    try
  //  {
        ruleFile.open(filename.c_str());
        if (!ruleFile)
        {
            std::cerr << "Could not open rules file: " << filename << "." << std::endl;
            return rules;
        }
    //Rule rule;
    std::string ruleName;
    std::string rulePattern;
    std::string rulePatternLeft, rulePatternMatch, rulePatternRight;
    int ruleLeftSize, ruleMatchSize, ruleRightSize;
    ruleLeftSize = ruleMatchSize = ruleRightSize = 0;
    bool hasLeft = false;
    bool hasRight = false;
    int bracketCount = 0;

    std::vector<std::string> tokensPatterns;
    std::vector<std::string> tokensModifiers;
    std::vector<bool> tokensRequired;

    std::vector<int> matchedIndices;

    NegativePatternStrings negativePatterns;

    std::string chars = "";

    rulePattern = "";
    rulePatternLeft = "";
    rulePatternMatch = "";
    rulePatternRight = "";

    int line_number = 0;

    while (!ruleFile.eof())
    {
        line_number ++;
        std::string line;
        getline(ruleFile, line);

        //std::string line2 = line;
        //line = boost::u32regex_replace(line, regComment, "", boost::match_default | boost::format_sed);
        RE2::Replace(&line, regComment, "");
        if (line == "")
            continue;
        if (RE2::FullMatch(line, regWhite)) {
            continue;
        }
        //if (boost::u32regex_match(line, regWhite))
        //    continue;

        line = boost::algorithm::trim_copy(line);
        if (line.find("Rule") == 0)
        {
            if (ruleName != "")
            {
                //std::cout << "Akcje: " << chars << std::endl;
                bool repeat = false;
                ActionsPtr actions = this->compileRuleAction(chars,
                        ruleLeftSize, ruleMatchSize, ruleRightSize, ruleName,
                        repeat); //, latticeWrapper);
                RulePtr rule = RulePtr( new Rule(ruleName, rulePattern,
                            ruleLeftSize, ruleMatchSize, ruleRightSize,
                            actions, tokensPatterns, tokensModifiers,
                            tokensRequired, matchedIndices, repeat,
                            rulePatternLeft, rulePatternMatch, rulePatternRight,
                            negativePatterns) );
//                std::cerr << "dodaje regule: " << ruleName << std::endl;
                //rule.setActions...
//                delete actions;
                chars = "";
                ruleName = "";
                rulePattern = "";
                rulePatternLeft = "";
                rulePatternMatch = "";
                rulePatternRight = "";
                ruleLeftSize = ruleMatchSize = ruleRightSize = 0;
                hasLeft = false;
                hasRight = false;
                bracketCount = 0;
                tokensPatterns.clear();
                tokensModifiers.clear();
                tokensRequired.clear();
                matchedIndices.clear();
                negativePatterns.clear();
                rules->push_back(rule);
            }
            //zapisanie 'starej' reguly do wektora!

            ruleName = this->compileRuleName(line);
            //std::cout << "Nazwa: " << line << std::endl;

            //rule.setName(ruleName);
        }
        else if (line.find("Left:") == 0)
        {
            if (chars != "")
            {
                std::cerr << "Unexpected characters before Left context declaration: " << chars << " at line: " << line_number << std::endl;
                chars = "";
            }
            chars += line;
            hasLeft = true;
        }
        else if (line.find("Match:") == 0)
        {
            if (chars != "")
            {
                if (hasLeft)
                {
                    //std::cout << "Kontekst lewy: " << chars << std::endl;
                    rulePatternLeft = chars;
                    rulePattern = this->compileRulePattern(chars, ruleLeftSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
                    //rulePatternLeft = this->compileRuleMatch(chars, ruleLeftSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePattern = rulePatternLeft;
                    chars = "";
                }
                else
                {
                    std::cerr << "Unexpected characters before Match declaration: " << chars << " at line: " << line_number << std::endl;
                    chars = "";
                }
            }
            chars += line;
            hasRight = false;
        }
        else if (line.find("Right:") == 0)
        {
            if (chars != "")
            {
                //std::cout << "Maczo: " << chars << std::endl;
                rulePatternMatch = chars;
                rulePattern += this->compileRulePattern(chars, ruleMatchSize,
                        tokensPatterns, tokensModifiers, tokensRequired,
                        matchedIndices, bracketCount, negativePatterns);
                //rulePatternMatch = this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                //rulePattern += rulePatternMatch;
                chars = "";
            }
            chars += line;
            hasRight = true;
        }
        else if (line.find("Eval:") == 0)
        {
            if (chars != "")
            {
                if (!hasRight)
                {
                    //std::cout << "Maczo: " << chars << std::endl;
                    //std::cout << "Taki macz: " << chars << std::endl;
                    rulePatternMatch = chars;
                    rulePattern += this->compileRulePattern(chars, ruleMatchSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
                    //rulePatternMatch = this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePattern += rulePatternMatch;
                    //std::cout << "taki wzorze: " << rulePattern << std::endl;
                }
                else
                {
                    //std::cout << "Kontekst prawy: " << chars << std::endl;
                    rulePatternRight = chars;
                    rulePattern += this->compileRulePattern(chars, ruleRightSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
                    //rulePatternRight = this->compileRuleMatch(chars, ruleRightSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePattern += rulePatternRight;
                }
                chars = "";
            }
            chars += line;
        }
        else
        {
            chars += line;
        }
    }
    if (ruleName != "")
    {
        //std::cout << "Akcje: " << chars << std::endl;
        bool repeat = false;
        ActionsPtr actions = this->compileRuleAction(chars, ruleLeftSize,
                ruleMatchSize, ruleRightSize, ruleName, repeat); //, latticeWrapper);
        RulePtr rule = RulePtr( new Rule(ruleName, rulePattern, ruleLeftSize,
                    ruleMatchSize, ruleRightSize, actions, tokensPatterns,
                    tokensModifiers, tokensRequired, matchedIndices, repeat,
                    rulePatternLeft, rulePatternMatch, rulePatternRight,
                    negativePatterns) );
//        delete actions;
        //rule.setActions...
        rules->push_back(rule);
    }




   // }
   // catch (std::ifstream::failure e)
   // {
   //     std::cerr << "Error while opening file: " << filename << "." << std::endl;
   // }


//    std::locale old_locale;
//    std::locale utf8_locale(old_locale, new boost::program_options::detail::utf8_codecvt_facet(0));
//    std::locale::global(utf8_locale);
    return rules;
}

std::string RuleLoader::compileNonTokens(std::string &matched)
{
//    boost::u32regex regWhite = boost::make_u32regex("\\s+");
//    boost::u32regex regSpecialNeg = boost::make_u32regex("!(sb|se|ns)");
//    boost::u32regex regSpecial = boost::make_u32regex("(^|[ \\(\\)\\|])(sb|se|ns)");
//    boost::u32regex regLPar = boost::make_u32regex("\\(");
//    boost::u32regex regRPar = boost::make_u32regex("\\)([+*?])?");
    RE2 regWhite("\\s+");
    RE2 regSpecialNeg("!(sb|se|ns)");
    RE2 regSpecial("(^|[ \\(\\)\\|])(sb|se|ns)");
    RE2 regLPar("\\(");
    RE2 regRPar("\\)([+*?])?");

//    matched = boost::u32regex_replace(matched, regLPar, "((", boost::match_default | boost::format_sed);
//    matched = boost::u32regex_replace(matched, regRPar, ")\\1)", boost::match_default | boost::format_sed);
//    //matched = boost::u32regex_replace(matched, regSpecialNeg, "(<<s<[^<>]+<(?!\\1)[^<>]+<>)", boost::match_default | boost::format_sed);
//    matched = boost::u32regex_replace(matched, regSpecialNeg, "((<<t<[^<>]+<[^<>]+<[^>]+>)|(<<g<[^<>+]<[^<>]+<[^<>]+<[^>]+))", boost::match_default | boost::format_sed);
//    matched = boost::u32regex_replace(matched, regSpecial, "\\1(<<s<[^<>]+<\\2<>)", boost::match_default | boost::format_sed);
//    matched = boost::u32regex_replace(matched, regWhite, "", boost::match_default | boost::format_sed);
    RE2::GlobalReplace(&matched, regLPar, "((");
    RE2::GlobalReplace(&matched, regRPar, ")\\1)");
    //matched = boost::u32regex_replace(matched, regSpecialNeg, "(<<s<[^<>]+<(?!\\1)[^<>]+<>)", boost::match_default | boost::format_sed);
    RE2::GlobalReplace(&matched, regSpecialNeg, "((<<t<[^<>]+<[^<>]+<[^>]+>)|(<<g<[^<>+]<[^<>]+<[^<>]+<[^>]+))");
    RE2::GlobalReplace(&matched, regSpecial, "\\1(<<s<[^<>]+<\\2<>)");
    RE2::GlobalReplace(&matched, regWhite, "");

    return matched;
}

std::string RuleLoader::compileRuleName(std::string matched)
{
    //boost::u32regex regRuleName = boost::make_u32regex("Rule\\s*\"\\s*(.*)\\s*\"\\s*;?\\s*");
    RE2 regRuleName("Rule\\s*\"\\s*(.*)\\s*\"\\s*;?\\s*");
    //boost::smatch container;
    std::string container;
    //if (boost::u32regex_match(matched, container, regRuleName))
    if (RE2::FullMatch(matched, regRuleName, &container)) {
        std::string s = container; //@todo: zwinac ten badziew
//        std::cout << "Kompilejszyn reguly: " << container[1] << std::endl;
        return s;
    }
    else
    {
        std::cerr << "Wrong rule declaration: " << matched << "." << std::endl;
        return "";
    }

}

std::string RuleLoader::compileRulePattern(std::string &matched, int &size,
        std::vector<std::string> &tokensPatterns,
        std::vector<std::string> &tokensModifiers,
        std::vector<bool> &tokensRequired,
        std::vector<int> &matchedIndices, int &bracketCount,
        NegativePatternStrings &negativePatterns) {
    //boost::u32regex regMatch = boost::make_u32regex("^(Match|Left|Right)\\s*:\\s*");
    //boost::u32regex regWhite = boost::make_u32regex("\\s+");
    RE2 regMatch("^(Match|Left|Right)\\s*:\\s*");
    RE2 regWhite("\\s+");

    //matched = boost::u32regex_replace(matched, regMatch, "", boost::match_default | boost::format_sed);
    RE2::Replace(&matched, regMatch, "");

    //if ((matched == "") || (boost::u32regex_match(matched, regWhite)))
    if ((matched == "") || (RE2::FullMatch(matched, regWhite)))
    {
        std::cerr << "No patterns defined in section." << std::endl;
        return "null";
    }

    if (matched[matched.size() - 1] == ';')
    {
        matched = matched.substr(0, matched.size() - 1);
    }
    else
    {
        //tu komunikat, ze blad skladni, nie ma srednika na koncu!
    }

    //size = 0;
    size = countTokens(matched);
    std::string compiledMatch = "";
    std::string token = "";
    std::string before = "";
    while ((token = getToken(matched, before)) != "")
    {

        std::string compiledToken = compileToken(token, negativePatterns);
        if (matched != "")
        {
            if (matched[0] == '+' || matched[0] == '*')
            {
                compiledToken = "(" + compiledToken + matched[0] + ")";
                matched = matched.substr(1, std::string::npos);
            }
        }

        std::string compiled = compileNonTokens(before) + compiledToken;//compileToken(token);
        compiledMatch += compiled;
    //    size ++;
    }
    if (matched != "")
        compiledMatch += compileNonTokens(matched);

    int i = 0;
    int mindex = bracketCount;
//    if (matchedIndexes.size() == 0)
//        mindex = 0;
//    else
//        mindex = matchedIndexes.back();
    std::string s = compiledMatch;
    int brackets = 0;
    bool nonMatching = false;
    //@todo: przemyslec, czy ponizszego jakos nie da rady zrobic zmyslniej
    while (s != "")
    {
        if (s[i] == '(')
        {
            if ((i > 0) && (s[i - 1] == '\\'))
            {
                i ++;
                continue;
            }
            if (((i+1) < s.length()) && (s[i + 1] == '?'))
            {
                nonMatching = true;
                i ++;
                continue;
            }
            mindex ++;
            if (brackets == 0)
            {
//                std::cout << "wrzucom numer: " << mindex << std::endl;
                matchedIndices.push_back(mindex);
            }
            brackets ++;
        }
        if (s[i] == ')')
        {
            if ((i > 0) && (s[i - 1] == '\\'))
            {
                i ++;
                continue;
            }
            if (nonMatching)
            {
                nonMatching = false;
                i ++;
                continue;
            }

            brackets --;
            if (brackets == 0)
            {

                std::string pattern = s.substr(0, i + 1);
                if ((i + 1) < s.length())
                    s = s.substr(i + 1, std::string::npos);
                else
                    s = "";
                if (s != "")
                {
                    if (s[0] == '?')
                    {
//                        std::cerr << "s?: " << s << std::endl;
                        tokensModifiers.push_back("?");
                        tokensRequired.push_back(false);
                        s = s.substr(1, std::string::npos);
                    }
                    else if (s[0] == '*')
                    {
//                        std::cerr << "s*: " << s << std::endl;
                        tokensModifiers.push_back("*");
                        tokensRequired.push_back(false);
                        s = s.substr(1, std::string::npos);
                    }
                    else if (s[0] == '+')
                    {
//                        std::cerr << "s+: " << s << std::endl;
                        tokensModifiers.push_back("+");
                        tokensRequired.push_back(true);
                        s = s.substr(1, std::string::npos);
                    }
                    else
                    {
                        std::string pattern_test = pattern.substr(1, pattern.length() - 2);
                        if (pattern_test[pattern_test.length() - 1] == '+')
                        {
                            tokensModifiers.push_back("+");
                            tokensRequired.push_back(true);
                            //pattern = pattern.substr(0, pattern.length() - 1);
//                            pattern = "(" + pattern_test.substr(0, pattern_test.length() - 1) + ")";
                        }
                        else if (pattern_test[pattern_test.length() - 1] == '*')
                        {
                            tokensModifiers.push_back("*");
                            tokensRequired.push_back(false);
                            //pattern = pattern.substr(0, pattern.length() - 1);
//                            pattern = "(" + pattern_test.substr(0, pattern_test.length() - 1) + ")";
                        }
                        else if (pattern_test[pattern_test.length() - 1] == '?')
                        {
                            tokensModifiers.push_back("?");
                            tokensRequired.push_back(false);
                            //pattern = pattern.substr(0, pattern.length() - 1);
//                            pattern = "(" + pattern_test.substr(0, pattern_test.length() - 1) + ")";
                        }
                        else
                        {
                            tokensModifiers.push_back("");
                            tokensRequired.push_back(true);
                        }
                    }
                }
                else
                {
                    std::string pattern_test = pattern.substr(1, pattern.length() - 2);
                    if (pattern_test[pattern_test.length() - 1] == '+')
                    {
                        tokensModifiers.push_back("+");
                        tokensRequired.push_back(true);
                        //pattern = pattern.substr(0, pattern.length() - 1);
//                            pattern = "(" + pattern_test.substr(0, pattern_test.length() - 1) + ")";
                    }
                    else if (pattern_test[pattern_test.length() - 1] == '*')
                    {
                        tokensModifiers.push_back("*");
                        tokensRequired.push_back(false);
                        //pattern = pattern.substr(0, pattern.length() - 1);
//                            pattern = "(" + pattern_test.substr(0, pattern_test.length() - 1) + ")";
                    }
                    else if (pattern_test[pattern_test.length() - 1] == '?')
                    {
                        tokensModifiers.push_back("?");
                        tokensRequired.push_back(false);
                        //pattern = pattern.substr(0, pattern.length() - 1);
//                            pattern = "(" + pattern_test.substr(0, pattern_test.length() - 1) + ")";
                    }
                    else
                    {
                        tokensModifiers.push_back("");
                        tokensRequired.push_back(true);
                    }
                }
                tokensPatterns.push_back(pattern);
                i = 0;
                continue;
            }
        }
        i ++;
    }
    //std::cout << "kompiled macz: " << compiledMatch << std::endl;
    //std::cout << "Liczba tokenĂłw: " << size << std::endl;

    bracketCount = mindex;
    return compiledMatch;
}

std::string RuleLoader::getToken(std::string &matched, std::string &before)
{
    int brackets = 0;
    std::string s = "";
    int start = matched.find("[");
    if (start == std::string::npos)
    {
        //std::cerr << "Illegal matching definition: " << matched << std::endl;
        return "";
    }

    bool found = false;
    int i = start;
    while (i < matched.size())
    {
        if (matched[i] == '[')
        {
            if (i > 0)
            {
                if (matched[i - 1] != '\\')
                {
                    s = "";
                    brackets ++;
                }
            }
            else
                brackets ++;
        }
        else if (matched[i] == ']')
        {
            if (i > 0)
            {
                if (matched[i - 1] != '\\')
                {
                    brackets --;
                    if (brackets == 0)
                    {
                        found = true;
                        break;
                    }
                }
            }
            else
            {
                brackets --;
                if (brackets == 0)
                {
                    found = true;
                    break;
                }
            }
        }
        i ++;
    }
    if (found)
    {
        before = matched.substr(0, start);
        s = matched.substr(start, (i - start + 1));
        matched = matched.substr(i + 1, std::string::npos);
        return s;
    }
    else
    {
        return "";
    }


}


//std::string RuleLoader::getToken(std::string &matched, std::string &before)
//{
//    int begin = matched.find_first_of("[");//(");
//    if (begin == std::string::npos)
//    {
//        //jeszcze by trzeba wywolac nietokeny chyba
//        return "";
//    }
//  //  else
//  //  {
//  //      std::cout << "POS: " << begin << std::endl;
//  //  }
//    int end = begin + 1;
//    int pos = begin;
//    std::string find;
////    if (matched[begin] == '[')
//        find = "]";
////    else
////        find = ")";
//    while (end != std::string::npos)
//    {
//        end = matched.find(find, pos);
//        if (end != std::string::npos)
//        {
//            if (matched[end - 1] == '\\')
//            {
//                pos = end + 1;
//                continue;
//            }
//            else
//                break;
//        }
//    }
//
//    //readKey, readOperator, readValue
//
//    std::string result = matched.substr(begin, (end - begin + 1));
////    if (matched.find_first_of("*+?", end) == (end + 1))
////    {
////        result += matched[end + 1];
////        end ++;
////    }
//    before = matched.substr(0, begin);
//    matched = matched.substr(end + 1, std::string::npos);
//    return result;
//}

int RuleLoader::countTokens(std::string &matched)
{
    int count = 0;
    int parenthesis = 0;
    int brackets = 0;
    std::string s = "";
    int i = 0;
    if (matched == "sb" || matched == "se" || matched == "ns" || matched == "!sb" || matched == "!se" || matched == "!ns")
        return 1;

    while (i < matched.size())
    {
        if (matched[i] == '[')
        {
            if (i > 0)
            {
                if (matched[i - 1] != '\\')
                {
                    if ((brackets == 0) && (parenthesis == 0))
                        if ((s == "ns") || (s == "sb") || (s == "se") || (s == "!ns") || (s == "!sb") || (s == "!se"))
                            count ++;
                    s = "";
                    brackets ++;
                }
            }
            else
                brackets ++;
        }
        else if (matched[i] == '(')
        {
            if (i > 0)
            {
                if (matched[i - 1] != '\\')
                {
                    if ((brackets == 0) && (parenthesis == 0))
                        if ((s == "ns") || (s == "sb") || (s == "se") || (s == "!ns") || (s == "!sb") || (s == "!se"))
                            count ++;
                    s = "";
                    parenthesis ++;
                }
            }
            else
                parenthesis ++;
        }
        else if (matched[i] == ']')
        {
            if (i > 0)
            {
                if (matched[i - 1] != '\\')
                {
                    brackets --;
                    if ((brackets == 0) && (parenthesis == 0))
                        count ++;
                }
            }
            else
            {
                brackets --;
                if ((brackets == 0) && (parenthesis == 0))
                    count ++;
            }
        }
        else if (matched[i] == ')')
        {
            if (i > 0)
            {
                if (matched[i - 1] != '\\')
                {
                    parenthesis --;
                    if ((brackets == 0) && (parenthesis == 0))
                        count ++;
                }
            }
            else
            {
                parenthesis --;
                if ((brackets == 0) && (parenthesis == 0))
                    count ++;
            }
        }
        else
        {
            if ((brackets == 0) && (parenthesis == 0))
            {
                if (matched[i] == ' ')
                {
                    if ((s == "ns") || (s == "sb") || (s == "se") || (s == "!ns") || (s == "!sb") || (s == "!se"))
                        count ++;
                    s = "";
                }
                else
                {
                    if ((matched[i] != '!') && (matched[i] != '?') && (matched[i] != '*') && (matched[i] != '+'))
                        s += matched[i];
                }
            }
        }

        i ++;
    }

    return count;
}

std::string RuleLoader::compileToken(std::string &token,
        NegativePatternStrings &negativePatterns, bool no_prefix) {

    if ((token[0] == '[') && (token[token.size() - 1] == ']'))
    {
        token = token.substr(1, token.size() - 2);
    }
    else
    {
        std::cerr << "Illegal token: " << token << std::endl;
        return "";
    }
    bool tokenMatch = false;
    std::string orth = "";
    //Interpretations interps;
    //interps.push_back(baseMask);
    TokenPatternPart emptyPatternPart;
    TokenPattern emptyPattern;
    emptyPattern.parts.assign(tagset->getNumberOfAttributes() + 1,
            emptyPatternPart);
    TokenPatterns tokenPatterns;
    tokenPatterns.push_back(emptyPattern);
    bool groupMatch = false;
    std::string compiledHead = "";
    std::string type = "";

    while (token != "")
    {
        bool icase = false;
        std::string key = getKey(token);
        std::string compOperator = getOperator(token);
        if ((key != "head") && (key != "type"))
        {
            tokenMatch = true;
            groupMatch = false;
        }
        else
        {
            groupMatch = true;
            tokenMatch = false;
        }

        std::string value;
        if (key != "head")
        {
            icase = false;
            value = getValue(token);
            if ((key == "base") || (key == "orth"))
            {
                if (token.find("/i") == 0)
                {
//                    std::cout << "Mam to miglanca niczeulego!" << std::endl;  //TODO: zrobic to case-insenstive dopasowywawenie, aczkolwiek moze byc problem z tym, bo boost nie umie. zamienic to racznie jakimis sposobami? zamienic na [Uu] ? Ale to pewnie bÄdzie szybkie niemoĹźliwie
                    token = token.substr(2, std::string::npos);
                    icase = true;
                }
            }
        }
        else
        {
            //aa
            int begin = token.find("[");
            int end = token.find("]", begin + 1);
            int cite = token.find("\"", begin + 1);
            if (cite > 0)
            {
                int begin2 = token.find("[", begin + 1);
                if (begin2 > cite)
                {
                    cite = token.find("\"", cite + 1);
                    if (cite > 0)
                    {
                        end = token.find("]", cite + 1);
                    }
                }
//                while ((token[end - 1] == ':') && (end < token.size()))
//                {
//                    end = token.find("]", end + 2);
//                }
            }
            //int cite = token.find("\"", begin + 1);
            //while (end
            std::string head = token.substr(begin, end - begin + 1);
            if ((end + 1) < token.size())
                token = token.substr(end + 1, std::string::npos);
            else
                token = "";
            if (head != "[]")
            {
                //compiledHead = compileToken(head);
                //compiledHead = compiledHead.substr(4, compiledHead.length() - 5);
                compiledHead = compileToken(head, negativePatterns, true);
            }
            else
                //compiledHead = "<<t<[^<>]+<[^<>]+<[^>]+>";
                compiledHead = "<[^<>]+<[^<>]+<[^>]+>";
        }

        if (key == "head")
        {
            //wszystko juz zrobione wyzej?
        }
        else if (key == "type")
        {
            type = value;
        }
        else if (key == "pos")
        {
            //bool ret = compilePosCondition(compOperator, value, interps);
            bool ret = compilePosCondition(compOperator, value, tokenPatterns);
            if (!ret)
            {
                std::cerr << "EROR!" << std::endl;
            }
        }
        else if (key == "base")
        {
            //bool ret = compileBaseCondition(compOperator, value, interps, icase);
            bool ret = compileBaseCondition(compOperator, value, tokenPatterns, icase);
            if (!ret)
            {
                std::cerr << "EROR!" << std::endl;
            }
        }
        else if (key == "orth")
        {
            orth = compileOrthCondition(compOperator, value, icase);
            if (orth == "")
            {
                std::cerr << "EROR!" << std::endl;
            }
        }
        else
        {  //atrybuty teraz!
            //bool ret = compileAttributeCondition(key, compOperator, value, interps);
            bool ret = compileAttributeCondition(key, compOperator, value,
                    tokenPatterns);
            if (!ret)
            {
                std::cerr << "EROR!" << std::endl;
            }
        }
        //std::cout << "Klucz: " << key << std::endl << "Operator: " << compOperator << std::endl << "Wartosc: " << value << std::endl;
        if (token.find("&&") == 0)
        {
            token = token.substr(2, std::string::npos);

        }
        // wytnij &&
    }

    std::stringstream ss;
    //@todo: experimental uniform token compiled pattern format
    if (! no_prefix) {
        if (tokenMatch)
            ss << "<<t";
        if (groupMatch)
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
        ss << generateTokenPatternsString(tokenPatterns, negativePatterns);
        //ss << interpretationsToString(interps); //@todo: o tu - jak to jest puste to co?
    }
    ss << ">";

    std::string result = ss.str();
    //@todo: former format goes below:
/*    if (tokenMatch)
    {
        if (orth == "")
            orth = "[^<>]+";

        result = "<<t";
        result += "<[^<>]+"; //id
        result += "<" + orth;
        result += interpretationsToString(interps);
        result += ">";
    }

    if (groupMatch)
    {
        result = "<<g";
        result += "<[^<>]+"; //id
        result += "<" + type;// + ">";
        if (compiledHead != "")
            result += compiledHead;
        else
            //result += "<<t<[^<>]+<[^<>]+<[^>]+>";
            result += "<[^<>]+<[^<>]+<[^>]+>";
    }*/

    return "(" + result + ")";
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
/*    int pos = token.find_first_of("!~=");
    while ((pos > 0) && (token[pos - 1] == '\\') && (pos != std::string::npos))
    {
        pos = token.find_first_of("!~=", pos);
    }
    if (pos == std::string::npos)
    {
        std::cerr << "Illegal token specification: " << token << std::endl;
        return "";
    }
    std::string key = token.substr(0, pos);
    key = boost::algorithm::trim_copy(key);
    token = token.substr(pos, std::string::npos);

    return key;*/
}

std::string RuleLoader::getValue(std::string &token)
{
    int start_pos = token.find_first_not_of(" "); //skip spaces at the begining
    if (start_pos == std::string::npos)
        start_pos = 0;
    int end_pos = start_pos;
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

    //@todo: tu jeszcze te cuda z eskejpowanymi nawiasami

    return value;

    /*int begin = token.find_first_not_of(" ");
    if (begin == std::string::npos)
        begin = 0;
    int end = begin;
    int quotes = 0;
    int icase = 0 ;
    while (end < token.length())
    {
        if ((token[end] == ' ') || (token[end] == '&'))
        {
            if (quotes == 0)
            {
                break;
            }
            //    break;
            else
            {
                end ++;
                continue;
            }
        }
        if (token[end] == '"') {
            if ((end == 0) || (token[end - 1] != '\\'))
            {
                if (!quotes)
                    quotes ++;
                else
                    quotes --;
            }
        }
        end ++;
    }

    //int end = token.find_first_of(" &", begin + 1);
    //if (end == std::string::npos)
    //    end = token.size();
    if (end == token.size())
    {
        if ((end > 1) && (token[end - 2] == '/') && (token[end - 1] == 'i'))
        {
            end -= 2;
            icase = -1;
        }
    }

    quotes = 0;
    if ((token[begin] == '"') && ((end == 0) || (token[end - 1] == '"')))
    {
        quotes = 1;
    }
//    int begin = token.find("\"");
//    while ((begin > 0) && (token[begin - 1] == '\\') && (begin != std::string::npos))
//    {
//        begin = token.find("\"", begin);
//    }
//    if (begin == std::string::npos)
//    {
//        std::cerr << "llegal token specification: " << token << std::endl;
//        return "";
//    }
//    int end = token.find("\"", begin + 1);
//    while ((end > 0) && (token[end - 1] == '\\') && (end != std::string::npos))
//    {
//        end = token.find("\"", end);
//    }
//    if (end == std::string::npos)
//    {
//        std::cerr << "Illegal token specification: " << token << std::endl;
//        return "";
//    }
    std::string value = token.substr(begin + quotes,  (end - begin - 2 * quotes));
    if ((end + 1) < token.size())
        token = boost::algorithm::trim_copy(token.substr(end + 1 + icase, std::string::npos));
    else
        token = "";

    //if (value.find_first_of("\\{}()") != std::string::npos)
    if (value.find_first_of("\\()") != std::string::npos)
    {
//        std::cout << "jest tu jakis cwaniak: " << value << std::endl;
        int q = 0;
        std::string val;
        //while (value.find_first_of("\\{}()", q) != std::string::npos)
        while (value.find_first_of("\\()", q) != std::string::npos)
        {
            //int p = value.find_first_of("\\{}()", q);
            int p = value.find_first_of("\\()", q);
            if ((value.at(p) == '(') || (value.at(p) == ')'))
            {
                if ((p == 0) || (value.at(p - 1) != '\\'))
//                if (!(p > 0) && (value.at(p-1) == '\\'))
                {
                    val += value.substr(q, p - q);
                    val += value.at(p);
                    q = p + 1;
                    continue;
                }
            }
            val += value.substr(q, p - q);
            //val += "\\" + value.substr(p, 1);
            val += value.substr(p, 1);
//            std::cout << "val= " << val << std::endl;
            q = p + 1;
        }
        if (q < value.length())
            val += value.substr(q, std::string::npos);
//        std::cout << "tak wyglada po robocie: " << val << std::endl;
        value = val;
    }

    return value;*/
}

std::string RuleLoader::getOperator(std::string &token) {
    utf8iterator begin(token.begin(), token.begin(), token.end());
    utf8iterator end(token.end(), token.begin(), token.end());
    utf8iterator it = begin;
    while (it != end) {   // find the position of code point following the comparison operator
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
/*    int pos = token.find_first_not_of("!~=");
    if (pos == std::string::npos)
    {
        std::cerr << "Illegal token specification: " << token << std::endl;
        return "";
    }
    std::string op = token.substr(0, pos);
    token = boost::algorithm::trim_copy(token.substr(pos, std::string::npos));
    return op;*/
}

bool RuleLoader::compilePosCondition(std::string &comparisonOperator,
        std::string &value, TokenPatterns &tokenPatterns) {
    //std::string mapped;
    std::string posString;
    if (value.find_first_of(".|*+?[]") != std::string::npos) {
        std::vector<std::string> values = tagset->getPosMatching(value);
        if (values.size() == 0) {
            std::cerr << "No parts of speech identifiers matching regular expression: \"" << value << "\"." << std::endl;
            return false;
        }
        posString = boost::join(values, "|");
        //mapped = "";
//        std::vector<char>::iterator i = values.begin();
//        //@todo: warto by to przerobic na jakeigos joina, ale ten char zawadza tu nieco
//        while (i != values.end()) {
//            mapped += *i;
//            i ++;
//        }
//        if (comparisonOperator != "!~")
//            mapped = "[" + mapped + "]";
    } else {
        if (!tagset->checkPos(value)) {
            std::cerr << "Unknown part of speech: " << value << "." << std::endl;
            return false;
        }
        //mapped = tagset->mapPos(value);
        posString = value;
    }
    if (comparisonOperator == "~~") {
        for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
                patternIt != tokenPatterns.end(); patternIt ++) {
            patternIt->parts.at(0).condition = posString;
            patternIt->parts.at(0).negative = false;
        }
//        Interpretations::iterator i = interps.begin();
//        while (i != interps.end()) {
//            (*i)[0] = mapped;
//            i ++;
//        }
    } else if (comparisonOperator == "~") {
        if (tokenPatterns.size() == 1) {
            TokenPattern formerPattern = tokenPatterns.front();
            TokenPattern newPattern = formerPattern;
            newPattern.parts.at(0).condition = posString;
            newPattern.parts.at(0).negative = false;
            newPattern.modifier = "()";
            formerPattern.modifier = "*";
            tokenPatterns.at(0) = formerPattern;
            tokenPatterns.push_back(newPattern);
            tokenPatterns.push_back(formerPattern);
        } else {
            tokenPatterns.at(1).parts.at(0).condition = posString;
            tokenPatterns.at(1).parts.at(0).negative = false;
        }
//        if (interps.size() == 1) {
//            TokenMask currentInterpretation = interps.front();
//            currentInterpretation[modifierIndex] = "*";
//            (interps.front())[0] = mapped;
//            (interps.front())[modifierIndex] = "()";
//            interps.push_back(currentInterpretation);
//            interps.insert(interps.begin(), currentInterpretation);
//        } else {
//            Interpretations::iterator i = interps.begin() + 1;
//            (*i)[0] = mapped;
//        }
    } else if (comparisonOperator == "!~") {
        for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
                patternIt != tokenPatterns.end(); patternIt ++) {
            patternIt->parts.at(0).condition = posString;
            patternIt->parts.at(0).negative = true;
        }
//        Interpretations::iterator i = interps.begin();
//        while (i != interps.end()) {
//            (*i)[0] = "[^" + mapped;
//            (*i)[0] += "]";
//            i ++;
//        }
    } else {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return false;
    }

    return true;
}

/*bool RuleLoader::compilePosCondition(std::string &comparisonOperator, std::string &value, Interpretations &interps) {
    std::string mapped;
    if (value.find_first_of(".|*+?[]") != std::string::npos) {
        std::vector<char> values = tagset->mapPosMatching(value);
        if (values.size() == 0) {
            std::cerr << "No parts of speech identifiers matching regular expression: \"" << value << "\"." << std::endl;
            return false;
        }
        mapped = "";
        std::vector<char>::iterator i = values.begin();
        //@todo: warto by to przerobic na jakeigos joina, ale ten char zawadza tu nieco
        while (i != values.end()) {
            mapped += *i;
            i ++;
        }
        if (comparisonOperator != "!~")
            mapped = "[" + mapped + "]";
    } else {
        if (!tagset->checkPos(value)) {
            std::cerr << "Unknown part of speech: " << value << "." << std::endl;
            return false;
        }
        mapped = tagset->mapPos(value);
    }
    if (comparisonOperator == "~~") {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end()) {
            (*i)[0] = mapped;
            i ++;
        }
    } else if (comparisonOperator == "~") {
        if (interps.size() == 1) {
            TokenMask currentInterpretation = interps.front();
            currentInterpretation[modifierIndex] = "*";
            (interps.front())[0] = mapped;
            (interps.front())[modifierIndex] = "()";
            interps.push_back(currentInterpretation);
            interps.insert(interps.begin(), currentInterpretation);
        } else {
            Interpretations::iterator i = interps.begin() + 1;
            (*i)[0] = mapped;
        }
    } else if (comparisonOperator == "!~") {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end()) {
            (*i)[0] = "[^" + mapped;
            (*i)[0] += "]";
            i ++;
        }
    } else {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return false;
    }

    return true;
}*/

bool RuleLoader::compileBaseCondition(std::string &comparisonOperator,
                    std::string &value, TokenPatterns &tokenPatterns,
                    bool icase) {
    RE2::GlobalReplace(&value, *regAmp, "\\&amp;");
    RE2::GlobalReplace(&value, *regLt, "\\&lt;");
    RE2::GlobalReplace(&value, *regGt, "\\&gt;");
    RE2::GlobalReplace(&value, *regLPar, "\\&lpar;");
    RE2::GlobalReplace(&value, *regRPar, "\\&rpar;");
    RE2::GlobalReplace(&value, *regAlt, "\\&bar;");
    RE2::GlobalReplace(&value, *regPlus, "\\&plus;");
    RE2::GlobalReplace(&value, *regAsterisk, "\\&astr;");
    RE2::GlobalReplace(&value, *regOpt, "\\&qmark;");

    if (icase) {
        value = "(?i:" + value + ")";
    }

    if (value.find_first_of(".|*+?") != std::string::npos){
        std::string tmpVal = "";
        int idx = 0;
        int pos;
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

    //@todo: tutaj tego typu rzeczy nie da sie zrobic jakos ladniej w RE2? tam jest ta funkcja metaquote - cos takiego tu nie wystarczy?
//    boost::u32regex regLpar = boost::make_u32regex("\\\\\\\\\\\\\\(");
//    boost::u32regex regRpar = boost::make_u32regex("\\\\\\\\\\\\\\)");
//    value = boost::u32regex_replace(value, regLpar, "\\&lpar;", boost::match_default | boost::format_sed);
//    value = boost::u32regex_replace(value, regRpar, "\\&rpar;", boost::match_default | boost::format_sed);
    RE2 regLpar("\\\\\\\\\\\\\\(");
    RE2 regRpar("\\\\\\\\\\\\\\)");
    RE2::GlobalReplace(&value, regLpar, "\\&lpar;");
    RE2::GlobalReplace(&value, regRpar, "\\&rpar;");

    if (value.find("|") != std::string::npos) {
        value = "(" + value + ")";
    }

    if (comparisonOperator == "~~") {
        for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
                patternIt != tokenPatterns.end(); patternIt ++) {
            patternIt->base.condition = value;
            patternIt->base.negative = false;
        }
//        Interpretations::iterator i = interps.begin();
//        while (i != interps.end()) {
//            (*i)[baseIndex] = value;
//            i ++;
//        }
    } else if (comparisonOperator == "~") {
        if (tokenPatterns.size() == 1) {
            TokenPattern formerPattern = tokenPatterns.front();
            TokenPattern newPattern = formerPattern;
            newPattern.base.condition = value;
            newPattern.base.negative = false;
            newPattern.modifier = "()";
            formerPattern.modifier = "*";
            tokenPatterns.at(0) = formerPattern;
            tokenPatterns.push_back(newPattern);
            tokenPatterns.push_back(formerPattern);
        } else {
            tokenPatterns.at(1).base.condition = value;
            tokenPatterns.at(1).base.negative = false;
        }
//        if (interps.size() == 1) {
//            TokenMask currentInterpretation = interps.front();
//            currentInterpretation[modifierIndex] = "*";
//            (interps.front())[baseIndex] = value;
//            (interps.front())[modifierIndex] = "()";
//            interps.push_back(currentInterpretation);
//            interps.insert(interps.begin(), currentInterpretation);
//        } else {
//            Interpretations::iterator i = interps.begin() + 1;
//            (*i)[baseIndex] = value;
//        }
    } else if (comparisonOperator == "!~") {
        for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
                patternIt != tokenPatterns.end(); patternIt ++) {
            patternIt->base.condition = value;
            patternIt->base.negative = true;
        }
//        Interpretations::iterator i = interps.begin();
//        while (i != interps.end()) {
//            //(*i)[baseIndex] = "(?!" + value + ")[^<>]+";
//            (*i)[baseIndex] = "(?P<SZATAN>" + value + ")[^<>]+";
//            i ++;
//        }
    } else {
        std::cerr << "Unknown comparison operator: " << comparisonOperator
            << "." << std::endl;
        return false;
    }

    return true;
}

/*bool RuleLoader::compileBaseCondition(std::string &comparisonOperator,
                    std::string &value, Interpretations &interps, bool icase) {
//    value = boost::regex_replace(value, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regLt, "\\&lt;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regGt, "\\&gt;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regAlt, "\\&bar;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);
    RE2::GlobalReplace(&value, *regAmp, "\\&amp;");
    RE2::GlobalReplace(&value, *regLt, "\\&lt;");
    RE2::GlobalReplace(&value, *regGt, "\\&gt;");
    RE2::GlobalReplace(&value, *regLPar, "\\&lpar;");
    RE2::GlobalReplace(&value, *regRPar, "\\&rpar;");
    RE2::GlobalReplace(&value, *regAlt, "\\&bar;");
    RE2::GlobalReplace(&value, *regPlus, "\\&plus;");
    RE2::GlobalReplace(&value, *regAsterisk, "\\&astr;");
    RE2::GlobalReplace(&value, *regOpt, "\\&qmark;");

    if (icase) {
        value = "(?i:" + value + ")";
//        std::cerr << "input: " << value << std::endl;
//        std::string valU = value;
////        valU = std::toupper(valU, std::locale(""));
//        std::string valL = value;
////        valL = std::tolower(valL, std::locale(""));
//        std::cerr << "to lower: " << valL << std::endl;
//        std::cerr << "to upper: " << valU << std::endl;
////        UnicodeString valU = icuConverter::fromUtf(value);
////        UnicodeString valL = icuConverter::fromUtf(value);
////        valU.toUpper();
////        valL.toLower();
////        StringCharacterIterator itU(valU);
////        StringCharacterIterator itL(valL);
//        std::stringstream ss;
//        for (int i = 0; i < valL.size() && i < valU.size(); i ++) {
//            std::cerr << "U: " << std::toupper(valU[i], std::locale("")) << std::endl;
//            std::cerr << "L: " << std::tolower(valL[i], std::locale("")) << std::endl;
//            if (valU[i] != valL[i])
//                ss << "[" << valU[i] << valL[i] << "]";
//            else
//                ss << valU[i];
//        }
////        while (itU.hasNext())
////        {
////            UnicodeString tu = itU.current();
////            UnicodeString tl = itL.current();
////            if (tu != tl)
////                ss << "[" << icuConverter::toUtf(tu) << icuConverter::toUtf(tl) << "]";
////            else
////                ss << icuConverter::toUtf(tu);
////            itU.next();
////            itL.next();
////        }
//        value = ss.str();
    }

    if (value.find_first_of(".|*+?") != std::string::npos){
        std::string tmpVal = "";
        int idx = 0;
        int pos;
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

//    if (icase)
//    {
//        std::string valueU = value;
//        std::string valueL = value;
//        //boost::to_upper(valueU);
//        //boost::to_lower(valueL);
//        std::transform(value.begin(), value.end(), valueU.begin(), ::toupper);
//        std::transform(value.begin(), value.end(), valueL.begin(), ::tolower);
//        std::stringstream ss;
//        std::wstring wideU = utf8converter::FromUtf8(valueU);
//        std::wstring wideL = utf8converter::FromUtf8(valueL);
//        int j = 0;
//        while (wideU.at(j))
//        {
//            std::wstring tu(1, wideU.at(j));
//            std::wstring tl(1, wideL.at(j));
//            ss << "[" << utf8converter::ToUtf8(tu) << utf8converter::ToUtf8(tl) << "]";
//            j ++;
//        }
//        //value = "";
//        //for (int i = 0; i < valueU.length(); i++)
//        //    ss << "[" << valueU[i] << valueL[i] << "]";
//        value = ss.str();
//    }

    //@todo: tutaj tego typu rzeczy nie da sie zrobic jakos ladniej w RE2? tam jest ta funkcja metaquote - cos takiego tu nie wystarczy?
//    boost::u32regex regLpar = boost::make_u32regex("\\\\\\\\\\\\\\(");
//    boost::u32regex regRpar = boost::make_u32regex("\\\\\\\\\\\\\\)");
//    value = boost::u32regex_replace(value, regLpar, "\\&lpar;", boost::match_default | boost::format_sed);
//    value = boost::u32regex_replace(value, regRpar, "\\&rpar;", boost::match_default | boost::format_sed);
    RE2 regLpar("\\\\\\\\\\\\\\(");
    RE2 regRpar("\\\\\\\\\\\\\\)");
    RE2::GlobalReplace(&value, regLpar, "\\&lpar;");
    RE2::GlobalReplace(&value, regRpar, "\\&rpar;");

    if (value.find("|") != std::string::npos) {
        value = "(" + value + ")";
    }

    if (comparisonOperator == "~~") {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end()) {
            (*i)[baseIndex] = value;
            i ++;
        }
    } else if (comparisonOperator == "~") {
        if (interps.size() == 1) {
            TokenMask currentInterpretation = interps.front();
            currentInterpretation[modifierIndex] = "*";
            (interps.front())[baseIndex] = value;
            (interps.front())[modifierIndex] = "()";
            interps.push_back(currentInterpretation);
            interps.insert(interps.begin(), currentInterpretation);
        } else {
            Interpretations::iterator i = interps.begin() + 1;
            (*i)[baseIndex] = value;
        }
    } else if (comparisonOperator == "!~") {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end()) {
            //(*i)[baseIndex] = "(?!" + value + ")[^<>]+";
            (*i)[baseIndex] = "(?P<SZATAN>" + value + ")[^<>]+";
            i ++;
        }
    } else {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return false;
    }

    return true;
}*/

bool RuleLoader::compileAttributeCondition(std::string &key,
        std::string &comparisonOperator, std::string &value,
        TokenPatterns &tokenPatterns) {
    //std::string mapped;
    int attributeIndex = tagset->getAttributeIndex(key);
    if (attributeIndex == -1) {
        std::cerr << "No such attribute: " << key << "." << std::endl;
        return false;
    }
    attributeIndex += 1; //part of speech preceds attributes in token pattern parts vector
    std::string attributeString;
    if (value.find_first_of(".|*+?") != std::string::npos) {
//        attributeIndex = tagset->getAttributeIndex(key) + 1;
//        if (attributeIndex == -1) {
//            std::cerr << "No such attribute: " << key << "." << std::endl;
//            return false;
//        }
        std::vector<std::string> values =
            tagset->getAttributeValuesMatching(key, value);
        //std::vector<char> values = tagset->mapAttributeValuesMatching(key, value);
        if (values.size() == 0) {
            std::cerr << "No values of " << key <<
                " matching regular expression: '" << value << "'." << std::endl;
            return false;
        }
//        //@todo: tu tez warto jakis join dac
//        std::vector<char>::iterator i = values.begin();
//        mapped = "";
//        while (i != values.end()) {
//            mapped += *i;
//            i ++;
//        }
//        if (comparisonOperator != "!~")
//            if (values.size() > 1)
//                mapped = "[" + mapped + "]";
        attributeString = boost::algorithm::join(values, "|");
    } else {
        attributeString = value;
//        if (tagset->checkAttribute(key)) {
//            if (tagset->checkAttributeValue(key, value)) {
//                std::pair<char, int> pair = tagset->mapAttributeValue(key, value);
//                mapped = pair.first;
//                attributeIndex = pair.second + 1; // bo pierwsza w 'tablicy' interpretacji jest czesc mowy!
//            } else {
//                std::cerr << "Unknown value: " << value << " of the attribute: " << key << "." << std::endl;
//                return false;
//            }
//        } else {
//            std::cerr << "No such attribute: " << key << "." << std::endl;
//            return false;
//        }
    }
    if (comparisonOperator == "~~") {
        for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
                patternIt != tokenPatterns.end(); patternIt ++) {
            patternIt->parts.at(attributeIndex).condition = attributeString;
            patternIt->parts.at(attributeIndex).negative = false;
        }
//        Interpretations::iterator i = interps.begin();
//        while (i != interps.end()) {
//            (*i)[attributeIndex] = mapped;
//            i ++;
//        }
    } else if (comparisonOperator == "~") {
        if (tokenPatterns.size() == 1) {
            TokenPattern formerPattern = tokenPatterns.front();
            TokenPattern newPattern = formerPattern;
            newPattern.parts.at(attributeIndex).condition = attributeString;;
            newPattern.parts.at(attributeIndex).negative = false;
            newPattern.modifier = "()";
            formerPattern.modifier = "*";
            tokenPatterns.at(0) = formerPattern;
            tokenPatterns.push_back(newPattern);
            tokenPatterns.push_back(formerPattern);
        } else {
            tokenPatterns.at(1).parts.at(attributeIndex).condition = attributeString;;
            tokenPatterns.at(1).parts.at(attributeIndex).negative = false;
        }
//        if (interps.size() == 1) {
//            TokenMask currentInterpretation = interps.front();
//            currentInterpretation[modifierIndex] = "*";
//            (interps.front())[attributeIndex] = mapped;
//            (interps.front())[modifierIndex] = "()";
//            interps.push_back(currentInterpretation);
//            interps.insert(interps.begin(), currentInterpretation);
//        } else {
//            Interpretations::iterator i = interps.begin() + 1;
//            (*i)[attributeIndex] = mapped;
//        }
    }
    else if (comparisonOperator == "!~") {
        for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
                patternIt != tokenPatterns.end(); patternIt ++) {
            patternIt->parts.at(attributeIndex).condition = attributeString;
            patternIt->parts.at(attributeIndex).negative = true;
        }
//        Interpretations::iterator i = interps.begin();
//        while (i != interps.end()) {
//            (*i)[attributeIndex] = "[^" + mapped;
//            (*i)[attributeIndex] += "]";
//            i ++;
//        }
    } else {
        std::cerr << "Unknown comparison operator: " << comparisonOperator
            << "." << std::endl;
        return false;
    }

    return true;
}

/*bool RuleLoader::compileAttributeCondition(std::string &key, std::string &comparisonOperator, std::string &value, Interpretations &interps)
{
    std::string mapped;
    int attributeIndex;
    if (value.find_first_of(".|*+?") != std::string::npos) {
        attributeIndex = tagset->getAttributeIndex(key) + 1;
        if (attributeIndex == -1) {
            std::cerr << "No such attribute: " << key << "." << std::endl;
            return false;
        }
        std::vector<char> values = tagset->mapAttributeValuesMatching(key, value);
        if (values.size() == 0) {
            std::cerr << "No values of " << key << " matching regular expression: '" << value << "'." << std::endl;
            return false;
        }
        //@todo: tu tez warto jakis join dac
        std::vector<char>::iterator i = values.begin();
        mapped = "";
        while (i != values.end()) {
            mapped += *i;
            i ++;
        }
        if (comparisonOperator != "!~")
            if (values.size() > 1)
                mapped = "[" + mapped + "]";
    } else {
        if (tagset->checkAttribute(key)) {
            if (tagset->checkAttributeValue(key, value)) {
                std::pair<char, int> pair = tagset->mapAttributeValue(key, value);
                mapped = pair.first;
                attributeIndex = pair.second + 1; // bo pierwsza w 'tablicy' interpretacji jest czesc mowy!
            } else {
                std::cerr << "Unknown value: " << value << " of the attribute: " << key << "." << std::endl;
                return false;
            }
        } else {
            std::cerr << "No such attribute: " << key << "." << std::endl;
            return false;
        }
    }
    if (comparisonOperator == "~~") {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end()) {
            (*i)[attributeIndex] = mapped;
            i ++;
        }
    } else if (comparisonOperator == "~") {
        if (interps.size() == 1) {
            TokenMask currentInterpretation = interps.front();
            currentInterpretation[modifierIndex] = "*";
            (interps.front())[attributeIndex] = mapped;
            (interps.front())[modifierIndex] = "()";
            interps.push_back(currentInterpretation);
            interps.insert(interps.begin(), currentInterpretation);
        } else {
            Interpretations::iterator i = interps.begin() + 1;
            (*i)[attributeIndex] = mapped;
        }
    }
    else if (comparisonOperator == "!~") {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end()) {
            (*i)[attributeIndex] = "[^" + mapped;
            (*i)[attributeIndex] += "]";
            i ++;
        }
    } else {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return false;
    }

    return true;
}*/

std::string RuleLoader::compileOrthCondition(std::string &comparisonOperator,
        std::string &value, bool icase) {
//    value = boost::regex_replace(value, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regLt, "\\&lt;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regGt, "\\&gt;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regAlt, "\\&bar;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
//    value = boost::regex_replace(value, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);
    RE2::GlobalReplace(&value, *regAmp, "\\&amp;");
    RE2::GlobalReplace(&value, *regLt, "\\&lt;");
    RE2::GlobalReplace(&value, *regGt, "\\&gt;");
    RE2::GlobalReplace(&value, *regLPar, "\\&lpar;");
    RE2::GlobalReplace(&value, *regRPar, "\\&rpar;");
    RE2::GlobalReplace(&value, *regAlt, "\\&bar;");
    RE2::GlobalReplace(&value, *regPlus, "\\&plus;");
    RE2::GlobalReplace(&value, *regAsterisk, "\\&astr;");
    RE2::GlobalReplace(&value, *regOpt, "\\&qmark;");

    if (icase) {
        value = "(?i:" + value + ")";
//        std::string valU = value;
//        boost::to_upper(valU);
//        std::string valL = value;
//        boost::to_lower(valL);
////        UnicodeString valU = icuConverter::fromUtf(value);
////        UnicodeString valL = icuConverter::fromUtf(value);
////        valU.toUpper();
////        valL.toLower();
////        StringCharacterIterator itU(valU);
////        StringCharacterIterator itL(valL);
//        std::stringstream ss;
//        for (int i = 0; i < valL.size() && i << valU.size(); i ++) {
//            if (valU[i] != valL[i])
//                ss << "[" << valU[i] << valL[i] << "]";
//            else
//                ss << valU[i];
//        }
////        while (itU.hasNext())
////        {
////            UnicodeString tu = itU.current();
////            UnicodeString tl = itL.current();
////            if (tu != tl)
////                ss << "[" << icuConverter::toUtf(tu) << icuConverter::toUtf(tl) << "]";
////            else
////                ss << icuConverter::toUtf(tu);
////            itU.next();
////            itL.next();
////        }
//        value = ss.str(); /*
//        std::string valueU = value;
//        std::string valueL = value;
//        std::transform(value.begin(), value.end(), valueU.begin(), ::toupper);
//        std::transform(value.begin(), value.end(), valueL.begin(), ::tolower);
////        std::cout << "UC: " << valueU << std::endl;
////        std::cout << "LC: " << valueL << std::endl;
//        std::wstring wideU = utf8converter::FromUtf8(valueU);
//        std::wstring wideL = utf8converter::FromUtf8(valueL);
//        int j = 0;
//        while (wideU.at(j))
//        {
//            std::wstring tu(1, wideU.at(j));
//            std::wstring tl(1, wideL.at(j));
//            ss << "[" << utf8converter::ToUtf8(tu) << utf8converter::ToUtf8(tl) << "]";
//            j ++;
//        }
//        //value = "";
////        for (int i = 0; i < valueU.length(); i++)
////            ss << "[" << valueU[i] << valueL[i] << "]";
//        value = ss.str();*/
    }

    if (value.find_first_of(".|*+?") != std::string::npos)
    {
        std::string tmpVal = "";
        int idx = 0;
        int pos;
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

//    boost::u32regex regLpar = boost::make_u32regex("\\\\\\\\\\\\\\(");
//    boost::u32regex regRpar = boost::make_u32regex("\\\\\\\\\\\\\\)");
//    value = boost::u32regex_replace(value, regLpar, "\\&lpar;", boost::match_default | boost::format_sed);
//    value = boost::u32regex_replace(value, regRpar, "\\&rpar;", boost::match_default | boost::format_sed);
    RE2 regLpar("\\\\\\\\\\\\\\(");
    RE2 regRpar("\\\\\\\\\\\\\\)");
    RE2::GlobalReplace(&value, regLpar, "\\&lpar;");
    RE2::GlobalReplace(&value, regRpar, "\\&rpar;");

    std::string result;
    if ((comparisonOperator == "~") || (comparisonOperator == "~~"))
        result = value;
    else if (comparisonOperator == "!~")
        result = "(?!" + value + ")[^<>]+";
    else
    {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return "";
    }
    return result;
}

std::string RuleLoader::generateTokenPatternsString(TokenPatterns tokenPatterns,
        NegativePatternStrings &negativePatterns) {
    //std::string result = "";
    std::stringstream ss;
    for (TokenPatterns::iterator patternIt = tokenPatterns.begin();
            patternIt != tokenPatterns.end(); patternIt ++) {
        std::string basePattern = "[^<>]+";
        std::string morphoPattern = "";
        std::string pattern = "";
        if (patternIt->base.condition != "") {
            if (patternIt->base.negative) {
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
            } else {
                basePattern = patternIt->base.condition; //@todo: nie trzeba owinac w nawiasy jeszcze tego?
            }
        }
        for (std::vector<TokenPatternPart>::iterator partIt =
                patternIt->parts.begin();
                partIt != patternIt->parts.end(); partIt ++) {
            if (partIt->condition != "") {
                if (morphoPattern != "") {
                    morphoPattern += "(:[^:<>]+)*";
                } else {
                    morphoPattern = "[^:<>]+";
                }
                if (partIt->negative) {
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
    if (tokenPatterns.size() == 1)
        result = "(" + result + ")";
    return result;
//    Interpretations::iterator i = interps.begin();
//    while (i != interps.end())
//    {
//        std::string interpretation = "";
//        int j = 0;
//        while (j < modifierIndex)
//        {
//            interpretation += (*i)[j];
//            j ++;
//        }
//        interpretation = "<" + interpretation;
//        if ((*i)[modifierIndex] != "")
//        {
//            if ((*i)[modifierIndex] == "()")
//            {
//                interpretation = "(" + interpretation + ")";
//            }
//            else if ((*i)[modifierIndex] == "*")
//            {
//                interpretation = "(" + interpretation + ")*";
//            }
//            else if ((*i)[modifierIndex] == "+")
//            {
//                interpretation = "(" + interpretation + ")+";
//            }
//            else if ((*i)[modifierIndex] == "?")
//            {
//                interpretation = "(" + interpretation + ")?";
//            }
//        }
//        result += interpretation;
//        i ++;
//    }
//    if (interps.size() == 1)
//        result = "(" + result + ")+";
//
//    if (result.find(nothingSet) != std::string::npos)
//        //result = boost::u32regex_replace(result, regNothingSet, "[^<>]+", boost::match_default | boost::format_sed);
//        RE2::GlobalReplace(&result, *regNothingSet, "[^<>]+");
//    //result = boost::u32regex_replace(utf8converter::ToUtf8(utf8converter::FromUtf8(result)), regNothingSet, "[^<>]+", boost::match_default | boost::format_sed);
//    //64: result = boost::u32regex_replace(result, regNothingSet, "[^<>]+", boost::match_default | boost::format_sed);
//
//    return result;
}

//std::string RuleLoader::interpretationsToString(Interpretations interps) {
//    std::string result = "";
//    Interpretations::iterator i = interps.begin();
//    while (i != interps.end())
//    {
//        std::string interpretation = "";
//        int j = 0;
//        while (j < modifierIndex)
//        {
//            interpretation += (*i)[j];
//            j ++;
//        }
//        interpretation = "<" + interpretation;
//        if ((*i)[modifierIndex] != "")
//        {
//            if ((*i)[modifierIndex] == "()")
//            {
//                interpretation = "(" + interpretation + ")";
//            }
//            else if ((*i)[modifierIndex] == "*")
//            {
//                interpretation = "(" + interpretation + ")*";
//            }
//            else if ((*i)[modifierIndex] == "+")
//            {
//                interpretation = "(" + interpretation + ")+";
//            }
//            else if ((*i)[modifierIndex] == "?")
//            {
//                interpretation = "(" + interpretation + ")?";
//            }
//        }
//        result += interpretation;
//        i ++;
//    }
//    if (interps.size() == 1)
//        result = "(" + result + ")+";
//
//    if (result.find(nothingSet) != std::string::npos)
//        //result = boost::u32regex_replace(result, regNothingSet, "[^<>]+", boost::match_default | boost::format_sed);
//        RE2::GlobalReplace(&result, *regNothingSet, "[^<>]+");
//        //result = boost::u32regex_replace(utf8converter::ToUtf8(utf8converter::FromUtf8(result)), regNothingSet, "[^<>]+", boost::match_default | boost::format_sed);
//        //64: result = boost::u32regex_replace(result, regNothingSet, "[^<>]+", boost::match_default | boost::format_sed);
//
//    return result;
//}

ActionsPtr RuleLoader::compileRuleAction(std::string &matched, int ruleLeftSize,
        int ruleMatchSize, int ruleRightSize, std::string ruleName, bool &repeat) {//,
//        LatticeWrapperPtr latticeWrapper) {

//    boost::u32regex regEval = boost::make_u32regex("^Eval\\s*:\\s*");
//    boost::u32regex regGroup = boost::make_u32regex("group\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // group(XX, 1);
//    //boost::regex regDelete("delete\\s*\\(\\s*([^\\s!~\"]+)\\s*([!~]?~)\\s*\"([^\\s\",]+)\"\\s*,\\s*(\\d+)\\s*\\)\\s*");    //delete(pos!~"X", 1)
//    boost::u32regex regDelete = boost::make_u32regex("delete\\s*\\(\\s*([^,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
//    boost::u32regex regAdd = boost::make_u32regex("add\\s*\\(\\s*\"([^\\s\",]+)\"\\s*,\\s*((\"[^\\s\",]+\")|(base))\\s*,\\s*(\\d+)\\s*\\)\\s*");                                       //add("X:m:sg", 1);
//    boost::u32regex regUnify = boost::make_u32regex("unify\\s*\\(\\s*([^,]+)\\s*((,\\s*(\\d+)\\s*)+)\\)\\s*");                                           //unify(gender number case, 1, 2, 3);
////    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*([^,]+)\\s*,\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
//    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
//    boost::u32regex regJoin = boost::make_u32regex("join\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // join(XX, 1);
//    boost::u32regex regAttach = boost::make_u32regex("attach\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // attach(XX, 1);
//    boost::u32regex regTransform = boost::make_u32regex("transform\\s*\\(\\s*(\\d+)\\s*,\\s*([^\\s,]+)\\s*\\)\\s*");                                         // transform(1, YY);
//    boost::u32regex regRepeat = boost::make_u32regex("repeat");                                         // repeat;
//    boost::u32regex regWhite = boost::make_u32regex("\\s+");
//    boost::u32regex regActionSeparator = boost::make_u32regex("\\s*;\\s*");
    RE2 regEval("^Eval\\s*:\\s*");
    // group(NP, 1);
    RE2 regGroup("group\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //boost::regex regDelete("delete\\s*\\(\\s*([^\\s!~\"]+)\\s*([!~]?~)\\s*\"([^\\s\",]+)\"\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //delete(pos!~"adj", 1)
    RE2 regDelete("delete\\s*\\(\\s*([^,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //add("adj:sg:m", 1);
    RE2 regAdd("add\\s*\\(\\s*\"([^\\s\",]+)\"\\s*,\\s*((\"[^\\s\",]+\")|(base))\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //unify(gender number case, 1, 2, 3);
    RE2 regUnify("unify\\s*\\(\\s*([^,]+)\\s*((,\\s*(\\d+)\\s*)+)\\)\\s*");
//    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*([^,]+)\\s*,\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
    //syntok("coup d'état");
    RE2 regSyntok("syntok\\s*\\(\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
    //join(NP, 1);
    RE2 regJoin("join\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //attach(NP, 1);
    RE2 regAttach("attach\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //transform(1, VP);
    RE2 regTransform("transform\\s*\\(\\s*(\\d+)\\s*,\\s*([^\\s,]+)\\s*\\)\\s*");
    //repeat
    RE2 regRepeat("repeat");
    RE2 regWhite("\\s+");
    std::string actionSeparator = ";";

    //std::cout << "Moje wyrazenia: " << std::endl << regGroup.str() << std::endl << regDelete.str() << std::endl << regAdd.str() << std::endl << regUnify.str() << std::endl;

    //matched = boost::u32regex_replace(matched, regEval, "", boost::match_default | boost::format_sed);
    RE2::Replace(&matched, regEval, "");

    ActionsPtr actions = ActionsPtr( new Actions() );

    //if ((matched == "") || (boost::u32regex_match(matched, regWhite)))
    if ((matched == "") || (RE2::FullMatch(matched, regWhite)))
    {
        std::cerr << "No actions defined in Eval section." << std::endl;
        return actions;//(Actions)NULL;
    }

//    repeat = false;

    std::vector<std::string> actionStrings;
    boost::split(actionStrings, matched, boost::is_any_of(actionSeparator));
//    boost::u32regex_token_iterator<std::string::iterator> i(matched.begin(), matched.end(), regActionSeparator, -1);
//    boost::u32regex_token_iterator<std::string::iterator> j;
//    while (i != j)
    for (std::vector<std::string>::iterator asit = actionStrings.begin();
            asit != actionStrings.end(); asit ++) {
        //std::string actionString = *i;
        std::string actionString = boost::algorithm::trim_copy(*asit);
        if ((actionString == "") || (RE2::FullMatch(actionString, regWhite)))
            continue;
        //boost::smatch container;
        std::string groupType;
        int groupHead;
        int tokenIndex;
        std::string conditionsString;
        std::string attrs;
        std::string toks;
        std::string baseString;
        std::string interpretation;
        std::string mask;
        int slot1, slot2; //@todo: typy!
        //if (boost::u32regex_match(actionString, container, regRepeat)) {
        if (RE2::FullMatch(actionString, regRepeat)) {
            repeat = true;
        //} else if (boost::u32regex_match(actionString, container, regGroup)) {
        } else if (RE2::FullMatch(actionString, regGroup, &groupType, &groupHead)) {
            //GroupActionPtr action = GroupActionPtr( new GroupAction(container[1], ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1), boost::lexical_cast<int>(container[2]), ruleName) );
            GroupActionPtr action = GroupActionPtr( new GroupAction(
                        groupType, ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1),
                        groupHead, ruleName ) ); //, latticeWrapper ) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
        } else if (RE2::FullMatch(actionString, regJoin, &groupType, &groupHead)) {
            //JoinActionPtr action = JoinActionPtr( new JoinAction(container[1], ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1), boost::lexical_cast<int>(container[2]), ruleName) );
            JoinActionPtr action = JoinActionPtr( new JoinAction(
                        groupType, ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1),
                        groupHead, ruleName ) ); //, latticeWrapper ) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
        //} else if (boost::u32regex_match(actionString, container, regAttach)) {
        } else if (RE2::FullMatch(actionString, regAttach, &groupType, &groupHead)) {
            //AttachActionPtr action = AttachActionPtr( new AttachAction(container[1], ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1), boost::lexical_cast<int>(container[2]), ruleName) );
            AttachActionPtr action = AttachActionPtr( new AttachAction(
                        groupType, ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1),
                        groupHead, ruleName ) ); //, latticeWrapper ) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
        } else if (RE2::FullMatch(actionString, regTransform, &groupHead, &groupType)) {
            TransformActionPtr action = TransformActionPtr( new TransformAction(
                        groupType, groupHead, ruleName ) ); //, latticeWrapper ) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
        //} else if (boost::u32regex_match(actionString, container, regDelete)) {
        } else if (RE2::FullMatch(actionString, regDelete,
                    &conditionsString, &tokenIndex)) {
            //int tokenIndex = boost::lexical_cast<int>(container[2]);
            //std::string conditions = container[1];
            std::string uncompiled = conditionsString;
            //TokenMask mask = baseMask;
            //std::string orth = "[^<>]+";
            DeleteConditions conditions;

            while (conditionsString != "") {
                std::string key = getKey(conditionsString);
                std::string compOperator = getOperator(conditionsString);
                std::string value = getValue(conditionsString);
                //compileDeleteCondition(key, compOperator, value, mask, orth);
                compileDeleteCondition(key, compOperator, value, conditions);
                if (conditionsString.find("&&") == 0) {
                    conditionsString = conditionsString.substr(2, std::string::npos);
                }
            }

            //std::string condition = "";
            //TokenMask::iterator i = mask.begin();
            //while (i != mask.end()) {
            //    condition += *i;
            //    i ++;
            //}
            //condition = orth + "<" + condition;

//            std::string key = container[1];
//            std::string comparisonOperator = container[2];
//            std::string value = container[3];
//            int tokenIndex = boost::lexical_cast<int>(container[4]);
//            std::string orth = "";
//            Interpretations interps;
//            interps.push_back(baseMask);
//            std::string condition;
//            if (key == "pos")
//            {
//                compilePosCondition(comparisonOperator, value, interps);
//                condition = interpretationsToString(interps);
//            }
//            else if (key == "base")
//            {
//                compileBaseCondition(comparisonOperator, value, interps);
//                condition = interpretationsToString(interps);
//            }
//            else if (key == "orth")
//            {
//                orth = compileOrthCondition(comparisonOperator, value);
//                condition = orth;
//            }
//            else
//            {
//                compileAttributeCondition(key, comparisonOperator, value, interps);
//                condition = interpretationsToString(interps);
//            }
            //DeleteActionPtr action = DeleteActionPtr( new DeleteAction(
            //            condition, tokenIndex, uncompiled ) );
            DeleteActionPtr action = DeleteActionPtr( new DeleteAction(
                        conditions, tokenIndex, uncompiled ) ); //, latticeWrapper ) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
 //           std::cout << "Usun interpretacje: " << container[4] << " pod warunkiem, ze: " << container[1] << " " << container[2] << " " << container[3] << std::endl;
        //} else if (boost::u32regex_match(actionString, container, regAdd)) {
        } else if (RE2::FullMatch(actionString, regAdd, &interpretation,
                    &baseString, &slot1, &slot2, &tokenIndex)) { //@todo: dlaczemu nie mozna uzyc NULL zamiast slotX?
            //int tokenIndex = boost::lexical_cast<int>(container[5]);
            //std::string base = container[2];
            std::string base = baseString;
            if (base == "base") {
                base = "[^<>]+";
            } else {
                base = base.substr(1, base.length() - 2); //usuniecie cudzyslowow wokol formy bazowej
            }

            //std::vector<InterpretationPair> interpretations; // = compileAddInterpretations(container[1]);
            std::vector<std::string> interpretations;
            //std::string interpretation = container[1];
            compileAddInterpretation(interpretation, interpretations);

            AddActionPtr action = AddActionPtr( new AddAction(
                        interpretations, base, tokenIndex, interpretation ) );
//                        latticeWrapper) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);

//            std::cout << "Dodaj interpretacje: " << container[1] << " do: " << container[2] << std::endl;
        //} else if (boost::u32regex_match(actionString, container, regUnify)) {
        } else if (RE2::FullMatch(actionString, regUnify, &attrs, &toks)) {
            //std::vector<int> attributes;
            std::vector<std::string> patterns;
            std::vector<std::string> attributes;
            std::vector<int> tokens;
            std::vector<std::string> attributes_;
            //std::string attrs = container[1];
            //std::string toks = container[2];
            //std::string mask;
            //mask.assign(tagset->getNumberOfAttributes() + 1, '.');
            std::vector<std::string> attributesVector;
            boost::split(attributesVector, attrs, boost::is_any_of(" "));
            for (std::vector<std::string>::iterator ait = attributesVector.begin();
                    ait != attributesVector.end(); ait ++) {
            //boost::u32regex_token_iterator<std::string::iterator> a(attrs.begin(), attrs.end(), regWhite, -1);
            //boost::u32regex_token_iterator<std::string::iterator> b;
            //while (a != b) {
                //std::string aa = *a; //TODO: jakies rzutowanie tego nie zalatwi?
                std::string aa = boost::algorithm::trim_copy(*ait);
                if (tagset->checkAttribute(aa)) {
                    //attributes.push_back(tagset->getAttributeIndex(aa));
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
              //  a ++;
            }
            //boost::u32regex regTokenSeparator = boost::make_u32regex("\\s*,\\s*");
            std::string tokenSeparator = ",";
            std::vector<std::string> tokensVector;
            boost::split(tokensVector, toks, boost::is_any_of(tokenSeparator));
            for (std::vector<std::string>::iterator tit = tokensVector.begin();
                    tit != tokensVector.end(); tit ++) {
            //boost::u32regex_token_iterator<std::string::iterator> t(toks.begin(), toks.end(), regTokenSeparator, -1);
            //boost::u32regex_token_iterator<std::string::iterator> u;
            //while (t != u) {
                if (*tit != "") {
                    tokens.push_back(boost::lexical_cast<int>(*tit));
                }
            //    t ++;
            }

            //UnifyActionPtr action = UnifyActionPtr( new UnifyAction(
            //            attributes, tokens, attributes_ ) );
            UnifyActionPtr action = UnifyActionPtr( new UnifyAction(
                        patterns, attributes, tokens, attributes_ ) );
//                        latticeWrapper ) );
            if (verbose)
                action->setVerbose();

            action->rule = ruleName;
            //action->setBaseMask(mask);
            actions->push_back(action);
        //} else if (boost::u32regex_match(actionString, container, regWord)){
        } else if (RE2::FullMatch(actionString, regSyntok, &mask)) {
            std::vector<int> tokens;
//            std::string toks = container[1];
            //std::string mask = container[1];

            for (int x = ruleLeftSize; x < (ruleLeftSize + ruleMatchSize); x ++) {
                tokens.push_back(x + 1);
            }
//            boost::u32regex regTokenSeparator = boost::make_u32regex("\\s*\\.\\s*");
//            boost::u32regex_token_iterator<std::string::iterator> t(toks.begin(), toks.end(), regTokenSeparator, -1);
//            boost::u32regex_token_iterator<std::string::iterator> u;
//            while (t != u)
//            {
//                if (*t != "")
//                {
//                    tokens.push_back(boost::lexical_cast<int>(*t));
//                }
//                t ++;
//            }
            //std::vector<InterpretationPair> interpretations; // = compileAddInterpretations(container[1]);
            std::vector<std::string> interpretations;
            compileAddInterpretation(mask, interpretations);

            SyntokActionPtr action = SyntokActionPtr( new SyntokAction(
                        ruleLeftSize, ruleLeftSize + ruleMatchSize - 1, tokens,
                        interpretations, ruleName, mask ) ); //, latticeWrapper ) );
            if (verbose)
                action->setVerbose();
            if (syntok)
                action->setSyntok();
             //   ((SyntokAction*)action)->setSyntok();

            actions->push_back(action);

        } else {
            std::cerr << "Unknown action: " << actionString << "." << std::endl;
        }
//        i ++;
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
            if (values.size() == 0) {
                std::cerr << "No parts of speech identifiers matching the regular expression: \"" << value << "\"." << std::endl;
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
            if (values.size() == 0) {
                std::cerr << "No values of " << key << " matching the regular expression: \"" << value << "\"." << std::endl;
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

//bool RuleLoader::compileDeleteCondition(std::string &key, std::string &comparisonOperator, std::string &value, TokenMask &mask, std::string &orth)
//{
//    std::string mapped;
//    int attributeIndex;
//    if (key == "pos")
//    {
//        if (value.find_first_of(".|*+?") != std::string::npos)
//        {
//            std::vector<char> values = tagset->mapPosMatching(value);
//            if (values.size() == 0)
//            {
//                std::cerr << "No parts of speech identifiers matching regular expression: \"" << value << "\"." << std::endl;
//                return false;
//            }
//            mapped = "";
//            std::vector<char>::iterator i = values.begin();
//            while (i != values.end())
//            {
//                mapped += *i;
//                i ++;
//            }
//            if (comparisonOperator != "!~")
//                mapped = "[" + mapped + "]";
//        }
//        else
//        {
//            if (!tagset->checkPos(value))
//            {
//                std::cerr << "Unknown part of speech: " << value << "." << std::endl;
//                return false;
//            }
//            mapped = tagset->mapPos(value);
//        }
//        attributeIndex = 0;
//    }
//    else if (key == "base")
//    {
//        if ((comparisonOperator == "~~") || (comparisonOperator == "~"))
//        {
//            mapped = value;
//        }
//        else if (comparisonOperator == "!~")
//        {
//            mapped = "(?!" + value + ")[^<>]+";
//            mask[baseIndex] = mapped;
//            return true;
//        }
//        else
//        {
//            std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
//            return false;
//        }
//        attributeIndex = baseIndex;
//    }
//    else if (key == "orth")
//    {
//        if ((comparisonOperator == "~~") || (comparisonOperator == "~"))
//        {
//            orth = value;
//        }
//        else if (comparisonOperator == "!~")
//        {
//            orth = "(?!" + value + ")[^<>]+";
//        }
//        else
//        {
//            std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
//            return false;
//        }
//        return true;
//    }
//    else
//    {
//        if (value.find_first_of(".|*+?") != std::string::npos)
//        {
//            attributeIndex = tagset->getAttributeIndex(key) + 1;
//            if (attributeIndex == -1)
//            {
//                std::cerr << "No such attribute: " << key << "." << std::endl;
//                return false;
//            }
//            std::vector<char> values = tagset->mapAttributeValuesMatching(key, value);
//            if (values.size() == 0)
//            {
//                std::cerr << "No values of " << key << " matching regular expression: \"" << value << "\"." << std::endl;
//                return false;
//            }
//            std::vector<char>::iterator i = values.begin();
//            mapped = "";
//            while (i != values.end())
//            {
//                mapped += *i;
//                i ++;
//            }
//            if (comparisonOperator != "!~")
//                if (values.size() > 1)
//                    mapped = "[" + mapped + "]";
//        }
//        else
//        {
//            if (tagset->checkAttribute(key))
//            {
//                if (tagset->checkAttributeValue(key, value))
//                {
//                    std::pair<char, int> pair = tagset->mapAttributeValue(key, value);
//                    mapped = pair.first;
//                    attributeIndex = pair.second + 1; // bo pierwsza w 'tablicy' interpretacji jest czesc mowy!
//                }
//                else
//                {
//                    std::cerr << "Unknown value: " << value << " of the attribute: " << key << "." << std::endl;
//                    return false;
//                }
//            }
//            else
//            {
//                std::cerr << "No such attribute: " << key << "." << std::endl;
//                return false;
//            }
//        }
//    }
//    if ((comparisonOperator == "~~") || (comparisonOperator == "~"))
//    {
//        mask[attributeIndex] = mapped;
//    }
//    else if (comparisonOperator == "!~")
//    {
//        if ((attributeIndex != 0) && (attributeIndex != baseIndex))
//            mask[attributeIndex] = "[^0" + mapped;
//        else
//            mask[attributeIndex] = "[^" + mapped;
//        mask[attributeIndex] += "]";
//    }
//    else
//    {
//        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
//        return false;
//    }
//    return true;
//}

//bool RuleLoader::compileAddInterpretation(std::string &pattern, std::vector<InterpretationPair> &interpretations)
bool RuleLoader::compileAddInterpretation(std::string &pattern,
        std::vector<std::string> &interpretations) {
    //boost::u32regex regFieldSeparator = boost::make_u32regex(":");
    std::string fieldSeparator = ":";
    std::vector<std::string> partsVector;
    boost::split(partsVector, pattern, boost::is_any_of(fieldSeparator));
    //boost::u32regex_token_iterator<std::string::iterator> i(pattern.begin(), pattern.end(), regFieldSeparator, -1);
    //boost::u32regex_token_iterator<std::string::iterator> j;
    int index = 0;
    std::string pos;
    std::vector<std::string> morphologies;
    //while (i != j)
    for (std::vector<std::string>::iterator it = partsVector.begin();
            it != partsVector.end(); it ++) {
        std::string value = *it;
        if (index == 0)
        {
            if (tagset->checkPos(value))
            {
                morphologies.push_back(value);
                pos = value;
            }
            else
            {
                std::cerr << "Unknown part of speech: " << value << "." << std::endl;
                return false;
            }
        }
        else
        {
            std::string attribute = tagset->getAttributeAtIndex(pos, index - 1); //pierwsza jest czesc mowy, stad numeracja jest o jeden wieksza
            std::vector<std::string> values;
            if (value == "*")
            {
                values = tagset->getAttributeValues(attribute);
            }
            else if (value.find(".") != std::string::npos)
            {
                while (value.find(".") != std::string::npos)
                {
                    int pos = value.find(".");
                    std::string val = value.substr(0,pos);
                    if (tagset->checkAttributeValue(attribute, val))
                    {
                        values.push_back(val);
                        value = value.substr(pos + 1, std::string::npos);
                    }
                    else
                    {
                        std::cerr << "Unknown value: " << value << " of attribute: " << attribute << "." << std::endl;
                        return false;
                    }
                }
                if (tagset->checkAttributeValue(attribute, value))
                    values.push_back(value);
                else
                {
                    std::cerr << "Unknown value: " << value << " of attribute: " << attribute << "." << std::endl;
                    return false;
                }
            }
            else
            {
                if (tagset->checkAttributeValue(attribute, value))
                    values.push_back(value);
                else
                {
                    std::cerr << "Unknown value: " << value << " of attribute: " << attribute << "." << std::endl;
                    return false;
                }
            }
            std::vector<std::string> newMorphologies;
            std::vector<std::string>::iterator v = values.begin();
            while (v != values.end())
            {
                std::vector<std::string>::iterator o = morphologies.begin();
                while (o != morphologies.end())
                {
                    newMorphologies.push_back(*o + ":" + *v);
                    o ++;
                }
                v ++;
            }
            morphologies = newMorphologies;

        }

        //i ++;
        index ++;
    }

    //@todo: czy to powyzej to jest jest optymalnie zrobione?

    for (std::vector<std::string>::iterator m = morphologies.begin();
            m != morphologies.end(); m ++) {
        std::string mapped = tagset->mapMorphology(*m);
        if (mapped != "") {
            interpretations.push_back(*m);
        } else {
            //empty mapped string means the morphology is not valid.
            std::cerr << "Morphology not valid: " << *m << std::endl;
            return false;
        }
    }
    /*std::vector<std::string>::iterator m = morphologies.begin();
    while (m != morphologies.end())
    {
        std::string mapped = tagset->mapMorphology(*m);
        if (mapped != "")
        {
            interpretations.push_back(std::pair<std::string, std::string>(*m, mapped));
        }
        else
        {
            std::cerr << "Improper morphology: " + *m + "." << std::endl;
            return false;
        }
        m ++;
    }*/

    return true;
}

void RuleLoader::setVerbose() {
    verbose = true;
}

void RuleLoader::setSyntok() {
    syntok = true;
}


RulePtr RuleLoader::compileRule(std::string ruleString) //, LatticeWrapperPtr latticeWrapper)
{
    RE2 regComment("#.*");
    RE2 regWhite("\\s+");
    std::string lineSep = "\n"; //@todo: ma byc tak czy krocej \n ?
    std::string ruleName;
    std::string rulePattern;
    std::string rulePatternLeft, rulePatternMatch, rulePatternRight;
    int ruleLeftSize, ruleMatchSize, ruleRightSize;
    ruleLeftSize = ruleMatchSize = ruleRightSize = 0;
    bool hasLeft = false;
    bool hasRight = false;
    int bracketCount = 0;

    std::vector<std::string> tokensPatterns;
    std::vector<std::string> tokensModifiers;
    std::vector<bool> tokensRequired;

    std::vector<int> matchedIndices;

    NegativePatternStrings negativePatterns;

    std::string chars = "";

    rulePattern = "";
    rulePatternLeft = "";
    rulePatternMatch = "";
    rulePatternRight = "";

    //boost::u32regex_token_iterator<std::string::iterator> i(ruleString.begin(), ruleString.end(), regLineSep, -1);
    //boost::u32regex_token_iterator<std::string::iterator> j;
    std::vector<std::string> linesVector;
    boost::split(linesVector, ruleString, boost::is_any_of(lineSep));
    //while (i != j)
    for (std::vector<std::string>::iterator line_it = linesVector.begin();
            line_it != linesVector.end(); line_it ++) {
        std::string line = *line_it;

        //line = boost::u32regex_replace(line, regComment, "", boost::match_default | boost::format_sed);
        RE2::Replace(&line, regComment, "");
        if (line == "")
            continue;
        //if (boost::u32regex_match(line, regWhite))
        if (RE2::FullMatch(line, regWhite))
            continue;

        line = boost::algorithm::trim_copy(line);
        if (line.find("Rule") == 0)
        {
            ruleName = this->compileRuleName(line);
            //std::cout << "Nazwa: " << line << std::endl;

            //rule.setName(ruleName);
        }
        else if (line.find("Left:") == 0)
        {
            if (chars != "")
            {
                std::cerr << "Unexpected characters before Left context declaration: " << chars << std::endl;
                chars = "";
            }
            chars += line;
            hasLeft = true;
        }
        else if (line.find("Match:") == 0)
        {
            if (chars != "")
            {
                if (hasLeft)
                {
                    //std::cout << "Kontekst lewy: " << chars << std::endl;
                    rulePatternLeft = chars;
                    rulePattern = this->compileRulePattern(chars, ruleLeftSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
                    //rulePatternLeft = this->compileRuleMatch(chars, ruleLeftSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePattern = rulePatternLeft;
                    chars = "";
                }
                else
                {
                    std::cerr << "Unexpected characters before Match declaration: " << chars << std::endl;
                    chars = "";
                }
            }
            chars += line;
            hasRight = false;
        }
        else if (line.find("Right:") == 0)
        {
            if (chars != "")
            {
                //std::cout << "Maczo: " << chars << std::endl;
                rulePatternMatch = chars;
                rulePattern += this->compileRulePattern(chars, ruleMatchSize,
                        tokensPatterns, tokensModifiers, tokensRequired,
                        matchedIndices, bracketCount, negativePatterns);
                //rulePatternMatch = this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                //rulePattern += rulePatternMatch;
                chars = "";
            }
            chars += line;
            hasRight = true;
        }
        else if (line.find("Eval:") == 0)
        {
            if (chars != "")
            {
                if (!hasRight)
                {
                    //std::cout << "Maczo: " << chars << std::endl;
                    //std::cout << "Taki macz: " << chars << std::endl;
                    rulePatternMatch = chars;
                    rulePattern += this->compileRulePattern(chars, ruleMatchSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
                    //rulePatternMatch = this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePattern += rulePatternMatch;
                    //std::cout << "taki wzorze: " << rulePattern << std::endl;
                }
                else
                {
                    //std::cout << "Kontekst prawy: " << chars << std::endl;
                    rulePatternRight = chars;
                    rulePattern += this->compileRulePattern(chars, ruleRightSize,
                            tokensPatterns, tokensModifiers, tokensRequired,
                            matchedIndices, bracketCount, negativePatterns);
                    //rulePatternRight = this->compileRuleMatch(chars, ruleRightSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePattern += rulePatternRight;
                }
                chars = "";
            }
            chars += line;
        }
        else
        {
            chars += line;
        }
        //i ++;
    }
    //std::cout << "Akcje: " << chars << std::endl;
    bool repeat = false;
    ActionsPtr actions = this->compileRuleAction(chars, ruleLeftSize,
            ruleMatchSize, ruleRightSize, ruleName, repeat); //, latticeWrapper);
    //Rule *rule = new Rule(ruleName, rulePattern, ruleLeftSize, ruleMatchSize, ruleRightSize, *actions, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, repeat, rulePatternLeft, rulePatternMatch, rulePatternRight);
    RulePtr rule = RulePtr( new Rule(ruleName, rulePattern, ruleLeftSize,
                ruleMatchSize, ruleRightSize, actions, tokensPatterns,
                tokensModifiers, tokensRequired, matchedIndices,
                repeat, rulePatternLeft, rulePatternMatch, rulePatternRight,
                negativePatterns ) );
//    delete actions;
    //rule.setActions...
    return rule;
}

ActionPtr RuleLoader::compileAction(std::string actionString, RulePtr rule) {
//        LatticeWrapperPtr latticeWrapper) {
//    boost::u32regex regEval = boost::make_u32regex("^Eval\\s*:\\s*");
//    boost::u32regex regGroup = boost::make_u32regex("group\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // group(XX, 1);
//    //boost::regex regDelete("delete\\s*\\(\\s*([^\\s!~\"]+)\\s*([!~]?~)\\s*\"([^\\s\",]+)\"\\s*,\\s*(\\d+)\\s*\\)\\s*");    //delete(pos!~"X", 1)
//    boost::u32regex regDelete = boost::make_u32regex("delete\\s*\\(\\s*([^,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
//    boost::u32regex regAdd = boost::make_u32regex("add\\s*\\(\\s*\"([^\\s\",]+)\"\\s*,\\s*((\"[^\\s\",]+\")|(base))\\s*,\\s*(\\d+)\\s*\\)\\s*");                                       //add("X:m:sg", 1);
//    boost::u32regex regUnify = boost::make_u32regex("unify\\s*\\(\\s*([^,]+)\\s*((,\\s*(\\d+)\\s*)+)\\)\\s*");                                           //unify(gender number case, 1, 2, 3);
////    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*([^,]+)\\s*,\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
//    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
//    boost::u32regex regJoin = boost::make_u32regex("join\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // join(XX, 1);
//    boost::u32regex regAttach = boost::make_u32regex("attach\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // attach(XX, 1);
//    boost::u32regex regTransform = boost::make_u32regex("transform\\s*\\(\\s*(\\d+)\\s*,\\s*([^\\s,]+)\\s*\\)\\s*");                                         // transform(1, YY);
//    boost::u32regex regRepeat = boost::make_u32regex("repeat");                                         // repeat;
//    boost::u32regex regWhite = boost::make_u32regex("\\s+");
//    boost::u32regex regActionSeparator = boost::make_u32regex("\\s*;\\s*");
    RE2 regEval("^Eval\\s*:\\s*");
    // group(XX, 1);
    RE2 regGroup("group\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //delete(pos!~"X", 1)
    RE2 regDelete("delete\\s*\\(\\s*([^,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //add("X:m:sg", 1);
    RE2 regAdd("add\\s*\\(\\s*\"([^\\s\",]+)\"\\s*,\\s*((\"[^\\s\",]+\")|(base))\\s*,\\s*(\\d+)\\s*\\)\\s*");
    //unify(gender number case, 1, 2, 3);
    RE2 regUnify("unify\\s*\\(\\s*([^,]+)\\s*((,\\s*(\\d+)\\s*)+)\\)\\s*");
    //syntok("coup d'état");
    RE2 regSyntok("syntok\\s*\\(\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
    // join(XX, 1);
    RE2 regJoin("join\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    // attach(XX, 1);
    RE2 regAttach("attach\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    // transform(1, YY);
    RE2 regTransform("transform\\s*\\(\\s*(\\d+)\\s*,\\s*([^\\s,]+)\\s*\\)\\s*");
    // repeat;
    RE2 regRepeat("repeat");
    RE2 regWhite("\\s+");
    std::string actionSeparator = ";";

    ActionPtr action;

    if (actionString[actionString.length() - 1] == ';')
        actionString = actionString.substr(0, actionString.length() - 1);

        std::string groupType;
        int groupHead;
        int tokenIndex;
        std::string conditionsString;
        std::string attrs;
        std::string toks;
        std::string baseString;
        std::string interpretation;
        std::string mask;
        int slot1, slot2; //@todo: typy!

//        boost::smatch container;
        if (RE2::FullMatch(actionString, regRepeat)) {
            //@todo: to co tu sie niby dzieje?
//            repeat = true;
        }
        else if (RE2::FullMatch(actionString, regGroup, &groupType, &groupHead)) {
            action = GroupActionPtr( new GroupAction(groupType,
                        rule->getLeftCount(),
                        (rule->getLeftCount() + rule->getMatchCount() - 1),
                        groupHead, rule->getName() ) ); //, latticeWrapper) );
            if (verbose)
                action->setVerbose();
        }
        else if (RE2::FullMatch(actionString, regJoin, &groupType, &groupHead)) {
            action = JoinActionPtr( new JoinAction(groupType,
                        rule->getLeftCount(),
                        (rule->getLeftCount() + rule->getMatchCount() - 1),
                        groupHead, rule->getName() ) );//, latticeWrapper) );
            if (verbose)
                action->setVerbose();
        }
        else if (RE2::FullMatch(actionString, regAttach, &groupType, &groupHead)) {
            action = AttachActionPtr(new AttachAction(groupType,
                        rule->getLeftCount(),
                        (rule->getLeftCount() + rule->getMatchCount() - 1),
                        groupHead, rule->getName() ) );//, latticeWrapper) );
            if (verbose)
                action->setVerbose();
        }
        else if (RE2::FullMatch(actionString, regTransform, &groupHead, &groupType)) {
            action = TransformActionPtr( new TransformAction(groupType,
                        groupHead, rule->getName() ) );//, latticeWrapper) );
            if (verbose)
                action->setVerbose();
        }
        else if (RE2::FullMatch(actionString, regDelete, &conditionsString, &tokenIndex)) {
            //int tokenIndex = boost::lexical_cast<int>(container[2]);
            //std::string conditions = container[1];
            std::string uncompiled = conditionsString;
            //TokenMask mask = baseMask;
            std::string orth = "[^<>]+";
            DeleteConditions conditions;

            while (conditionsString != "") {
                std::string key = getKey(conditionsString);
                std::string compOperator = getOperator(conditionsString);
                std::string value = getValue(conditionsString);
                //compileDeleteCondition(key, compOperator, value, mask, orth);
                compileDeleteCondition(key, compOperator, value, conditions);
                if (conditionsString.find("&&") == 0) {
                    conditionsString = conditionsString.substr(2, std::string::npos);
                }
            }

            //std::string condition = "";
            //TokenMask::iterator i = mask.begin();
            //while (i != mask.end()) {
            //    condition += *i;
            //    i ++;
            //}
            //condition = orth + "<" + condition;

            //action = DeleteActionPtr( new DeleteAction(condition, tokenIndex, uncompiled) );
            action = DeleteActionPtr( new DeleteAction(conditions, tokenIndex, uncompiled ) );//, latticeWrapper) );
            if (verbose)
                action->setVerbose();
 //           std::cout << "Usun interpretacje: " << container[4] << " pod warunkiem, ze: " << container[1] << " " << container[2] << " " << container[3] << std::endl;
        }
        else if (RE2::FullMatch(actionString, regAdd, &interpretation, &baseString, &slot1, &slot2, &tokenIndex)) {
            //int tokenIndex = boost::lexical_cast<int>(container[5]);
            //std::string base = container[2];
            std::string base = baseString;
            if (base == "base") {
                base = "[^<>]+";
            } else {
                base = base.substr(1, base.length() - 2); //usuniecie cudzyslowow wokol formy bazowej
            }

            //std::vector<InterpretationPair> interpretations; // = compileAddInterpretations(container[1]);
            std::vector<std::string> interpretations; // = compileAddInterpretations(container[1]);
            //std::string interpretation = container[1];
            compileAddInterpretation(interpretation, interpretations);

            action = AddActionPtr( new AddAction(interpretations, base, tokenIndex, interpretation ) );//, latticeWrapper) );
            if (verbose)
                action->setVerbose();

//            std::cout << "Dodaj interpretacje: " << container[1] << " do: " << container[2] << std::endl;
        }
        else if (RE2::FullMatch(actionString, regUnify, &attrs, &toks)) {
            //std::vector<int> attributes;
            std::vector<std::string> attributes;
            std::vector<std::string> patterns;
            std::vector<int> tokens;
            std::vector<std::string> attributes_;
            //std::string attrs = container[1];
            //std::string toks = container[2];
            //std::string mask;
            //mask.assign(tagset->getNumberOfAttributes() + 1, '.');
            std::vector<std::string> attributesVector;
            boost::split(attributesVector, attrs, boost::is_any_of(" "));
            //boost::u32regex_token_iterator<std::string::iterator> a(attrs.begin(), attrs.end(), regWhite, -1);
            //boost::u32regex_token_iterator<std::string::iterator> b;
            //while (a != b)
            for (std::vector<std::string>::iterator attr_it = attributesVector.begin();
                    attr_it != attributesVector.end(); attr_it ++) {
                std::string aa = boost::trim_copy(*attr_it);
                if (tagset->checkAttribute(aa)) {
                    //attributes.push_back(tagset->getAttributeIndex(aa));
                    attributes.push_back(aa);
                    std::vector<std::string> values =
                        tagset->getAttributeValues(aa);
                    std::string valuesString = boost::algorithm::join(values, "|");
                    std::string pattern = "^[^:]+(:[^:]+)*:(" + valuesString
                        + ")(:[^:]+)*$";
                    patterns.push_back(pattern);
                    attributes_.push_back(aa);
                } else {
                    std::cout << "Unknown attribute: " << *attr_it << "." << std::endl;
                }
                //a ++;
            }
            std::string tokenSeparator = ",";
            std::vector<std::string> tokensVector;
            boost::split(tokensVector, toks, boost::is_any_of(tokenSeparator));
            //boost::u32regex regTokenSeparator = boost::make_u32regex("\\s*,\\s*");
            //boost::u32regex_token_iterator<std::string::iterator> t(toks.begin(), toks.end(), regTokenSeparator, -1);
            //boost::u32regex_token_iterator<std::string::iterator> u;
            //while (t != u)
            for (std::vector<std::string>::iterator tok_it = tokensVector.begin();
                    tok_it != tokensVector.end(); tok_it ++) {
                if (*tok_it != "") {
                    tokens.push_back(boost::lexical_cast<int>(*tok_it));
                }
                //t ++;
            }

            //action = UnifyActionPtr( new UnifyAction(attributes, tokens, attributes_) );
            action = UnifyActionPtr(
                    new UnifyAction(patterns, attributes, tokens, attributes_ ) );
            if (verbose)
                action->setVerbose();

            (boost::dynamic_pointer_cast<UnifyAction>(action))->rule = rule->getName();
            //(boost::dynamic_pointer_cast<UnifyAction>(action))->setBaseMask(mask);
        }
        else if (RE2::FullMatch(actionString, regSyntok, &mask))
        {
            std::vector<int> tokens;
//            std::string toks = container[1];
            //std::string mask = container[1];

            for (int x = rule->getLeftCount(); x < (rule->getLeftCount() + rule->getMatchCount()); x ++) {
                tokens.push_back( x + 1);
            }
            //std::vector<InterpretationPair> interpretations; // = compileAddInterpretations(container[1]);
            std::vector<std::string> interpretations;
            compileAddInterpretation(mask, interpretations);

            action = SyntokActionPtr( new SyntokAction(rule->getLeftCount(), rule->getLeftCount() + rule->getMatchCount() - 1, tokens, interpretations, rule->getName(), mask ) );//, latticeWrapper) );
            if (verbose)
                action->setVerbose();
            if (syntok)
                (boost::dynamic_pointer_cast<SyntokAction>(action))->setSyntok();


        } else {
            std::cerr << "Unknown action: " << actionString << "." << std::endl;
        }
        return action;
}

}

}

}

