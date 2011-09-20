
#include "rule.hpp"

#include <iostream>

#include "group_action.hpp"
#include <exception>

//#include <re2/re2.h>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

Rule::Rule()
{
    actions = ActionsPtr( new Actions );
    pattern = PatternPtr( new Pattern("") );
    repeat = false;

    left_ = "";
    match_ = "";
    right_ = "";

    leftCount = 0;
    matchCount = 0;
    rightCount = 0;
}

#if HAVE_RE2
Rule::Rule( std::string aName, std::string aCompiled, int aLeftCount,
            int aMatchCount, int aRightCount, ActionsPtr aActions,
            std::vector<std::string> aTokensPatterns, std::vector<std::string> aTokensModifiers,
            std::vector<bool> aTokensRequired, std::vector<int> aMatchedIndices,
            bool aRepeat, std::string aLeft, std::string aMatch, std::string aRight,
            NegativePatternStrings aNegativePatterns ) {
#else
Rule::Rule( std::string aName, std::string aCompiled, int aLeftCount,
            int aMatchCount, int aRightCount, ActionsPtr aActions,
            std::vector<std::string> aTokensPatterns, std::vector<std::string> aTokensModifiers,
            std::vector<bool> aTokensRequired, std::vector<int> aMatchedIndices,
            bool aRepeat, std::string aLeft, std::string aMatch, std::string aRight) {
#endif

    tokensPatterns.insert(tokensPatterns.begin(), aTokensPatterns.begin(), aTokensPatterns.end());
    tokensModifiers.insert(tokensModifiers.begin(), aTokensModifiers.begin(), aTokensModifiers.end());
    tokensRequired.insert(tokensRequired.begin(), aTokensRequired.begin(), aTokensRequired.end());
//    matchedTokensSize = new std::vector<int>;
    matchedIndices.insert(matchedIndices.begin(), aMatchedIndices.begin(),
            aMatchedIndices.end());// = new std::vector<int>(aMatchedIndexes);

    pattern = PatternPtr( new Pattern("") );
    actions = aActions;

    name = aName;
    setPattern(aCompiled);
    leftCount = aLeftCount;
    matchCount = aMatchCount;
    rightCount = aRightCount;

    repeat = aRepeat;

    left_ = aLeft;
    match_ = aMatch;
    right_ = aRight;

//    matchedTokensSize.assign(tokensPatterns.size(), 0);

#if HAVE_RE2
    for (NegativePatternStrings::iterator negPatIt =
            aNegativePatterns.begin(); negPatIt != aNegativePatterns.end();
            ++ negPatIt) {
        PatternPtr negativePattern = PatternPtr(new Pattern( negPatIt->second ));
        negativePatterns.insert(std::pair<std::string, PatternPtr>(
                    negPatIt->first, negativePattern) );
    }
#endif
}

Rule::~Rule() {
}

bool Rule::apply(std::string &, Lattice &lattice, int currentEntity,
        RuleTokenSizes &ruleTokenSizes) {
    bool ret = false;
    Actions::iterator i = actions->begin();
    while (i != actions->end()) {
        if ((*i)->apply(lattice, currentEntity, ruleTokenSizes)) {
            ret = true;
        }
        ++ i;
    }
    return ret;
}

int Rule::matchPattern(std::string &sentenceString, int matchNumber,
        std::string &beforeMatched, std::vector<StringPiece> &match) {

    int matchCount = 0;

    int num_groups = pattern->NumberOfCapturingGroups();// + 1;
#if HAVE_RE2
    std::map<std::string, int> namedGroups = pattern->NamedCapturingGroups();
#endif
    StringPiece* matchedS = new StringPiece[num_groups];
    Arg** matched = new Arg*[num_groups];
    for (int argIt = 0; argIt < num_groups; argIt ++) {
        matched[argIt] = new Arg;
        *matched[argIt] = &matchedS[argIt];
    }
    StringPiece sentence_str(sentenceString);
    StringPiece orig_str(sentenceString);

    std::string before = "";
    //    std::cerr << "macz namber: " << matchNumber << std::endl;

    //    std::cerr << "ZDANIE: " << sentenceString << std::endl;
    //    std::cerr << "REGULA: " << compiled << std::endl;
    //    std::cerr << "NGRUP:  " << groups_map.size() << std::endl;
    //    for (std::map<std::string, int>::iterator buc = groups_map.begin();
    //            buc != groups_map.end(); buc ++)
    //        std::cerr << "  " << buc->first << " -> " << buc->second << std::endl;
    //        std::cout << "Zdanie: " << sentence << std::endl;
    //    std::cerr << "wzorzec reguly felernyj" << compiled << std::endl;
    try {
        //while ( pattern->Match( sentence_str, start, end,
        //                        RE2::UNANCHORED, matched, num_groups ) ) { // @todo: tu nie bedzie chyba while tylko if, skoro bedzie parametrem odkad szukac, a nie ktore ogolnie dopasowanie ma byc wziete na warsztat
        while ( RegExp::FindAndConsumeN( &sentence_str, *pattern, matched, num_groups ) ) { // @todo: tu nie bedzie chyba while tylko if, skoro bedzie parametrem odkad szukac, a nie ktore ogolnie dopasowanie ma byc wziete na warsztat
            //while (boost::u32regex_search(start, end, matched, *pattern, flags)) {
            //int prefix_len = matchedS[0].begin() - sentence_str.begin();
            //int prefix_len = matchedS[0].begin() - orig_str.begin();
            int prefix_len = matchedS[0].data() - orig_str.data();
            std::string prefix = sentenceString.substr(0, prefix_len); //@todo: czy od start do prefix_len?
            before += prefix;
            //start = matched[0].second;
            //        start = prefix_len + 1;
            std::string matching = "";
            if (matchCount == matchNumber) {
                matching = matchedS[0].as_string();

                for (int i = 0; i < num_groups; i ++) {
                    match.push_back(matchedS[i]);
                }

#if HAVE_RE2
                if (! namedGroups.empty()) {
                    bool negPatternMatched = false;
                    for (std::map<std::string, int>::iterator namedGroupIt =
                            namedGroups.begin(); namedGroupIt != namedGroups.end();
                            ++ namedGroupIt) {
                        std::string groupName = namedGroupIt->first;
                        int groupIndex = namedGroupIt->second;
                        NegativePatterns::iterator negPatternIt =
                            negativePatterns.find(groupName);
                        if (negPatternIt != negativePatterns.end()) {
                            std::string submatch = matchedS[groupIndex].as_string();
                            if (RegExp::FullMatch(submatch, *(negPatternIt->second) )) {
                                negPatternMatched = true;
                                break;
                            }
                        }
                    }
                    if (negPatternMatched) {
                        before += matchedS[0].as_string();
                        continue;
                    }
                }
#endif

                if (matching == "") {
                    delete[] matchedS;
                    for (int argIt = 0; argIt < num_groups; argIt ++)
                        delete matched[argIt];
                    delete[] matched;
                    return -1;
                }
                beforeMatched = before;
                int r = getPatternStart(before); // + 1; //@todo: ten +1 wynika ze sposobu numerowania? w kazdym razie tak jest, zeby bylo w akcjach potem dokadlnie tak, jak wczesniej bylo. moze to sie uda zmienic po wymienieniu calosci
                delete[] matchedS;
                for (int argIt = 0; argIt < num_groups; argIt ++)
                    delete matched[argIt];
                delete[] matched;
                return r;
            }
            else {
                before += matchedS[0].as_string();
            }
            matchCount ++;
        }
        }
        catch (std::exception &e) {
            std::cerr << "Exception in puddle: " << e.what() << std::endl;
            delete[] matchedS;
            for (int argIt = 0; argIt < num_groups; argIt ++)
                delete matched[argIt];
            delete[] matched;
            return -1;
        }
        delete[] matchedS;
        for (int argIt = 0; argIt < num_groups; argIt ++)
            delete matched[argIt];
        delete[] matched;

        return -1;
    }

bool Rule::test(std::string &, Lattice &lattice, int currentEntity,
        std::vector<StringPiece> &match, RuleTokenSizes &ruleTokenSizes) {

    ruleTokenSizes.clear();
    ruleTokenSizes.assign(matchedIndices.size(), 0);

//    std::cerr << "patterns: " << tokensPatterns.size() << std::endl;
//    std::cerr << "modifs: " << tokensModifiers.size() << std::endl;
//    std::cerr << "reqs: " << tokensRequired.size() << std::endl;
//    std::vector<std::string>::iterator xx = tokensPatterns.begin();
//    std::vector<std::string>::iterator yy = tokensModifiers.begin();
//    std::vector<bool>::iterator zz = tokensRequired.begin();
//    while (xx != tokensPatterns.end())
//    {
//        std::cerr << *xx << " " << *yy << " " << *zz << std::endl;
//        xx ++;
//        yy ++;
//        zz ++;
//    }
//    std::cout << "Maczing przed: " << matching << std::endl;
//    std::vector<std::string>::iterator it = tokensPatterns.begin();
    std::vector<int>::iterator it = matchedIndices.begin();
    int i = 0;
    int j = 0;
//    while (it != tokensPatterns.end())
//    {
//        if (tokensRequired[i] == false)
//        {
//            i++;
//            it ++;
//            continue;
//        }
////        std::cout << "Runda pierwsza: " << *it << std::endl;
//
//        boost::u32regex expr = boost::make_u32regex(".*" + *it + ".*");
//        boost::u32regex exprReplace = boost::make_u32regex("" + *it);
//
////        std::cout << "pattern sratern: " << *it << std::endl;
//        std::string mod = tokensModifiers[i];
////        std::cout << "TAKI MOD: " << mod << "." << std::endl;
//        if (mod == "")
//        {
////            std::cout << "pasuje to gowno bez niczego" << std::endl;
//            //matchedTokensSize[i] = 1;
//            boost::smatch what;
//            boost::u32regex_match(matching, what, expr);
//            std::string ma = what[1];
//            matchedTokensSize[i] = countEntities(ma);
//            matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
//        }
//        else if (mod == "+")
//        {
////            std::cout << "pasuje to gowno z plusem" << std::endl;
////            std::cout << "To wycinam: " << *it << " z " << matching << std::endl;
//            if (boost::u32regex_match(matching, expr))
//            {
//                matchedTokensSize[i] ++;
////                std::cout << "Przed: " << matching << std::endl;
//                matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
////                std::cout << "Po: " << matching << std::endl;
//         //       continue;
//            }
//        }
//        i ++;
//        it ++;
//    }
//
////    std::cout << "Maczing po pierwszej rundzie: " << matching << std::endl;
//
//    it = tokensPatterns.begin();
//    i = 0;
//    while (it != tokensPatterns.end())
    while (it != matchedIndices.end())
    {
//        std::cerr << "Numerex: " << *it << std::endl;
//        std::cerr << "a dwojka: " << match[0] << std::endl;
        std::string part = match[*it].as_string();
        if ((part == "") && (tokensRequired[i]))
        {
//            std::cerr << "part pusty" << std::endl;
            return false;
        }
        ruleTokenSizes.at(i) = countEntities(part);
        if (tokensRequired[i] && ruleTokenSizes[i] == 0)
        {
//            std::cerr << "obowiazkowy element " << i << " dlaczemus jest pusty. nie bawie sie tak" << std::endl;
            return false;
        }
        j += ruleTokenSizes.at(i);
//        boost::u32regex expr = boost::make_u32regex("^(" + *it + ").*");
//        boost::u32regex exprReplace = boost::make_u32regex("^" + *it);
//
//    //    std::cout << "pattern sratern: " << *it << std::endl;
//        std::string mod = tokensModifiers[i];
//        if (mod == "")
//        {
//            //matchedTokensSize[i] = 1;
//            boost::smatch what;
//            boost::u32regex_match(matching, what, expr);
//            std::string ma = what[1];
//            matchedTokensSize[i] = countEntities(ma);
//            matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
//        }
//        else if (mod == "?")
//        //if (mod == "?")
//        {
//            if (boost::u32regex_match(matching, expr))
//            {
//                matchedTokensSize[i] = 1;
//                matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
//            }
//            else
//            {
//                matchedTokensSize[i] = 0;
//            }
//        }
//        else if (mod == "+")
//        {
////            std::cout << "To wycinam: " << *it << " z " << matching << std::endl;
//            if (boost::u32regex_match(matching, expr))
//            {
//                matchedTokensSize[i] ++;
////                std::cout << "Przed: " << matching << std::endl;
//                matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
////                std::cout << "Po: " << matching << std::endl;
//                continue;
//            }
//        }
//        else if (mod == "*")
//        {
//            if (boost::u32regex_match(matching, expr))
//            {
//                matchedTokensSize[i] ++;
//                matching = boost::u32regex_replace(matching, exprReplace, "", boost::match_default | boost::format_sed);
//                continue;
//            }
//        }
        i ++;
        ++ it;
    }
    if (j == 0)
    {
        return false;
    }
    //if (matched.prefix() != "")
    //{
//        beforeMatch = matched.prefix();
    //    return false;
    //}

//    std::cout << "Pasuje: " << matched[0] << " do: " << pattern << std::endl;
//    std::cout << "Natomiast liczebnosc dopasowanych elementow prezentuje sie nastepujaco:" << std::endl;
//    std::vector<int>::iterator im = matchedTokensSize->begin();
//    int x = 0;
//    while (im != matchedTokensSize->end())
//    {
////        std::cout << "Element " << x << ": " << *im << " klamotow" << std::endl;
//        x ++;
//        im ++;
//    }

//        std::cout << "pasuje mi: " << sentence << " do: " << pattern << std::endl;

    Actions::iterator ia = actions->begin();
    bool ret = true;
    while (ia != actions->end())
    {
//        Action *act = new Action(*ia);
        //if ((*ia)->test(entities, currentEntity, matchedTokensSize) == false)
        //if ((*ia)->test(pg, currentEntity, matchedTokensSize) == false)
        if ((*ia)->test(lattice, currentEntity, ruleTokenSizes) == false) {
            int limit;
            if (tokensModifiers.at(leftCount + matchCount - 1) == "+" || tokensModifiers.at(leftCount + matchCount - 1) == "")
                limit = 1;
            else
            {
                limit = 0;
            }
            if (ruleTokenSizes.at(leftCount + matchCount - 1) > limit)
            {
                ruleTokenSizes.at(leftCount + matchCount - 1) --;
                continue;
            }
            ret = false;
            break;
        }
//        delete *act;

        ++ ia;
    }

//    if (ret)
//        std::cerr << "wychodze z testu z prawda" << std::endl;
//    else
//        std::cerr << "wychodze z testu z klamstwem" << std::endl;

    return ret;
}

//@todo: a tego nie zamienimy na jakies zliczenie ile jest ciagow <<[tgs] w dopasowaniu?
int Rule::countEntities(std::string matched)
{
    size_t i = 0;
    int p = 0;
    int brackets = 0;

    while (i < matched.length())
    {
        if (matched[i] == '(')
            brackets ++;
        if (matched[i] == ')')
            brackets --;
        if (matched[i] == '<')
            if (i < matched.length())
            {
                if (matched[i + 1] == '<')
                {
                    if (brackets == 0)
                    {
                        p ++;
                        i ++;
                    }
                }
                else if (i > 0)
                {
                    if (matched[i - 1] == '(')
                    {
                        brackets --;
                        i ++;
                    }
                    else if (matched[i - 1] == ')')
                    {
                        brackets ++;
                        i ++;
                    }
                }
            }
        i ++;
    }

    return p;
//    std::string s = matched;
//    while (s.find("<<", p) != std::string::npos)
//    {
//        //if (s[p+2] == 'g')
//        //{
//        //     i ++;
//        //     p = s.find("<<t", p) + 3;
//        //     continue;
//        //}
//        i ++;
//        p = s.find("<<", p) + 2;
//    }
//    return i;
}

std::string Rule::getName() const {
    return name;
}

PatternPtr Rule::getPattern() const {
    return pattern;
}

std::string Rule::getCompiled() const {
    return compiled;
}

int Rule::getLeftCount() const {
    return leftCount;
}

int Rule::getMatchCount() const {
    return matchCount;
}

int Rule::getRightCount() const {
    return rightCount;
}

ActionsPtr Rule::getActions() const {
    return actions;
}

void Rule::setName(std::string aName)
{
    name = aName;
}

void Rule::setPattern(std::string aCompiled)
{
    //delete pattern;
    //pattern = new Pattern;

    //*pattern = boost::make_u32regex(aCompiled.c_str(), boost::regex::perl);
    pattern = PatternPtr(new Pattern(aCompiled));
    //*pattern = boost::make_u32regex(utf8converter::ToUtf8(utf8converter::FromUtf8(aCompiled)));
    //64: *pattern = boost::make_u32regex(aCompiled);
    compiled = aCompiled;
}

void Rule::setLeftCount(int aCount)
{
    leftCount = aCount;
}

void Rule::setMatchCount(int aCount)
{
    matchCount = aCount;
}

void Rule::setRightCount(int aCount)
{
    rightCount = aCount;
}

void Rule::setActions(Actions aActions)
{
    *actions = aActions;
}

void Rule::setRepeat(bool aRepeat)
{
    repeat = aRepeat;
}

bool Rule::getRepeat() const {
    return repeat;
}

void Rule::setMatch(std::string aMatch)
{
    match_ = aMatch;
}

void Rule::setLeft(std::string aLeft)
{
    left_ = aLeft;
}

void Rule::setRight(std::string aRight)
{
    right_ = aRight;
}

void Rule::setTokensPatterns(std::vector<std::string> aTokensPatterns)
{
    tokensPatterns.clear();
    tokensPatterns.insert(tokensPatterns.begin(), aTokensPatterns.begin(), aTokensPatterns.end());
//    delete tokensPatterns;
//    tokensPatterns = new std::vector<std::string>(aTokensPatterns);
    //delete matchedTokensSize;
    //matchedTokensSize = new std::vector<int>;
//    matchedTokensSize.clear();
//    matchedTokensSize.assign(tokensPatterns.size(), 0);
}

void Rule::setTokensModifiers(std::vector<std::string> aTokensModifiers)
{
    tokensModifiers.clear();
    tokensModifiers.insert(tokensModifiers.begin(), aTokensModifiers.begin(), aTokensModifiers.end());
//    delete tokensModifiers;
//    tokensModifiers = new std::vector<std::string>(aTokensModifiers);
}

void Rule::setTokensRequired(std::vector<bool> aTokensRequired)
{
    tokensRequired.clear();
    tokensRequired.insert(tokensRequired.begin(), aTokensRequired.begin(), aTokensRequired.end());
//    delete tokensRequired;
//    tokensRequired = new std::vector<bool>(aTokensRequired);
}

void Rule::setMatchedIndices(std::vector<int> aMatchedIndices)
{
    matchedIndices.clear();
    matchedIndices.insert(matchedIndices.begin(), aMatchedIndices.begin(),
            aMatchedIndices.end());
//    delete matchedIndexes;
//    matchedIndexes = new std::vector<int>(aMatchedIndexes);
}

std::string Rule::getMatch() const {
    return match_;
}

std::string Rule::getLeft() const {
    return left_;
}

std::string Rule::getRight() const {
    return right_;
}

void Rule::addAction(ActionPtr action)
{
    actions->push_back(action);
}

void Rule::deleteAction(size_t index)
{
    if (index < actions->size())
        actions->erase(actions->begin() + index);
}

std::string Rule::makeReadable()
{
    std::stringstream ss;
    ss << "Rule \"" << name << "\"" << std::endl;

    if (leftCount > 0)
    {
        ss << left_ << std::endl;
    }
    ss << match_ << std::endl;
    if (rightCount > 0)
    {
        ss << right_ << std::endl;
    }
    ss << "Eval: ";
    for (Actions::iterator a = actions->begin(); a != actions->end(); ++ a)
    {
        if ((*a)->getType() == "delete")
            ss << "delete(" << (boost::dynamic_pointer_cast<DeleteAction>(*a))->getUPattern() << ", " << (boost::dynamic_pointer_cast<DeleteAction>(*a))->getTokenIndex() << ");" << std::endl;
        else if ((*a)->getType() == "add")
        {
            ss << "add(" << (boost::dynamic_pointer_cast<AddAction>(*a))->getUInterpretation() << ", " << "\"";
            if ((boost::dynamic_pointer_cast<AddAction>(*a))->getBase() != "[^<>]+")
                ss << (boost::dynamic_pointer_cast<AddAction>(*a))->getBase();
            else
                ss << "base";
            ss << "\"" << ", " << (boost::dynamic_pointer_cast<AddAction>(*a))->getTokenIndex() << ");" << std::endl;
        }
        else if ((*a)->getType() == "unify")
        {
            ss << "unify(";
            std::vector<std::string> attributes = (boost::dynamic_pointer_cast<UnifyAction>(*a))->getUAttributes();
            std::vector<int> indices = (boost::dynamic_pointer_cast<UnifyAction>(*a))->getTokenIndices();

            for (std::vector<std::string>::iterator it = attributes.begin(); it != attributes.end(); ++ it)
            {
                if (it != attributes.begin())
                    ss << " ";
                ss << *it;
            }
            for (std::vector<int>::iterator it = indices.begin(); it != indices.end(); ++ it)
                ss << ", " << *it;
            ss << ");" << std::endl;
        }
        else if ((*a)->getType() == "syntok")
        {
            ss << "syntok(\"" << (boost::dynamic_pointer_cast<SyntokAction>(*a))->getUMorphology() << "\");" << std::endl;
        }
        else if ((*a)->getType() == "group")
            ss << "group(" << (boost::dynamic_pointer_cast<GroupAction>(*a))->getGroup() << ", " << (boost::dynamic_pointer_cast<GroupAction>(*a))->getHead() << ");" << std::endl;
        else if ((*a)->getType() == "join")
            ss << "join(" << (boost::dynamic_pointer_cast<JoinAction>(*a))->getGroup() << ", " << (boost::dynamic_pointer_cast<JoinAction>(*a))->getHead() << ");" << std::endl;
        else if ((*a)->getType() == "attach")
            ss << "attach(" << (boost::dynamic_pointer_cast<AttachAction>(*a))->getGroup() << ", " << (boost::dynamic_pointer_cast<AttachAction>(*a))->getHead() << ");" << std::endl;
        else if ((*a)->getType() == "transform")
            ss << "transform(" << (boost::dynamic_pointer_cast<TransformAction>(*a))->getElement() << ", " << (boost::dynamic_pointer_cast<TransformAction>(*a))->getGroup() << ");" << std::endl;
    }

    return ss.str();
}


        int Rule::getPatternStart(std::string &beforePattern) {
            int r = -1;

            Pattern regPatternInfo("<<[tsg]<\\d+<(\\d+)[^>]+>$");
            int end;
            if (RegExp::PartialMatch(beforePattern, regPatternInfo, &end)) {
                r = end;
            }

            return r;
        }

}

}

}

