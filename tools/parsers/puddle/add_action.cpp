
#include "add_action.hpp"
#include <iostream>
//#include "token.hpp"
//#include "group.hpp"
//#include "syntok.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

//AddAction::AddAction(std::vector<InterpretationPair> aInterpretations, std::string aBase, int aTokenIndex, std::string uInterpretation)
AddAction::AddAction(std::vector<std::string> aInterpretations, std::string aBase,
        int aTokenIndex, std::string uInterpretation) {
    base = aBase;
    tokenIndex = aTokenIndex;
    type = "add";
    verbose = false;

    //interpretations = new std::vector<InterpretationPair>(aInterpretations);
    interpretations.insert(interpretations.begin(), aInterpretations.begin(),
            aInterpretations.end());
    //interpretations = aInterpretations;

    interpretation_ = uInterpretation; //@todo: to jest gdzie w ogole tu potrzebne? czy w syntoku?
}

AddAction::~AddAction()
{
    //delete interpretations;
}

//bool AddAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
bool AddAction::apply(ParseGraphPtr pg, int currentEntity,
        std::vector<int> matchedTokensSize) {
    int count = matchedTokensSize[tokenIndex - 1];
    if (count == 0)
    {
//        std::cout << "Nothing matched to " << tokenIndex << " in ...." << std::endl;
        return true;
    }
    int before = 0;
    int i = 0;
    while (i < (tokenIndex - 1))
    {
        before += matchedTokensSize[i];
        i ++;
    }

    bool allBaseForms = (base == "[^<>]+");
    for (int edge_i = before; edge_i < (before + count); edge_i ++) {
        TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
        //for (std::vector<InterpretationPair>::iterator interp_it =
        for (std::vector<std::string>::iterator morph_it =
                interpretations.begin();
                morph_it != interpretations.end();
                morph_it ++) {
            if (! allBaseForms) {
                PosInfo pi(base, *morph_it, 1);
                edge->addMorphology(pi);
            } else {
                for (std::vector<PosInfo>::iterator var_it =
                        edge->variants_.begin();
                        var_it != edge->variants_.end();
                        var_it ++) {
                    std::string orig_base = boost::get<0>(*var_it);
                    PosInfo pi(orig_base, *morph_it, 1);
                    edge->addMorphology(pi);
                }
            }
        }
    }

/*    for (int entIndex = before; entIndex < (before + count); entIndex ++)
    {
        Entity *entity = entities[currentEntity + entIndex];

        bool allBases = false;

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

            if (base == "[^<>]+")
                allBases = true;
            if (allBases)
            {
                std::set<std::string> baseForms;// = new std::set<std::string*>;
                std::vector<std::string> morpho = token->getMorphology();
                std::vector<std::string>::iterator m = morpho.begin();
                while (m != morpho.end())
                {
                    std::string baseform = (*m).substr(0, (*m).find(":", 1));
                    baseForms.insert(baseform);
                    m ++;
                }
                std::set<std::string>::iterator b = baseForms.begin();
                while (b != baseForms.end())
                {
                    std::vector<InterpretationPair>::iterator i = interpretations.begin();
                    while (i != interpretations.end())
                    {
                        m = morpho.begin();
                        while (m != morpho.end())
                        {
                            if (*m == (*b + ":" + i->first))
                                break;
                            m ++;
                        }
                        if (m == morpho.end()) // jesli przeszedl wszystkie interpretacje i nie na trafil na ta dokladana, czyli trzeba dolozyc!
                        {
                            std::string morph = *b + ":" + i->first;
                            std::string comp = i->second + *b;
                            token->addInterpretation(morph, comp);
                            //delete morph;
                            //delete comp;
                            (*e)->variants_.push_back(PosInfo(*b, i->first, 1));
                        }
                        i ++;
                    }
                    b ++;
                }
                //b = baseForms.begin();
                //while (b != baseForms.end())
                //{
                //    delete *b;
                //    b ++;
                //}
                //delete baseForms;

            }
            else
            {
                std::vector<std::string> morpho = token->getMorphology();
                //   bool changed = token.addInterpretation(this->pattern);  //TODO: te 3 linijki w jedna, zeby oszczedzic wszystkiego!
                std::vector<InterpretationPair>::iterator i = interpretations.begin();
                while (i != interpretations.end())
                {
                    std::vector<std::string>::iterator m = morpho.begin();
                    while (m != morpho.end())
                    {
                        if (*m == (base + ":" + i->first))
                            break;
                        m ++;
                    }
                    if (m == morpho.end()) // jesli przeszedl wszystkie interpretacje i nie na trafil na ta dokladana, czyli trzeba dolozyc!
                    {
                        std::string morph = base + ":" + i->first;
                        std::string comp = i->second + base;
                        token->addInterpretation(morph, comp);
                        //delete morph;
                        //delete comp;
                        (*e)->variants_.push_back(PosInfo(base, i->first, 1));
                    }
                    i ++;
                }
            }

            std::vector<PosInfo> var = (*e)->variants_;
            for (e = edges.begin(); e != edges.end(); e ++)
            {
                if (((*e)->isPhrasal()) && ((*e)->getHead() == id))
                {
                    (*e)->variants_ = var;
                }
            }
            //delete id;

            token->updateCompiled();
            entities[currentEntity + entIndex] = token;
        }
        else if (entity->getType() == "group")
        {
            Group *group = (Group*) entity;
            //Token *head = group->getHeadToken();
            while ((group->getHeadToken())->getType() == "group")
            {
                group = (Group*) (group->getHeadToken());
            }

            int index = group->getHeadIndex();
            int current = index - entIndex;
            return apply(group->children, edges, current, matchedTokensSize);
        }
        else if (entity->getType() == "syntok")
        {
            Syntok *syntok = (Syntok*) entity; //entities[currentEntity + (this->getTokenIndex() - 1)];
            std::string id = syntok->getId();
            Edges::iterator e;
            for (e = edges.begin(); e != edges.end(); e ++)
            {
                if ((*e)->getId() == id)
                    break;
            }
            if (e == edges.end())
            {
                std::cout << "Nie ma takiego id tu: " << id << std::endl;
            }
            //delete id;

            if (base == "[^<>]+")
                allBases = true;
            if (allBases)
            {
                std::set<std::string> baseForms;// = new std::set<std::string*>;
                std::vector<std::string> morpho = syntok->getMorphology();
                std::vector<std::string>::iterator m = morpho.begin();
                while (m != morpho.end())
                {
                    std::string baseform = m->substr(0, m->find(":", 1));
                    baseForms.insert(baseform);
                    m ++;
                }
                std::set<std::string>::iterator b = baseForms.begin();
                while (b != baseForms.end())
                {
                    std::vector<InterpretationPair>::iterator i = interpretations.begin();
                    while (i != interpretations.end())
                    {
                        m = morpho.begin();
                        while (m != morpho.end())
                        {
                            if (*m == (*b + ":" + i->first))
                                break;
                            m ++;
                        }
                        if (m == morpho.end()) // jesli przeszedl wszystkie interpretacje i nie na trafil na ta dokladana, czyli trzeba dolozyc!
                        {
                            std::string morph = *b + ":" + i->first;
                            std::string comp = i->second + *b;
                            syntok->addInterpretation(morph, comp);
                            //delete morph;
                            //delete comp;
                            //((*e)->variants_).push_back(PosInfo(*b, i->first, 1));
                            (*e)->addMorphology(PosInfo(*b, i->first, 1));
                        }
                        i ++;
                    }
                    b ++;
                }
                //b = baseForms->begin();
                //while (b != baseForms->end())
                //{
                //    delete *b;
                //    b ++;
                //}
                //delete baseForms;

            }
            else
            {
                std::vector<std::string> morpho = syntok->getMorphology();
                //   bool changed = token.addInterpretation(this->pattern);  //TODO: te 3 linijki w jedna, zeby oszczedzic wszystkiego!
                std::vector<InterpretationPair>::iterator i = interpretations.begin();
                while (i != interpretations.end())
                {
                    std::vector<std::string>::iterator m = morpho.begin();
                    while (m != morpho.end())
                    {
                        if (*m == (base + ":" + i->first))
                            break;
                        m ++;
                    }
                    if (m == morpho.end()) // jesli przeszedl wszystkie interpretacje i nie na trafil na ta dokladana, czyli trzeba dolozyc!
                    {
                        std::string morph = base + ":" + i->first;
                        std::string comp = i->second + base;
                        syntok->addInterpretation(morph, comp);
                        //delete morph;
                        //delete comp;
                        (*e)->variants_.push_back(PosInfo(base, i->first, 1));
                    }
                    i ++;
                }
            }
            syntok->updateCompiled();
            entities[currentEntity + entIndex] = syntok;

        }
    }*/
    return true;
}

