
#include <iostream>
#include "delete_action.hpp"
#include "token.hpp"
#include "group.hpp"
#include "syntok.hpp"

namespace poleng {

    namespace bonsai {
        namespace puddle {

DeleteAction::DeleteAction(std::string aPattern, int aTokenIndex, std::string uPattern) {
    //pattern = boost::make_u32regex(aPattern);
    //pattern();
    pattern = PatternPtr(new RE2(aPattern));
    patternString = aPattern;
    tokenIndex = aTokenIndex;
    type = "delete";
    verbose = false;
    pattern_ = uPattern;
}

bool DeleteAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
{
    int count = matchedTokensSize[tokenIndex - 1];
    if (count == 0)
    {
//        std::cout << "Nothing matched to " << tokenIndex << " in ...." << std::endl;
        return true;
    }

    if (nothingToDelete)
    {
//        std::cerr << "nic do delete" << std::endl;
        return false;
    }

    int before = 0;
    int i = 0;
    while (i < (tokenIndex - 1))
    {
        before += matchedTokensSize[i];
        i ++;
    }

//    std::cerr << "kurent entity: " << currentEntity << std::endl;

    bool ret = false;
    for (int entIndex = before; entIndex < (before + count); entIndex++)
    {
//    std::cerr << "W usuwaniu" << std::endl;
    Entity *entity = entities[currentEntity + entIndex];

    if (entity->getType() == "token")
    {
        Token *token = (Token*) entity; //entities[currentEntity + (this->getTokenIndex() - 1)];
        std::string id = token->getId();
        Edges::iterator e;
        for (e = edges.begin(); e != edges.end(); e ++)
        {
            if ((*e)->getId() == id)
                break;
        }
        //delete id;
//    std::cerr << "ROZMIAR PRZED: " << token->getCompiledInterpretations().size() << std::endl;
        ret = token->deleteInterpretation(this->pattern, *e);  //TODO: te 3 linijki w jedna, zeby oszczedzic wszystkiego!
//        std::vector<PosInfo> var = (*e)->variants_;

        if (ret)
            token->updateCompiled();

        entities[currentEntity + entIndex] = token;
//    std::cerr << "ROZMIAR PO: " << token->getCompiledInterpretations().size() << std::endl;
    }
    else if (entity->getType() == "group")
    {
        Group *group = (Group*) entity;
        while ((group->children.at(group->getHeadIndex()))->getType() == "group")
        {
            group = (Group*) (group->children.at(group->getHeadIndex()));
        }

        int index = group->getHeadIndex();

        if (group->children.at(index)->getType() == "token")
        {
        Token *token = (Token*) group->children.at(index);
        std::string id = token->getId();
        Edges::iterator e;
        for (e = edges.begin(); e != edges.end(); e ++)
        {
            if ((*e)->getId() == id)
                break;
        }

        ret = token->deleteInterpretation(this->pattern, *e);
        std::vector<PosInfo> var = (*e)->variants_;

        if (ret)
        {
            token->updateCompiled();
            ((Group*)entity)->updateCompiled();
        }
        group->children.at(index) = token;
//        if (group->getUnify())
//            group->reunify();

        for (e = edges.begin(); e != edges.end(); e ++)
        {
            if (((*e)->isPhrasal()) && ((*e)->getHead() == id))
            {
                (*e)->variants_ = var;
            }
        }
        //delete id;

        int current = index - entIndex;
        }
        else if (group->children.at(index)->getType() == "syntok")
        {
        Syntok *syntok = (Syntok*) group->children.at(index);
        std::string id = syntok->getId();
        Edges::iterator e;
        for (e = edges.begin(); e != edges.end(); e ++)
        {
            if ((*e)->getId() == id)
                break;
        }

        ret = syntok->deleteInterpretation(this->pattern, *e);
        std::vector<PosInfo> var = (*e)->variants_;

        if (ret)
        {
            syntok->updateCompiled();
            ((Group*)entity)->updateCompiled();
        }
        group->children.at(index) = syntok;
//        if (group->getUnify())
//            group->reunify();

        for (e = edges.begin(); e != edges.end(); e ++)
        {
            if (((*e)->isPhrasal()) && ((*e)->getHead() == id))
            {
                (*e)->variants_ = var;
            }
        }
        //delete id;

        int current = index - entIndex;
        }
        //    return apply(group->children, edges, current);
    }
    else if (entity->getType() == "syntok")
    {
        Syntok *syntok = (Syntok*) entity;
        std::string id = syntok->getId();
        Edges::iterator e;
        for (e = edges.begin(); e != edges.end(); e ++)
        {
            if ((*e)->getId() == id)
                break;
        }
        //delete id;
        ret = syntok->deleteInterpretation(this->pattern, *e);  //TODO: te 3 linijki w jedna, zeby oszczedzic wszystkiego!
        if (ret)
            syntok->updateCompiled();
        entities[currentEntity + entIndex] = syntok;
    }
    }

    if (!nothingToDelete)
    {
        if (!ret)
        {
//            std::cerr << "ten myk tu zaszedl" << std::endl;
            ret = true; // ten fragment dopisany przez regule "to"
                        // przed delete byla akcja unify, ktora zalatwiala robote akcji delete
                        // w wyniku czego delete zwracala falsza i cala regula sie nie wykonywala
                        // obserwowac trzeba, czy to tu dopisane nie rozwali czegos gdzie indziej
        }
    }
//    std::cerr << "zwracam: " << (ret ? "true" : "false") << std::endl;

    return ret;
    //return ret;
}

bool DeleteAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
{
//    std::cout << "Test usuwania" << std::endl;
    bool ret = false;
    nothingToDelete = false;

    int count = matchedTokensSize[tokenIndex - 1];
    if (count == 0)
    {
        if (verbose)
            std::cerr << "Nothing matched to " << tokenIndex << " in delete ...." << std::endl;
        return true;
    }
    int before = 0;
    int i = 0;
    while (i < (tokenIndex - 1))
    {
        before += matchedTokensSize[i];
        i ++;
    }

    for (int entIndex = before; entIndex < (count + before); entIndex ++)
    {

    Entity *entity = entities[currentEntity + entIndex];

    if (entity->getType() == "token")
    {
        Token *token = (Token*) entity; //entities[currentEntity + entIndex];
        std::vector<std::string> interps = token->getCompiledInterpretations();
        std::vector<std::string>::iterator i = interps.begin();
        while (i != interps.end())
        {
//            std::cout << "Interpretejszyn: " << *i << std::endl;
            std::string compare = token->getOrth() + "<" + *i;
         //   std::cerr << "Takie wyrazenie: " << pattern.str() << std::endl;
            //if (boost::u32regex_match(compare, pattern)) {
            if (RE2::FullMatch(compare, *pattern)) {
//                std::cout << "Wyrzucac bede token o indeksie: " << (currentEntity + entIndex) << std::endl;
                ret = true;
//                compare.reset();
           //     std::cerr << "Spasowalem: " << compare << " z " << pattern.str() << std::endl;
                break;
            }
//            else
//            {
//                std::cout << "Nie wyrzuce tokenu o indeksie: " << (currentEntity + entIndex) << std::endl;
//
////                std::cout << "Nie spasowalem: " << compare << " z " << pattern.str() << std::endl;
//            }
            i ++;
//            compare.reset();
        }
    }
    else if (entity->getType() == "group")
    {
        Group *group = (Group*) entity;
        while ((group->children.at(group->getHeadIndex()))->getType() == "group")
        {
            group = (Group*) (group->children.at(group->getHeadIndex()));
        }

        int index = group->getHeadIndex();

        Token *token = (Token*) group->children.at(index);
        std::vector<std::string> interps = token->getCompiledInterpretations();
        std::vector<std::string>::iterator i = interps.begin();
        while (i != interps.end())
        {
//            std::cout << "Interpretejszyn: " << *i << std::endl;
            std::string compare = token->getOrth() + "<" + *i;
        //    std::cerr << "Takie wyrazenie: " << pattern.str() << std::endl;
            //if (boost::u32regex_match(compare, pattern)) {
            if (RE2::FullMatch(compare, *pattern)) {
                ret = true;
                //compare.reset();
//                std::cerr << "Spasowalem: " << compare << " z " << getPattern() << std::endl;
                break;
            }
//            else
//            {
//                std::cout << "Nie spasowalem: " << compare << " z " << getPattern() << std::endl;
//            }
            i ++;
            //compare.reset();
        }

        int current = index - entIndex + 1;
        //return test(group->children, current);
    }
    else if (entity->getType() == "syntok")
    {
        Syntok *syntok = (Syntok*) entity;
        std::vector<std::string> interps = syntok->getCompiledInterpretations();
        std::vector<std::string>::iterator i = interps.begin();
        while (i != interps.end())
        {
            std::string compare = syntok->getOrth() + "<" + *i;
            //if (boost::u32regex_match(compare, pattern))
            if (RE2::FullMatch(compare, *pattern)) {
                ret = true;
                //compare.reset();
                break;
            }
            i ++;
            //compare.reset();
        }
    }

    }

    if (!ret)
    {
        nothingToDelete = true;
        ret = true;
    }

//    std::cout << "Wzorzec usuwanych: " << this->getPattern() << std::endl;
    return ret;
}

void DeleteAction::setPattern(std::string aPattern)
{
    //pattern = boost::make_u32regex(aPattern);
    pattern = PatternPtr(new RE2(aPattern));
    patternString = aPattern;
}

std::string DeleteAction::getPattern()
{
    return patternString;
//    return pattern.str();
}

int DeleteAction::getTokenIndex()
{
    return tokenIndex;
}

void DeleteAction::setTokenIndex(int aTokenIndex)
{
    tokenIndex = aTokenIndex;
}

std::string DeleteAction::getUPattern()
{
    return pattern_;
}

}

}

}
