
#include "rule_compiler.hpp"

#include <iostream>
#include <fstream>
#include <locale>
#include <algorithm>
#include <cctype>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <algorithm>
#define BOOST_UTF8_BEGIN_NAMESPACE namespace mynamespace {
#define BOOST_UTF8_END_NAMESPACE }
#define BOOST_UTF8_DECL

#include <boost/detail/utf8_codecvt_facet.hpp>

#include "icu_converter.hpp"
#include <unicode/schriter.h>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

RuleCompiler::RuleCompiler()
{
//    tagset = new Tagset;
//    rules = new Rules;
//    rules->clear();
    rules = RulesPtr( new Rules());

    verbose = false;

    regAmp = "&";
    regLt = "<";
    regGt = ">";
    regLPar = "\\\\\\(";
    regRPar = "\\\\\\)";
    regAlt = "\\\\\\|";
    regPlus = "\\\\\\+";
    regAsterisk = "\\\\\\*";
    regOpt = "\\\\\\?";

    syntok = false; //czy true?
}

RuleCompiler::RuleCompiler(std::string &filename)
{
    verbose = false;
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
    regNothingSet = boost::make_u32regex(utf8converter::ToUtf8(utf8converter::FromUtf8(tmpNothing)));
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

    syntok = false; //czy true?
}

RuleCompiler::~RuleCompiler()
{
//    delete tagset;
    //for (Rules::iterator r = rules->begin(); r!=rules->end(); r++)
    //    delete *r;
//    rules->clear();
    //delete rules;
}

void RuleCompiler::setTagset(TagsetPtr aTagset)
{
    //delete tagset;
    //tagset = new Tagset(aTagset);
    tagset = aTagset;
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
    regNothingSet = boost::make_u32regex(utf8converter::ToUtf8(utf8converter::FromUtf8(tmpNothing)));
    //64: regNothingSet = boost::make_u32regex(tmpNothing);
}

