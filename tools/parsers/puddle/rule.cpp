
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

    std::vector<int>::iterator it = matchedIndices.begin();
    int i = 0;
    int j = 0; //@todo: dac tej zmiennej jakas lepsza nazwe
    while (it != matchedIndices.end()) {
        std::string part = match[*it].as_string();
        if ((part == "") && (tokensRequired[i])) {
            return false;
        }
        ruleTokenSizes.at(i) = countEntities(part);
        if (tokensRequired[i] && ruleTokenSizes[i] == 0) {
            return false;
        }
        j += ruleTokenSizes.at(i);
        i ++;
        ++ it;
    }
    if (j == 0) {
        return false;
    }

    bool ret = true;
    Actions::iterator ia = actions->begin();
    while (ia != actions->end()) {
        if ((*ia)->test(lattice, currentEntity, ruleTokenSizes) == false) {
            int limit;
            int lastIndex = leftCount + matchCount - 1;
            if (tokensModifiers[lastIndex] == "+" ||
                    tokensModifiers[lastIndex] == "") {
                limit = 1;
            } else {
                limit = 0;
            }
            if (ruleTokenSizes[lastIndex] > limit) {
                ruleTokenSizes[lastIndex] --;
                continue;
            }
            ret = false;
            break;
        }
        ++ ia;
    }

    return ret;
}

int Rule::countEntities(std::string matched) {
    int p = 0;

    StringPiece tmpMatched = matched;
    std::string str;
    while ( RegExp::FindAndConsumeN( &tmpMatched, "^<<[tsg]<\\d+<\\d+[^>]+>", 0, 0) ) {
        p ++;
    }
    return p;
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