//bool AddAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
bool AddAction::test(ParseGraphPtr pg, int currentEntity,
        std::vector<int> matchedTokensSize) {

    int count = matchedTokensSize[tokenIndex - 1];
    if (count == 0) {
        if (verbose)
            std::cerr << "Nothing matched to " << tokenIndex << " in add ...." << std::endl;
        return true;
    }
    int before = 0;
    int i = 0;
    while (i < (tokenIndex - 1)) {
        before += matchedTokensSize[i];
        i ++;
    }

    //check whether the interpretation(s) (with the given or all base forms
    //of the token) which is/are to be added is/are not already in token's
    //morphological features
    bool ret = true; //@todo: moze by tak zmienic ret w akcjach na cos w stylu to_apply

    bool allBaseForms = (base == "[^<>]+");
    for (int edge_i = before; edge_i < (before + count); edge_i ++) {
        TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
        for (std::vector<PosInfo>::iterator var_it =
                edge->variants_.begin();
                var_it != edge->variants_.end();
                var_it ++) {
            if (! boost::get<2>(*var_it) )
                continue; //skip discarded interpretations
            if (! allBaseForms) {
                if (boost::get<0>(*var_it) != base)
                    continue; //take the next variant
            }
            bool interpretationFound = false;
            //for (std::vector<InterpretationPair>::iterator interp_it =
            for (std::vector<std::string>::iterator morph_it =
                    interpretations.begin();
                    morph_it != interpretations.end();
                    morph_it ++) {
                if (boost::get<1>(*var_it) == *morph_it) {
                    interpretationFound = true;
                    break;
                }
            }
            if (interpretationFound) { //if found the interpration in the token, finish testing and do not add the interpration again
                ret = false;
                break;
            }
        }
    }
    //old version below
    /*
    for (int entIndex = before; entIndex < (before + count); entIndex ++)
    {

        Entity *entity = entities[currentEntity + entIndex];

        bool allBases = false;

        if (entity->getType() == "token")
        {
            Token *token = (Token*) entity;
            if (base == "[^<>]+")
                allBases = true;
            if (allBases)
            {
                std::set<std::string> baseForms;// = new std::set<std::string*>;
                std::vector<std::string> morpho = token->getMorphology();
                std::vector<std::string>::iterator m = morpho.begin();
                while (m != morpho.end())
                {
                    std::string baseform = (*m).substr(0, (*m).find(":", 1));
                    baseForms.insert(baseform);
                    m ++;
                }
                std::set<std::string>::iterator b = baseForms.begin();
                while (b != baseForms.end())
                {
                    std::vector<InterpretationPair>::iterator i = interpretations.begin();
                    while (i != interpretations.end())
                    {
                        m = morpho.begin();
                        while (m != morpho.end())
                        {
                            if (*m == (*b + ":" + i->first))
                                break;
                            m ++;
                        }
                        if (m == morpho.end()) // jesli przeszedl wszystkie interpretacje i nie na trafil na ta dokladana, czyli trzeba dolozyc!
                        {
                            ret = true;
                            break;
                        }
                        i ++;
                    }
                    b ++;
                }
                //b = baseForms->begin();
                //while (b != baseForms->end())
                //{
                //    delete *b;
                //    b ++;
                //}
                //delete baseForms;
            }
            else
            {
                std::vector<std::string> morpho = token->getMorphology();
                std::vector<InterpretationPair>::iterator i = interpretations.begin();
                while (i != interpretations.end())
                {
                    std::vector<std::string>::iterator m = morpho.begin();
                    while (m != morpho.end())
                    {
                        if (*m == (base + ":" + i->first))
                            break;
                        m ++;
                    }
                    if (m == morpho.end()) // jesli przeszedl wszystkie interpretacje i nie na trafil na ta dokladana, czyli trzeba dolozyc!
                    {
                        ret = true;
                        break;
                    }
                    i ++;
                }
            }
        }
        else if (entity->getType() == "group")
        {
            Group *group = (Group*) entity;
            //Token *head = group->getHeadToken();
            while ((group->getHeadToken())->getType() == "group")
            {
                group = (Group*) (group->getHeadToken());
            }

            int index = group->getHeadIndex();
            int current = index - entIndex;
            return test(group->children, current, matchedTokensSize);
        }
        else if (entity->getType() == "syntok")
        {
            Syntok *syntok = (Syntok*) entity;
            if (base == "[^<>]+")
                allBases = true;
            if (allBases)
            {
                std::set<std::string> baseForms;// = new std::set<std::string*>;
                std::vector<std::string> morpho = syntok->getMorphology();
                std::vector<std::string>::iterator m = morpho.begin();
                while (m != morpho.end())
                {
                    std::string baseform = m->substr(0, m->find(":", 1));
                    baseForms.insert(baseform);
                    m ++;
                }
                std::set<std::string>::iterator b = baseForms.begin();
                while (b != baseForms.end())
                {
                    std::vector<InterpretationPair>::iterator i = interpretations.begin();
                    while (i != interpretations.end())
                    {
                        m = morpho.begin();
                        while (m != morpho.end())
                        {
                            if (*m == (*b + ":" + i->first))
                                break;
                            m ++;
                        }
                        if (m == morpho.end()) // jesli przeszedl wszystkie interpretacje i nie na trafil na ta dokladana, czyli trzeba dolozyc!
                        {
                            ret = true;
                            break;
                        }
                        i ++;
                    }
                    b ++;
                }
                //b = baseForms->begin();
                //while (b != baseForms->end())
                //{
                //    delete *b;
                //    b ++;
                //}
                //delete baseForms;
            }
            else
            {
                std::vector<std::string> morpho = syntok->getMorphology();
                std::vector<InterpretationPair>::iterator i = interpretations.begin();
                while (i != interpretations.end())
                {
                    std::vector<std::string>::iterator m = morpho.begin();
                    while (m != morpho.end())
                    {
                        if (*m == (base + ":" + i->first))
                            break;
                        m ++;
                    }
                    if (m == morpho.end()) // jesli przeszedl wszystkie interpretacje i nie na trafil na ta dokladana, czyli trzeba dolozyc!
                    {
                        ret = true;
                        break;
                    }
                    i ++;
                }
            }

        }
    }*/

    return ret;
}

//void AddAction::setInterpretations(std::vector<InterpretationPair> aInterpretations)
void AddAction::setInterpretations(std::vector<std::string> aInterpretations) {
    interpretations.insert(interpretations.begin(), aInterpretations.begin(),
            aInterpretations.end());
}

std::vector<std::string> AddAction::getInterpretations() {
    return interpretations;
}

void AddAction::setBase(std::string aBase)
{
    base = aBase;
}

std::string AddAction::getBase()
{
    return base;
}

void AddAction::setTokenIndex(int aTokenIndex)
{
    tokenIndex = aTokenIndex;
}

int AddAction::getTokenIndex()
{
    return tokenIndex;
}

std::string AddAction::getUInterpretation()
{
    return interpretation_;
}

}

}

}
