
#include "syntok_action.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

SyntokAction::SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndexes, std::vector<InterpretationPair> aMorphology, std::string aRuleName, std::string uMorphology)
{
    type = "syntok";
    start = aStart;
    end = aEnd;
    ruleName = aRuleName;
    verbose = false;
    syntok = false;
    //tokenIndexes = aTokenIndexes;
    tokenIndexes.insert(tokenIndexes.begin(), aTokenIndexes.begin(), aTokenIndexes.end());// = new std::vector<int>(aTokenIndexes);
    //morphology = aMorphology;
    morphology.insert(morphology.begin(), aMorphology.begin(), aMorphology.end());// = new std::vector<InterpretationPair>(aMorphology);

    morphology_ = uMorphology;
}

SyntokAction::~SyntokAction()
{
    //delete tokenIndexes;
    //delete morphology;
}

bool SyntokAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
{
    int realStart = 0;// start;
    int realEnd = 0;//start;
    int i = 0;
    while (i < matchedTokensSize.size())
    {
        if (i < start)
            realStart += matchedTokensSize[i];
        if (i <= end)
            realEnd += matchedTokensSize[i];
        else
            break;
        i ++;
    }
    realEnd --;

    std::vector<std::string> concatenated;// = new std::vector<StringPtr>;
    std::vector<int>::iterator ie = tokenIndexes.begin();
    while (ie != tokenIndexes.end())
    {
        int count = matchedTokensSize[*ie - 1];
        if (count == 0)
        {
            ie ++;
            continue;
        }
        int before = 0;
        int i = 0;
        while (i < (*ie - 1))
        {
            before += matchedTokensSize[i];
            i ++;
        }

        for (int index = before; index < (before + count); index ++)
        {
            Entity *entity = entities[currentEntity + index];//currentEntity + index];
            std::set<std::string> bases;// = new std::set<std::string>;
            if (entity->getType() == "token")
            {
                std::vector<std::string> morpho = ((Token*)entity)->getMorphology();
                std::vector<std::string>::iterator m = morpho.begin();
                while (m != morpho.end())
                {
                    std::string base = m->substr(0, m->find(":", 1));
                    if (bases.find(base) == bases.end())
                    {
                        bases.insert(base);
                    }
                    m ++;
                    //base.reset();
                    //delete base;
                }
            }
            else if (entity->getType() == "syntok")
            {
                std::vector<std::string> morpho = ((Syntok*)entity)->getMorphology();
                std::vector<std::string>::iterator m = morpho.begin();
                while (m != morpho.end())
                {
                    std::string base = m->substr(0, m->find(":", 1));
                    if (bases.find(base) == bases.end())
                    {
//                        std::cout << "Taka baza: " << base << std::endl;
                        bases.insert(base);
                    }
                    m ++;
                    //base.reset();
                    //delete base;
                }
            }

            if (concatenated.size() == 0)
            {
                std::set<std::string>::iterator b = bases.begin();
                while (b != bases.end())
                {
                    concatenated.push_back(*b);
                    b ++;
                }
            }
            else
            {
                std::vector<std::string> newConcatenated;// = new std::vector<StringPtr>;
                std::vector<std::string>::iterator c = concatenated.begin();
                while (c != concatenated.end())
                {
                    std::set<std::string>::iterator b = bases.begin();
                    while (b != bases.end())
                    {
                        newConcatenated.push_back(*c + " " + *b);
                        b ++;
                    }
                    c ++;
                }
                concatenated.assign(newConcatenated.begin(), newConcatenated.end());
                newConcatenated.clear();
                //delete newConcatenated;
            }
            bases.clear();
            //delete bases;
        }

        ie ++;
    }

    Syntok *syntok = new Syntok(entities, currentEntity + realStart, currentEntity + realEnd, ruleName);

        for (std::vector<std::string>::iterator conc = concatenated.begin(); conc != concatenated.end(); conc ++)
        {
            for (std::vector<InterpretationPair>::iterator ip = morphology.begin(); ip != morphology.end(); ip ++)
            {
                std::string morph = *conc + ":" + ip->first;
                std::string comp = ip->second + *conc;
                syntok->addInterpretation(morph, comp);
                //delete morph;
                //delete comp;
            }
        }

//    std::cout << "robie regule: " << ruleName << std::endl;

    syntok->updateCompiled();
    entities.insert(entities.begin() + currentEntity + realStart, syntok);

    if (syntok)
    {
        //TransitionInfo *wd = new TransitionInfo("token"); //byl word, co mi to zmieni?
        TransitionInfo *wd = new TransitionInfo("group"); //byl word, co mi to zmieni?
        wd->setId(syntok->getId());
        //wd->setLabel(*(syntok->getOrth()));
        wd->setLabel("SYNTOK");
        std::istringstream iss;// = new std::istringstream;

        //iss.str (word->contents[0]->getId());
        iss.str(syntok->getStart());
        int num;
        iss >> std::hex >> num;
        //    wd->setStart(boost::lexical_cast<int>(word->contents[0]->getId()));
        //    wd->setEnd(boost::lexical_cast<int>(word->contents[word->contents.size() - 1]->getId()));
        wd->setStart(num - 1);

//        std::cerr << "ustawiam start na: " << num << std::endl;
        //delete iss;

        std::istringstream iss2;// = new std::istringstream;
        //iss2.str (word->contents[word->contents.size() - 1]->getId());
        iss2.str (syntok->getEnd());
        //std::cout << iss2.str() << std::endl;
        int num2;
        iss2 >> std::hex >> num2;
        wd->setEnd(num2);
//        std::cerr << "ustawiam koniec na: " << (num2 + 1) << std::endl;
        //delete iss2;

        for (std::vector<std::string>::iterator conc = concatenated.begin(); conc != concatenated.end(); conc ++)
        {
            for (std::vector<InterpretationPair>::iterator ip = morphology.begin(); ip != morphology.end(); ip ++)
            {
                wd->addMorphology(PosInfo(*conc, ip->first, 1));
            }
        }

        Edges::iterator e = edges.begin();
        int max = 1;
        while (e != edges.end())
        {
            if ((*e)->getStart() == wd->getStart())
                if ((*e)->getDepth() > max)
                    max = (*e)->getDepth();
            //    add_edge((*e)->getStart(), (*e)->getEnd(), **e, g);
            e ++;
        }
        max ++;

        //wd->setDepth(max); //TODO
        //wd->setDepth(0); //TODO: jak tu nadawac te poziomy w koncu!
        wd->setDepth(2);

        edges.push_back(wd);
    }
    else
    {
        for (std::vector<std::string>::iterator conc = concatenated.begin(); conc != concatenated.end(); conc ++)
        {
            for (std::vector<InterpretationPair>::iterator ip = morphology.begin(); ip != morphology.end(); ip ++)
            {
                std::string morph = *conc + ":" + ip->first;
                std::string comp = ip->second + *conc;
                syntok->addInterpretation(morph, comp);
                //delete morph;
                //delete comp;
            }
        }
    }

    concatenated.clear();
    //delete concatenated;

    return true;
}