RulesPtr RuleCompiler::readFromFile(std::string &filename)
{
    boost::u32regex regComment = boost::make_u32regex("#.*");
    boost::u32regex regWhite = boost::make_u32regex("\\s+");

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

    std::vector<int> matchedIndexes;

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

        line = boost::u32regex_replace(line, regComment, "", boost::match_default | boost::format_sed);
        if (line == "")
            continue;
        if (boost::u32regex_match(line, regWhite))
            continue;

        line = boost::algorithm::trim_copy(line);
        if (line.find("Rule") == 0)
        {
            if (ruleName != "")
            {
                //std::cout << "Akcje: " << chars << std::endl;
                bool repeat = false;
                ActionsPtr actions = this->compileRuleAction(chars, ruleLeftSize, ruleMatchSize, ruleRightSize, ruleName, repeat);
                RulePtr rule = RulePtr( new Rule(ruleName, rulePattern, ruleLeftSize, ruleMatchSize, ruleRightSize, actions, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, repeat, rulePatternLeft, rulePatternMatch, rulePatternRight) );
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
                matchedIndexes.clear();
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
                    rulePattern = this->compileRuleMatch(chars, ruleLeftSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
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
                rulePattern += this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
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
                    rulePattern += this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePatternMatch = this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePattern += rulePatternMatch;
                    //std::cout << "taki wzorze: " << rulePattern << std::endl;
                }
                else
                {
                    //std::cout << "Kontekst prawy: " << chars << std::endl;
                    rulePatternRight = chars;
                    rulePattern += this->compileRuleMatch(chars, ruleRightSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
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
        ActionsPtr actions = this->compileRuleAction(chars, ruleLeftSize, ruleMatchSize, ruleRightSize, ruleName, repeat);
        RulePtr rule = RulePtr( new Rule(ruleName, rulePattern, ruleLeftSize, ruleMatchSize, ruleRightSize, actions, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, repeat, rulePatternLeft, rulePatternMatch, rulePatternRight) );
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
/*
   // Utf8Converter converter;
    std::wstring nonAsciiU = utf8converter::FromUtf8("ĄĆĘŁŃÓŚŹŻ");
    wchar_t nonL[] = {L'ą', L'ć', L'ę', L'ł', L'ń', L'ó', L'ś', L'ź', L'ż'};
    wchar_t nonU[] = {L'Ą', L'Ć', L'Ę', L'Ł', L'Ń', L'Ó', L'Ś', L'Ź', L'Ż'};
    int size = sizeof(nonL)/sizeof(wchar_t);
    //std::wstring nonAsciiL = converter.FromUtf8("ąćęłńóśźż");
    std::wstring nonAsciiL = utf8converter::FromUtf8("ąćęłńóśźż");
//    std::cout << "Aper: " << converter.ToUtf8(nonAsciiU) << std::endl;
//    std::cout << "Louer: " << converter.ToUtf8(nonAsciiL) << std::endl;

    //boost::wregex regNonAsciiU (L"[" + nonAsciiU + L"]");
//    boost::wregex regNonAsciiL (converter.FromUtf8("([^pr]*)([pr]+)([^pr]*)"));
//    boost::wregex regNonAsciiL (L"([^" + nonAsciiL + L"]*)([" + nonAsciiL + L"])(.*)");
    boost::u32regex regNonAscii = boost::make_u32regex ("([^" + utf8converter::ToUtf8(nonAsciiL) + "]*)([" + utf8converter::ToUtf8(nonAsciiL) + "])(.*)");

//    std::cout << "wyrazenie: " << converter.ToUtf8(regNonAsciiL.str()) << std::endl;
    std::ifstream file("simple.sr");
    //file.imbue(utf8_locale);
//    file.imbue(locale("pl_PL"));
//    wcout.imbue(locale("pl_PL"));

    while (!file.eof())
    {
        std::string line;
        std::string::iterator start, end;
        start = line.begin();
        end = line.end();
        getline(file, line);
        std::wstring wline = utf8converter::FromUtf8(line);
        std::string u, l;
        //boost::match_results<std::string::iterator> container;
        boost::match_flag_type flags = boost::match_default;
        //boost::wsmatch container;
        boost::smatch container;
        //std::cout << "Normal: " << converter.ToUtf8(wline) << std::endl;
        //while (boost::regex_search(start, end, container, regNonAsciiL, flags))
        //while (boost::regex_match(wline, container, regNonAsciiL))
        while (boost::u32regex_match(line, container, regNonAscii))
        {
            //std::cout << "jedynka: " << converter.ToUtf8(container[1]) << std::endl;
            //std::cout << "jedynka: " << container[1] << std::endl;
            //std::string character = converter.ToUtf8(container[2]);
            std::wstring character = utf8converter::FromUtf8(container[2]);
            std::string u8char = container[2];
            std::string u8nal = utf8converter::ToUtf8(nonAsciiL);
            std::string u8nau = utf8converter::ToUtf8(nonAsciiU);
            std::cout << u8nau << std::endl;
            int u8pos = u8nal.find(u8char);
            //std::cout << "u8pos:" << u8pos << std::endl;
            std::wcout << L"Znalezisko: " << nonAsciiU.substr(u8pos/2, 1) << std::endl;
//            std::cout << "Szukam: " << converter.ToUtf8(character) << std::endl;
//            std::cout << "W: " << converter.ToUtf8(nonAsciiL) << std::endl;
//            //std::cout << "Dwojka: " << character << std::endl;
//            int pos = 0;
//            std::string charact = converter.ToUtf8(character);
//            while (pos < size)
//            {
//                std::wstring ws (L"" + nonL[pos]);
//                if (ws == character)
//                {
//                    std::cout << "Znalazlem: " << charact << " na: " << pos << std::endl;
//                    break;
//                }
//                pos ++;
//            }
//            if (pos == size)
//                std::cout << "Nie znalazlem gnoja!" << std::endl;
//            //int pos = converter.ToUtf8(nonAsciiL).substr(0, -1).find(converter.ToUtf8(character).substr(0, 1));
//            std::cout << pos << std::endl;
//            u += container[1]; //line.substr(0, pos);
//            if (pos >= nonAsciiU.length())
//                std::cout << "nie ma tej lajzy: " << converter.ToUtf8(character) << std::endl;
//            else
//            {
//                std::wstring mapped = L"" + nonAsciiU.substr(pos, 1); // nonAsciiU.at(pos);
                std::wstring mapped = L"" + nonU[u8pos/2];
                std::wcout << "mapet: " << mapped << std::endl;
            u += utf8converter::ToUtf8(mapped);
//            }
            //std::cout << "u: " << u << std::endl;
            line = container[3]; //line.substr(pos + 1, std::string::npos);
            //std::cout << line << std::endl;
            //start = container.prefix().second;
            //flags |= boost::match_prev_avail;
            //flags |= boost::match_not_bob;
        }
        if (line.length() > 0)
            u += line;
        std::cout << u << std::endl;

    }

//    typedef wchar_t ucs4_t;
//
//    std::locale old_locale;
//    std::cerr << old_locale.name() << std::endl;
//    std::locale utf8_locale(old_locale, new boost::program_options::detail::utf8_codecvt_facet(0));
//    std::cerr << utf8_locale.name() << std::endl;
//    //std::locale::global(utf8_locale);
//
//    std::ifstream wideRuleFile("simple.sr");
//    wideRuleFile.imbue(utf8_locale);
//
//    while (!wideRuleFile.eof())
//    {
//        std::string ws;
//        getline(wideRuleFile, ws);
//        std::cout << "Normal: " << ws << std::endl;
//        std::string u, l;
//        u = ws;
//        std::string xx = ws;
////        boost::to_upper(u);
//     //   std::transform(ws.begin(), ws.end(), u.begin(), std::towupper);
//        std::string xxx = boost::algorithm::to_lower_copy(xx);
//     //   std::cout << "Upper:  " << u << std::endl;
//        std::cout << "Bustem: " << xxx << std::endl;
//       // std::wcout << L"Upper:  " << boost::to_upper(ws) << std::endl;
//       // std::wcout << L"Lower:  " << boost::to_lower(ws) << std::endl;
//    }
*/
    return rules;
}

std::string RuleCompiler::compileNonTokens(std::string &matched)
{
    boost::u32regex regWhite = boost::make_u32regex("\\s+");
    boost::u32regex regSpecialNeg = boost::make_u32regex("!(sb|se|ns)");
    boost::u32regex regSpecial = boost::make_u32regex("(^|[ \\(\\)\\|])(sb|se|ns)");
    boost::u32regex regLPar = boost::make_u32regex("\\(");
    boost::u32regex regRPar = boost::make_u32regex("\\)([+*?])?");

    matched = boost::u32regex_replace(matched, regLPar, "((", boost::match_default | boost::format_sed);
    matched = boost::u32regex_replace(matched, regRPar, ")\\1)", boost::match_default | boost::format_sed);
    //matched = boost::u32regex_replace(matched, regSpecialNeg, "(<<s<[^<>]+<(?!\\1)[^<>]+<>)", boost::match_default | boost::format_sed);
    matched = boost::u32regex_replace(matched, regSpecialNeg, "((<<t<[^<>]+<[^<>]+<[^>]+>)|(<<g<[^<>+]<[^<>]+<[^<>]+<[^>]+))", boost::match_default | boost::format_sed);
    matched = boost::u32regex_replace(matched, regSpecial, "\\1(<<s<[^<>]+<\\2<>)", boost::match_default | boost::format_sed);
    matched = boost::u32regex_replace(matched, regWhite, "", boost::match_default | boost::format_sed);

    return matched;
}

std::string RuleCompiler::compileRuleName(std::string matched)
{
    boost::u32regex regRuleName = boost::make_u32regex("Rule\\s*\"\\s*(.*)\\s*\"\\s*;?\\s*");
    boost::smatch container;
    if (boost::u32regex_match(matched, container, regRuleName))
    {
        std::string s = container[1];
//        std::cout << "Kompilejszyn reguly: " << container[1] << std::endl;
        return s;
    }
    else
    {
        std::cerr << "Wrong rule declaration: " << matched << "." << std::endl;
        return "";
    }

}

std::string RuleCompiler::compileRuleMatch(std::string &matched, int &size, std::vector<std::string> &tokensPatterns, std::vector<std::string> &tokensModifiers, std::vector<bool> &tokensRequired, std::vector<int> &matchedIndexes, int &bracketCount)
{
    boost::u32regex regMatch = boost::make_u32regex("^(Match|Left|Right)\\s*:\\s*");
    boost::u32regex regWhite = boost::make_u32regex("\\s+");

    matched = boost::u32regex_replace(matched, regMatch, "", boost::match_default | boost::format_sed);

    if ((matched == "") || (boost::u32regex_match(matched, regWhite)))
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

        std::string compiledToken = compileToken(token);
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
                matchedIndexes.push_back(mindex);
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

std::string RuleCompiler::getToken(std::string &matched, std::string &before)
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


//std::string RuleCompiler::getToken(std::string &matched, std::string &before)
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

int RuleCompiler::countTokens(std::string &matched)
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

std::string RuleCompiler::compileToken(std::string &token)
{

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
    Interpretations interps;
    interps.push_back(baseMask);
    bool groupMatch = false;
    std::string compiledHead = "";
    std::string type = "";

    std::string result = "";
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
                compiledHead = compileToken(head);
                compiledHead = compiledHead.substr(4, compiledHead.length() - 5);
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
            bool ret = compilePosCondition(compOperator, value, interps);
            if (!ret)
            {
                std::cerr << "EROR!" << std::endl;
            }
        }
        else if (key == "base")
        {
            bool ret = compileBaseCondition(compOperator, value, interps, icase);
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
            bool ret = compileAttributeCondition(key, compOperator, value, interps);
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

    if (tokenMatch)
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
    }

    return "(" + result + ")";
}

std::string RuleCompiler::getKey(std::string &token)
{
    int pos = token.find_first_of("!~=");
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

    return key;
}

std::string RuleCompiler::getValue(std::string &token)
{
    int begin = token.find_first_not_of(" ");
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
        if (token[end] == '"')
            if ((end == 0) || (token[end - 1] != '\\'))
            {
                if (!quotes)
                    quotes ++;
                else
                    quotes --;
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
    std::string value = token.substr(begin + quotes,  (end - begin - 2*quotes));
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

    return value;
}

std::string RuleCompiler::getOperator(std::string &token)
{
    int pos = token.find_first_not_of("!~=");
    if (pos == std::string::npos)
    {
        std::cerr << "Illegal token specification: " << token << std::endl;
        return "";
    }
    std::string op = token.substr(0, pos);
    token = boost::algorithm::trim_copy(token.substr(pos, std::string::npos));
    return op;
}

bool RuleCompiler::compilePosCondition(std::string &comparisonOperator, std::string &value, Interpretations &interps)
{
    std::string mapped;
    if (value.find_first_of(".|*+?[]") != std::string::npos)
    {
        std::vector<char> values = tagset->mapPosMatching(value);
        if (values.size() == 0)
        {
            std::cerr << "No parts of speech identifiers matching regular expression: \"" << value << "\"." << std::endl;
            return false;
        }
        mapped = "";
        std::vector<char>::iterator i = values.begin();
        while (i != values.end())
        {
            mapped += *i;
            i ++;
        }
        if (comparisonOperator != "!~")
            mapped = "[" + mapped + "]";
    }
    else
    {
        if (!tagset->checkPos(value))
        {
            std::cerr << "Unknown part of speech: " << value << "." << std::endl;
            return false;
        }
        mapped = tagset->mapPos(value);
    }
    if (comparisonOperator == "~~")
    {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end())
        {
            (*i)[0] = mapped;
            i ++;
        }
    }
    else if (comparisonOperator == "~")
    {
        if (interps.size() == 1)
        {
            TokenMask currentInterpretation = interps.front();
            currentInterpretation[modifierIndex] = "*";
            (interps.front())[0] = mapped;
            (interps.front())[modifierIndex] = "()";
            interps.push_back(currentInterpretation);
            interps.insert(interps.begin(), currentInterpretation);
        }
        else
        {
            Interpretations::iterator i = interps.begin() + 1;
            (*i)[0] = mapped;
        }
    }
    else if (comparisonOperator == "!~")
    {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end())
        {
            (*i)[0] = "[^" + mapped;
            (*i)[0] += "]";
            i ++;
        }
    }
    else
    {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return false;
    }

    return true;
}

bool RuleCompiler::compileBaseCondition(std::string &comparisonOperator, std::string &value, Interpretations &interps, bool icase)
{
    value = boost::regex_replace(value, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regLt, "\\&lt;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regGt, "\\&gt;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regAlt, "\\&bar;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);

    if (icase)
    {
        UnicodeString valU = icuConverter::fromUtf(value);
        UnicodeString valL = icuConverter::fromUtf(value);
        valU.toUpper();
        valL.toLower();
        StringCharacterIterator itU(valU);
        StringCharacterIterator itL(valL);
        std::stringstream ss;
        while (itU.hasNext())
        {
            UnicodeString tu = itU.current();
            UnicodeString tl = itL.current();
            if (tu != tl)
                ss << "[" << icuConverter::toUtf(tu) << icuConverter::toUtf(tl) << "]";
            else
                ss << icuConverter::toUtf(tu);
            itU.next();
            itL.next();
        }
        value = ss.str();
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

    boost::u32regex regLpar = boost::make_u32regex("\\\\\\\\\\\\\\(");
    boost::u32regex regRpar = boost::make_u32regex("\\\\\\\\\\\\\\)");
    value = boost::u32regex_replace(value, regLpar, "\\&lpar;", boost::match_default | boost::format_sed);
    value = boost::u32regex_replace(value, regRpar, "\\&rpar;", boost::match_default | boost::format_sed);

    if (value.find("|") != std::string::npos)
    {
        value = "(" + value + ")";
    }

    if (comparisonOperator == "~~")
    {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end())
        {
            (*i)[baseIndex] = value;
            i ++;
        }
    }
    else if (comparisonOperator == "~")
    {
        if (interps.size() == 1)
        {
            TokenMask currentInterpretation = interps.front();
            currentInterpretation[modifierIndex] = "*";
            (interps.front())[baseIndex] = value;
            (interps.front())[modifierIndex] = "()";
            interps.push_back(currentInterpretation);
            interps.insert(interps.begin(), currentInterpretation);
        }
        else
        {
            Interpretations::iterator i = interps.begin() + 1;
            (*i)[baseIndex] = value;
        }
    }
    else if (comparisonOperator == "!~")
    {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end())
        {
            (*i)[baseIndex] = "(?!" + value + ")[^<>]+";
            i ++;
        }
    }
    else
    {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return false;
    }

    return true;
}

bool RuleCompiler::compileAttributeCondition(std::string &key, std::string &comparisonOperator, std::string &value, Interpretations &interps)
{
    std::string mapped;
    int attributeIndex;
    if (value.find_first_of(".|*+?") != std::string::npos)
    {
        attributeIndex = tagset->getAttributeIndex(key) + 1;
        if (attributeIndex == -1)
        {
            std::cerr << "No such attribute: " << key << "." << std::endl;
            return false;
        }
        std::vector<char> values = tagset->mapAttributeValuesMatching(key, value);
        if (values.size() == 0)
        {
            std::cerr << "No values of " << key << " matching regular expression: \"" << value << "\"." << std::endl;
            return false;
        }
        std::vector<char>::iterator i = values.begin();
        mapped = "";
        while (i != values.end())
        {
            mapped += *i;
            i ++;
        }
        if (comparisonOperator != "!~")
            if (values.size() > 1)
                mapped = "[" + mapped + "]";
    }
    else
    {
        if (tagset->checkAttribute(key))
        {
            if (tagset->checkAttributeValue(key, value))
            {
                std::pair<char, int> pair = tagset->mapAttributeValue(key, value);
                mapped = pair.first;
                attributeIndex = pair.second + 1; // bo pierwsza w 'tablicy' interpretacji jest czesc mowy!
            }
            else
            {
                std::cerr << "Unknown value: " << value << " of the attribute: " << key << "." << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "No such attribute: " << key << "." << std::endl;
            return false;
        }
    }
    if (comparisonOperator == "~~")
    {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end())
        {
            (*i)[attributeIndex] = mapped;
            i ++;
        }
    }
    else if (comparisonOperator == "~")
    {
        if (interps.size() == 1)
        {
            TokenMask currentInterpretation = interps.front();
            currentInterpretation[modifierIndex] = "*";
            (interps.front())[attributeIndex] = mapped;
            (interps.front())[modifierIndex] = "()";
            interps.push_back(currentInterpretation);
            interps.insert(interps.begin(), currentInterpretation);
        }
        else
        {
            Interpretations::iterator i = interps.begin() + 1;
            (*i)[attributeIndex] = mapped;
        }
    }
    else if (comparisonOperator == "!~")
    {
        Interpretations::iterator i = interps.begin();
        while (i != interps.end())
        {
            (*i)[attributeIndex] = "[^" + mapped;
            (*i)[attributeIndex] += "]";
            i ++;
        }
    }
    else
    {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return false;
    }

    return true;
}

std::string RuleCompiler::compileOrthCondition(std::string &comparisonOperator, std::string &value, bool icase)
{
    value = boost::regex_replace(value, regAmp, "\\&amp;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regLt, "\\&lt;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regGt, "\\&gt;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regLPar, "\\&lpar;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regRPar, "\\&rpar;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regAlt, "\\&bar;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regPlus, "\\&plus;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regAsterisk, "\\&astr;", boost::match_default | boost::format_sed);
    value = boost::regex_replace(value, regOpt, "\\&qmark;", boost::match_default | boost::format_sed);

    if (icase)
    {
        UnicodeString valU = icuConverter::fromUtf(value);
        UnicodeString valL = icuConverter::fromUtf(value);
        valU.toUpper();
        valL.toLower();
        StringCharacterIterator itU(valU);
        StringCharacterIterator itL(valL);
        std::stringstream ss;
        while (itU.hasNext())
        {
            UnicodeString tu = itU.current();
            UnicodeString tl = itL.current();
            if (tu != tl)
                ss << "[" << icuConverter::toUtf(tu) << icuConverter::toUtf(tl) << "]";
            else
                ss << icuConverter::toUtf(tu);
            itU.next();
            itL.next();
        }
        value = ss.str(); /*
        std::string valueU = value;
        std::string valueL = value;
        std::transform(value.begin(), value.end(), valueU.begin(), ::toupper);
        std::transform(value.begin(), value.end(), valueL.begin(), ::tolower);
//        std::cout << "UC: " << valueU << std::endl;
//        std::cout << "LC: " << valueL << std::endl;
        std::wstring wideU = utf8converter::FromUtf8(valueU);
        std::wstring wideL = utf8converter::FromUtf8(valueL);
        int j = 0;
        while (wideU.at(j))
        {
            std::wstring tu(1, wideU.at(j));
            std::wstring tl(1, wideL.at(j));
            ss << "[" << utf8converter::ToUtf8(tu) << utf8converter::ToUtf8(tl) << "]";
            j ++;
        }
        //value = "";
//        for (int i = 0; i < valueU.length(); i++)
//            ss << "[" << valueU[i] << valueL[i] << "]";
        value = ss.str();*/
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

    boost::u32regex regLpar = boost::make_u32regex("\\\\\\\\\\\\\\(");
    boost::u32regex regRpar = boost::make_u32regex("\\\\\\\\\\\\\\)");
    value = boost::u32regex_replace(value, regLpar, "\\&lpar;", boost::match_default | boost::format_sed);
    value = boost::u32regex_replace(value, regRpar, "\\&rpar;", boost::match_default | boost::format_sed);

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

std::string RuleCompiler::interpretationsToString(Interpretations interps)
{
    std::string result = "";
    Interpretations::iterator i = interps.begin();
    while (i != interps.end())
    {
        std::string interpretation = "";
        int j = 0;
        while (j < modifierIndex)
        {
            interpretation += (*i)[j];
            j ++;
        }
        interpretation = "<" + interpretation;
        if ((*i)[modifierIndex] != "")
        {
            if ((*i)[modifierIndex] == "()")
            {
                interpretation = "(" + interpretation + ")";
            }
            else if ((*i)[modifierIndex] == "*")
            {
                interpretation = "(" + interpretation + ")*";
            }
            else if ((*i)[modifierIndex] == "+")
            {
                interpretation = "(" + interpretation + ")+";
            }
            else if ((*i)[modifierIndex] == "?")
            {
                interpretation = "(" + interpretation + ")?";
            }
        }
        result += interpretation;
        i ++;
    }
    if (interps.size() == 1)
        result = "(" + result + ")+";

    if (result.find(nothingSet) != std::string::npos)
        result = boost::u32regex_replace(utf8converter::ToUtf8(utf8converter::FromUtf8(result)), regNothingSet, "[^<>]+", boost::match_default | boost::format_sed);
        //64: result = boost::u32regex_replace(result, regNothingSet, "[^<>]+", boost::match_default | boost::format_sed);

    return result;
}

ActionsPtr RuleCompiler::compileRuleAction(std::string &matched, int ruleLeftSize, int ruleMatchSize, int ruleRightSize, std::string ruleName, bool &repeat)
{
    boost::u32regex regEval = boost::make_u32regex("^Eval\\s*:\\s*");
    boost::u32regex regGroup = boost::make_u32regex("group\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // group(XX, 1);
    //boost::regex regDelete("delete\\s*\\(\\s*([^\\s!~\"]+)\\s*([!~]?~)\\s*\"([^\\s\",]+)\"\\s*,\\s*(\\d+)\\s*\\)\\s*");    //delete(pos!~"X", 1)
    boost::u32regex regDelete = boost::make_u32regex("delete\\s*\\(\\s*([^,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    boost::u32regex regAdd = boost::make_u32regex("add\\s*\\(\\s*\"([^\\s\",]+)\"\\s*,\\s*((\"[^\\s\",]+\")|(base))\\s*,\\s*(\\d+)\\s*\\)\\s*");                                       //add("X:m:sg", 1);
    boost::u32regex regUnify = boost::make_u32regex("unify\\s*\\(\\s*([^,]+)\\s*((,\\s*(\\d+)\\s*)+)\\)\\s*");                                           //unify(gender number case, 1, 2, 3);
//    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*([^,]+)\\s*,\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
    boost::u32regex regJoin = boost::make_u32regex("join\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // join(XX, 1);
    boost::u32regex regAttach = boost::make_u32regex("attach\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // attach(XX, 1);
    boost::u32regex regTransform = boost::make_u32regex("transform\\s*\\(\\s*(\\d+)\\s*,\\s*([^\\s,]+)\\s*\\)\\s*");                                         // transform(1, YY);
    boost::u32regex regRepeat = boost::make_u32regex("repeat");                                         // repeat;
    boost::u32regex regWhite = boost::make_u32regex("\\s+");
    boost::u32regex regActionSeparator = boost::make_u32regex("\\s*;\\s*");

    //std::cout << "Moje wyrazenia: " << std::endl << regGroup.str() << std::endl << regDelete.str() << std::endl << regAdd.str() << std::endl << regUnify.str() << std::endl;

    matched = boost::u32regex_replace(matched, regEval, "", boost::match_default | boost::format_sed);

    ActionsPtr actions = ActionsPtr( new Actions);

    if ((matched == "") || (boost::u32regex_match(matched, regWhite)))
    {
        std::cerr << "No actions defined in Eval section." << std::endl;
        return actions;//(Actions)NULL;
    }

//    repeat = false;

    boost::u32regex_token_iterator<std::string::iterator> i(matched.begin(), matched.end(), regActionSeparator, -1);
    boost::u32regex_token_iterator<std::string::iterator> j;
    while (i != j)
    {
        std::string actionString = *i;
        boost::smatch container;
        if (boost::u32regex_match(actionString, container, regRepeat))
        {
            repeat = true;
        }
        else if (boost::u32regex_match(actionString, container, regGroup))
        {
            GroupActionPtr action = GroupActionPtr( new GroupAction(container[1], ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1), boost::lexical_cast<int>(container[2]), ruleName) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
        }
        else if (boost::u32regex_match(actionString, container, regJoin))
        {
            JoinActionPtr action = JoinActionPtr( new JoinAction(container[1], ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1), boost::lexical_cast<int>(container[2]), ruleName) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
        }
        else if (boost::u32regex_match(actionString, container, regAttach))
        {
            AttachActionPtr action = AttachActionPtr( new AttachAction(container[1], ruleLeftSize, (ruleLeftSize + ruleMatchSize - 1), boost::lexical_cast<int>(container[2]), ruleName) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
        }
        else if (boost::u32regex_match(actionString, container, regTransform))
        {
            TransformActionPtr action = TransformActionPtr( new TransformAction(container[2], boost::lexical_cast<int>(container[1]), ruleName) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
        }
        else if (boost::u32regex_match(actionString, container, regDelete))
        {
            int tokenIndex = boost::lexical_cast<int>(container[2]);
            std::string conditions = container[1];
            std::string uncompiled = conditions;
            TokenMask mask = baseMask;
            std::string orth = "[^<>]+";

            while (conditions != "")
            {
                std::string key = getKey(conditions);
                std::string compOperator = getOperator(conditions);
                std::string value = getValue(conditions);
                compileDeleteCondition(key, compOperator, value, mask, orth);
                if (conditions.find("&&") == 0)
                {
                    conditions = conditions.substr(2, std::string::npos);
                }
            }

            std::string condition = "";
            TokenMask::iterator i = mask.begin();
            while (i != mask.end())
            {
                condition += *i;
                i ++;
            }
            condition = orth + "<" + condition;

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
            DeleteActionPtr action = DeleteActionPtr( new DeleteAction(condition, tokenIndex, uncompiled) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);
 //           std::cout << "Usun interpretacje: " << container[4] << " pod warunkiem, ze: " << container[1] << " " << container[2] << " " << container[3] << std::endl;
        }
        else if (boost::u32regex_match(actionString, container, regAdd))
        {
            int tokenIndex = boost::lexical_cast<int>(container[5]);
            std::string base = container[2];
            if (base == "base")
            {
                base = "[^<>]+";
            }
            else
            {
                base = base.substr(1, base.length() - 2); //usuniecie cudzyslowow wokol formy bazowej
            }

            std::vector<InterpretationPair> interpretations; // = compileAddInterpretations(container[1]);
            std::string interpretation = container[1];
            compileAddInterpretation(interpretation, interpretations);

            AddActionPtr action = AddActionPtr( new AddAction(interpretations, base, tokenIndex, interpretation) );
            if (verbose)
                action->setVerbose();
            actions->push_back(action);

//            std::cout << "Dodaj interpretacje: " << container[1] << " do: " << container[2] << std::endl;
        }
        else if (boost::u32regex_match(actionString, container, regUnify))
        {
            std::vector<int> attributes;
            std::vector<int> tokens;
            std::vector<std::string> attributes_;
            std::string attrs = container[1];
            std::string toks = container[2];
            std::string mask;
            mask.assign(tagset->getNumberOfAttributes() + 1, '.');
            boost::u32regex_token_iterator<std::string::iterator> a(attrs.begin(), attrs.end(), regWhite, -1);
            boost::u32regex_token_iterator<std::string::iterator> b;
            while (a != b)
            {
                std::string aa = *a; //TODO: jakies rzutowanie tego nie zalatwi?
                if (tagset->checkAttribute(aa))
                {
                    attributes.push_back(tagset->getAttributeIndex(aa));
                    attributes_.push_back(aa);
                }
                else
                {
                    std::cout << "Unknown attribute: " << *a << "." << std::endl;
                }
                a ++;
            }
            boost::u32regex regTokenSeparator = boost::make_u32regex("\\s*,\\s*");
            boost::u32regex_token_iterator<std::string::iterator> t(toks.begin(), toks.end(), regTokenSeparator, -1);
            boost::u32regex_token_iterator<std::string::iterator> u;
            while (t != u)
            {
                if (*t != "")
                {
                    tokens.push_back(boost::lexical_cast<int>(*t));
                }
                t ++;
            }

            UnifyActionPtr action = UnifyActionPtr( new UnifyAction(attributes, tokens, attributes_) );
            if (verbose)
                action->setVerbose();

            action->rule = ruleName;
            action->setBaseMask(mask);
            actions->push_back(action);
        }
        else if (boost::u32regex_match(actionString, container, regWord))
        {
            std::vector<int> tokens;
//            std::string toks = container[1];
            std::string mask = container[1];

            for (int x = ruleLeftSize; x < (ruleLeftSize + ruleMatchSize); x ++)
            {
                tokens.push_back( x + 1);
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
            std::vector<InterpretationPair> interpretations; // = compileAddInterpretations(container[1]);
            compileAddInterpretation(mask, interpretations);

            SyntokActionPtr action = SyntokActionPtr( new SyntokAction(ruleLeftSize, ruleLeftSize + ruleMatchSize - 1, tokens, interpretations, ruleName, mask) );
            if (verbose)
                action->setVerbose();
            if (syntok)
                action->setSyntok();
             //   ((SyntokAction*)action)->setSyntok();

            actions->push_back(action);

        }
        else
        {
            std::cerr << "Unknown action: " << actionString << "." << std::endl;
        }
        i ++;
    }

    return actions;
}

bool RuleCompiler::compileDeleteCondition(std::string &key, std::string &comparisonOperator, std::string &value, TokenMask &mask, std::string &orth)
{
    std::string mapped;
    int attributeIndex;
    if (key == "pos")
    {
        if (value.find_first_of(".|*+?") != std::string::npos)
        {
            std::vector<char> values = tagset->mapPosMatching(value);
            if (values.size() == 0)
            {
                std::cerr << "No parts of speech identifiers matching regular expression: \"" << value << "\"." << std::endl;
                return false;
            }
            mapped = "";
            std::vector<char>::iterator i = values.begin();
            while (i != values.end())
            {
                mapped += *i;
                i ++;
            }
            if (comparisonOperator != "!~")
                mapped = "[" + mapped + "]";
        }
        else
        {
            if (!tagset->checkPos(value))
            {
                std::cerr << "Unknown part of speech: " << value << "." << std::endl;
                return false;
            }
            mapped = tagset->mapPos(value);
        }
        attributeIndex = 0;
    }
    else if (key == "base")
    {
        if ((comparisonOperator == "~~") || (comparisonOperator == "~"))
        {
            mapped = value;
        }
        else if (comparisonOperator == "!~")
        {
            mapped = "(?!" + value + ")[^<>]+";
            mask[baseIndex] = mapped;
            return true;
        }
        else
        {
            std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
            return false;
        }
        attributeIndex = baseIndex;
    }
    else if (key == "orth")
    {
        if ((comparisonOperator == "~~") || (comparisonOperator == "~"))
        {
            orth = value;
        }
        else if (comparisonOperator == "!~")
        {
            orth = "(?!" + value + ")[^<>]+";
        }
        else
        {
            std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
            return false;
        }
        return true;
    }
    else
    {
        if (value.find_first_of(".|*+?") != std::string::npos)
        {
            attributeIndex = tagset->getAttributeIndex(key) + 1;
            if (attributeIndex == -1)
            {
                std::cerr << "No such attribute: " << key << "." << std::endl;
                return false;
            }
            std::vector<char> values = tagset->mapAttributeValuesMatching(key, value);
            if (values.size() == 0)
            {
                std::cerr << "No values of " << key << " matching regular expression: \"" << value << "\"." << std::endl;
                return false;
            }
            std::vector<char>::iterator i = values.begin();
            mapped = "";
            while (i != values.end())
            {
                mapped += *i;
                i ++;
            }
            if (comparisonOperator != "!~")
                if (values.size() > 1)
                    mapped = "[" + mapped + "]";
        }
        else
        {
            if (tagset->checkAttribute(key))
            {
                if (tagset->checkAttributeValue(key, value))
                {
                    std::pair<char, int> pair = tagset->mapAttributeValue(key, value);
                    mapped = pair.first;
                    attributeIndex = pair.second + 1; // bo pierwsza w 'tablicy' interpretacji jest czesc mowy!
                }
                else
                {
                    std::cerr << "Unknown value: " << value << " of the attribute: " << key << "." << std::endl;
                    return false;
                }
            }
            else
            {
                std::cerr << "No such attribute: " << key << "." << std::endl;
                return false;
            }
        }
    }
    if ((comparisonOperator == "~~") || (comparisonOperator == "~"))
    {
        mask[attributeIndex] = mapped;
    }
    else if (comparisonOperator == "!~")
    {
        if ((attributeIndex != 0) && (attributeIndex != baseIndex))
            mask[attributeIndex] = "[^0" + mapped;
        else
            mask[attributeIndex] = "[^" + mapped;
        mask[attributeIndex] += "]";
    }
    else
    {
        std::cerr << "Unknown comparison operator: " << comparisonOperator << "." << std::endl;
        return false;
    }
    return true;
}

bool RuleCompiler::compileAddInterpretation(std::string &pattern, std::vector<InterpretationPair> &interpretations)
{
    boost::u32regex regFieldSeparator = boost::make_u32regex(":");
    boost::u32regex_token_iterator<std::string::iterator> i(pattern.begin(), pattern.end(), regFieldSeparator, -1);
    boost::u32regex_token_iterator<std::string::iterator> j;
    int index = 0;
    std::string pos;
    std::vector<std::string> morphologies;
    while (i != j)
    {
        std::string value = *i;
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

        i ++;
        index ++;
    }

    std::vector<std::string>::iterator m = morphologies.begin();
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
    }

    return true;
}

void RuleCompiler::setVerbose()
{
    verbose = true;
}

void RuleCompiler::setSyntok()
{
    syntok = true;
}


RulePtr RuleCompiler::compileRule(std::string ruleString)
{
    boost::u32regex regComment = boost::make_u32regex("#.*");
    boost::u32regex regWhite = boost::make_u32regex("\\s+");
    boost::u32regex regLineSep = boost::make_u32regex("\\n");
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

    std::vector<int> matchedIndexes;

    std::string chars = "";

    rulePattern = "";
    rulePatternLeft = "";
    rulePatternMatch = "";
    rulePatternRight = "";

    boost::u32regex_token_iterator<std::string::iterator> i(ruleString.begin(), ruleString.end(), regLineSep, -1);
    boost::u32regex_token_iterator<std::string::iterator> j;
    while (i != j)
    {
        std::string line = *i;

        line = boost::u32regex_replace(line, regComment, "", boost::match_default | boost::format_sed);
        if (line == "")
            continue;
        if (boost::u32regex_match(line, regWhite))
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
                    rulePattern = this->compileRuleMatch(chars, ruleLeftSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
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
                rulePattern += this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
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
                    rulePattern += this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePatternMatch = this->compileRuleMatch(chars, ruleMatchSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
                    //rulePattern += rulePatternMatch;
                    //std::cout << "taki wzorze: " << rulePattern << std::endl;
                }
                else
                {
                    //std::cout << "Kontekst prawy: " << chars << std::endl;
                    rulePatternRight = chars;
                    rulePattern += this->compileRuleMatch(chars, ruleRightSize, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, bracketCount);
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
        i ++;
    }
    //std::cout << "Akcje: " << chars << std::endl;
    bool repeat = false;
    ActionsPtr actions = this->compileRuleAction(chars, ruleLeftSize, ruleMatchSize, ruleRightSize, ruleName, repeat);
    //Rule *rule = new Rule(ruleName, rulePattern, ruleLeftSize, ruleMatchSize, ruleRightSize, *actions, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, repeat, rulePatternLeft, rulePatternMatch, rulePatternRight);
    RulePtr rule = RulePtr( new Rule(ruleName, rulePattern, ruleLeftSize, ruleMatchSize, ruleRightSize, actions, tokensPatterns, tokensModifiers, tokensRequired, matchedIndexes, repeat, rulePatternLeft, rulePatternMatch, rulePatternRight) );
//    delete actions;
    //rule.setActions...
    return rule;
}

ActionPtr RuleCompiler::compileAction(std::string actionString, RulePtr rule)
{
    boost::u32regex regEval = boost::make_u32regex("^Eval\\s*:\\s*");
    boost::u32regex regGroup = boost::make_u32regex("group\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // group(XX, 1);
    //boost::regex regDelete("delete\\s*\\(\\s*([^\\s!~\"]+)\\s*([!~]?~)\\s*\"([^\\s\",]+)\"\\s*,\\s*(\\d+)\\s*\\)\\s*");    //delete(pos!~"X", 1)
    boost::u32regex regDelete = boost::make_u32regex("delete\\s*\\(\\s*([^,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");
    boost::u32regex regAdd = boost::make_u32regex("add\\s*\\(\\s*\"([^\\s\",]+)\"\\s*,\\s*((\"[^\\s\",]+\")|(base))\\s*,\\s*(\\d+)\\s*\\)\\s*");                                       //add("X:m:sg", 1);
    boost::u32regex regUnify = boost::make_u32regex("unify\\s*\\(\\s*([^,]+)\\s*((,\\s*(\\d+)\\s*)+)\\)\\s*");                                           //unify(gender number case, 1, 2, 3);
//    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*([^,]+)\\s*,\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
    boost::u32regex regWord = boost::make_u32regex("syntok\\s*\\(\\s*\"([^,\"\\s]+)\"\\s*\\)\\s*");
    boost::u32regex regJoin = boost::make_u32regex("join\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // join(XX, 1);
    boost::u32regex regAttach = boost::make_u32regex("attach\\s*\\(\\s*([^\\s,]+)\\s*,\\s*(\\d+)\\s*\\)\\s*");                                         // attach(XX, 1);
    boost::u32regex regTransform = boost::make_u32regex("transform\\s*\\(\\s*(\\d+)\\s*,\\s*([^\\s,]+)\\s*\\)\\s*");                                         // transform(1, YY);
    boost::u32regex regRepeat = boost::make_u32regex("repeat");                                         // repeat;
    boost::u32regex regWhite = boost::make_u32regex("\\s+");
    boost::u32regex regActionSeparator = boost::make_u32regex("\\s*;\\s*");

    ActionPtr action;

    if (actionString[actionString.length() - 1] == ';')
        actionString = actionString.substr(0, actionString.length() - 1);

        boost::smatch container;
        if (boost::u32regex_match(actionString, container, regRepeat))
        {
//            repeat = true;
        }
        else if (boost::u32regex_match(actionString, container, regGroup))
        {
            action = GroupActionPtr( new GroupAction(container[1], rule->getLeftCount(), (rule->getLeftCount() + rule->getMatchCount() - 1), boost::lexical_cast<int>(container[2]), rule->getName()) );
            if (verbose)
                action->setVerbose();
        }
        else if (boost::u32regex_match(actionString, container, regJoin))
        {
            action = JoinActionPtr( new JoinAction(container[1], rule->getLeftCount(), (rule->getLeftCount() + rule->getMatchCount() - 1), boost::lexical_cast<int>(container[2]), rule->getName()) );
            if (verbose)
                action->setVerbose();
        }
        else if (boost::u32regex_match(actionString, container, regAttach))
        {
            action = AttachActionPtr(new AttachAction(container[1], rule->getLeftCount(), (rule->getLeftCount() + rule->getMatchCount() - 1), boost::lexical_cast<int>(container[2]), rule->getName()));
            if (verbose)
                action->setVerbose();
        }
        else if (boost::u32regex_match(actionString, container, regTransform))
        {
            action = TransformActionPtr( new TransformAction(container[2], boost::lexical_cast<int>(container[1]), rule->getName()) );
            if (verbose)
                action->setVerbose();
        }
        else if (boost::u32regex_match(actionString, container, regDelete))
        {
            int tokenIndex = boost::lexical_cast<int>(container[2]);
            std::string conditions = container[1];
            std::string uncompiled = conditions;
            TokenMask mask = baseMask;
            std::string orth = "[^<>]+";

            while (conditions != "")
            {
                std::string key = getKey(conditions);
                std::string compOperator = getOperator(conditions);
                std::string value = getValue(conditions);
                compileDeleteCondition(key, compOperator, value, mask, orth);
                if (conditions.find("&&") == 0)
                {
                    conditions = conditions.substr(2, std::string::npos);
                }
            }

            std::string condition = "";
            TokenMask::iterator i = mask.begin();
            while (i != mask.end())
            {
                condition += *i;
                i ++;
            }
            condition = orth + "<" + condition;

            action = DeleteActionPtr( new DeleteAction(condition, tokenIndex, uncompiled) );
            if (verbose)
                action->setVerbose();
 //           std::cout << "Usun interpretacje: " << container[4] << " pod warunkiem, ze: " << container[1] << " " << container[2] << " " << container[3] << std::endl;
        }
        else if (boost::u32regex_match(actionString, container, regAdd))
        {
            int tokenIndex = boost::lexical_cast<int>(container[5]);
            std::string base = container[2];
            if (base == "base")
            {
                base = "[^<>]+";
            }
            else
            {
                base = base.substr(1, base.length() - 2); //usuniecie cudzyslowow wokol formy bazowej
            }

            std::vector<InterpretationPair> interpretations; // = compileAddInterpretations(container[1]);
            std::string interpretation = container[1];
            compileAddInterpretation(interpretation, interpretations);

            action = AddActionPtr( new AddAction(interpretations, base, tokenIndex, interpretation) );
            if (verbose)
                action->setVerbose();

//            std::cout << "Dodaj interpretacje: " << container[1] << " do: " << container[2] << std::endl;
        }
        else if (boost::u32regex_match(actionString, container, regUnify))
        {
            std::vector<int> attributes;
            std::vector<int> tokens;
            std::vector<std::string> attributes_;
            std::string attrs = container[1];
            std::string toks = container[2];
            std::string mask;
            mask.assign(tagset->getNumberOfAttributes() + 1, '.');
            boost::u32regex_token_iterator<std::string::iterator> a(attrs.begin(), attrs.end(), regWhite, -1);
            boost::u32regex_token_iterator<std::string::iterator> b;
            while (a != b)
            {
                std::string aa = *a; //TODO: jakies rzutowanie tego nie zalatwi?
                if (tagset->checkAttribute(aa))
                {
                    attributes.push_back(tagset->getAttributeIndex(aa));
                    attributes_.push_back(aa);
                }
                else
                {
                    std::cout << "Unknown attribute: " << *a << "." << std::endl;
                }
                a ++;
            }
            boost::u32regex regTokenSeparator = boost::make_u32regex("\\s*,\\s*");
            boost::u32regex_token_iterator<std::string::iterator> t(toks.begin(), toks.end(), regTokenSeparator, -1);
            boost::u32regex_token_iterator<std::string::iterator> u;
            while (t != u)
            {
                if (*t != "")
                {
                    tokens.push_back(boost::lexical_cast<int>(*t));
                }
                t ++;
            }

            action = UnifyActionPtr( new UnifyAction(attributes, tokens, attributes_) );
            if (verbose)
                action->setVerbose();

            (boost::dynamic_pointer_cast<UnifyAction>(action))->rule = rule->getName();
            (boost::dynamic_pointer_cast<UnifyAction>(action))->setBaseMask(mask);
        }
        else if (boost::u32regex_match(actionString, container, regWord))
        {
            std::vector<int> tokens;
//            std::string toks = container[1];
            std::string mask = container[1];

            for (int x = rule->getLeftCount(); x < (rule->getLeftCount() + rule->getMatchCount()); x ++)
            {
                tokens.push_back( x + 1);
            }
            std::vector<InterpretationPair> interpretations; // = compileAddInterpretations(container[1]);
            compileAddInterpretation(mask, interpretations);

            action = SyntokActionPtr( new SyntokAction(rule->getLeftCount(), rule->getLeftCount() + rule->getMatchCount() - 1, tokens, interpretations, rule->getName(), mask) );
            if (verbose)
                action->setVerbose();
            if (syntok)
                (boost::dynamic_pointer_cast<SyntokAction>(action))->setSyntok();


        }
        else
        {
            std::cerr << "Unknown action: " << actionString << "." << std::endl;
        }
        return action;
}

}

}

}

/*
void RuleCompiler::test()
{
    Entities entities;
    Rules::iterator r = rules->begin();
    int currentEntity = 0;
    while (r != rules->end())
    {
        Rule *rule = *r;
        Actions *actions = rule->getActions();
        Actions::iterator a = actions->begin();
        while (a != actions->end())
        {
            Action *act = *a;
//            act->test(entities, currentEntity);
            a ++;
        }
        r ++;
        currentEntity ++;
    }
}*/

//std::string RuleCompiler::trim(std::string s)
//{
//    return s;
//}


//int main (int argc, char **argv)
//{
//    RuleCompiler rc("simple.sr");
//    rc.test();
//    return 0;
//}