bool SyntokAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
{
    //int sum = 0;
    for (std::vector<int>::iterator i = matchedTokensSize.begin(); i != matchedTokensSize.end(); i ++)
    {
        if (*i > 0)
            return true;
    }
    return false;
}

int SyntokAction::getStart()
{
    return start;
}

void SyntokAction::setStart(int aStart)
{
    start = aStart;
}

int SyntokAction::getEnd()
{
    return end;
}

void SyntokAction::setEnd(int aEnd)
{
    end = aEnd;
}

std::vector<InterpretationPair> SyntokAction::getMorphology()
{
    return morphology;
}

void SyntokAction::setMorphology(std::vector<InterpretationPair> aMorphology)
{
    morphology.clear();
    morphology.insert(morphology.begin(), aMorphology.begin(), aMorphology.end());
    //morphology = &aMorphology;
}

std::vector<int> SyntokAction::getTokenIndexes()
{
    return tokenIndexes;
}

void SyntokAction::setTokenIndexes(std::vector<int> aTokenIndexes)
{
    tokenIndexes.clear();
    tokenIndexes.insert(tokenIndexes.begin(), aTokenIndexes.begin(), aTokenIndexes.end());
    //tokenIndexes = &aTokenIndexes;
}

std::string SyntokAction::getRuleName()
{
    return ruleName;
}

void SyntokAction::setRuleName(std::string aRuleName)
{
    ruleName = aRuleName;
}

std::string SyntokAction::getUMorphology()
{
    return morphology_;
}

}

}

}
