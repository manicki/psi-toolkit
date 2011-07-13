
#include "join_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

//#include "TransitionGraph.hpp"
//#include "ParseGraph.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

JoinAction::JoinAction(std::string aGroup, int aStart, int aEnd, int aHead, std::string aRuleName)
{
    group = aGroup;
    start = aStart;
    end = aEnd;
    head = aHead; // - 1;
    if ((head > (end+1)) || (head <= start))
    {
        if (start == end)
            head = 0;
        else
            //head = start + (head % (end+1));
            head = (head % (end+1)) - start;
    }
//    std::cout << "Grupe zalozylem od " << start << " do " << end << " z glowa w: " << head << " dla reguly: " << aRuleName <<  std::endl;
    type = "join";
    ruleName = aRuleName;
    verbose = false;
}

JoinAction::~JoinAction()
{
}

bool JoinAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
{

//    std::cout << "Poczatek reguly: " << ruleName << std::endl;
//    std::cout << "PRZED mam elementow: " << entities.size() << std::endl;

    int realStart = 0;// start;
    int realEnd = 0;//start;
    int realHead = 0;//start;
    int i = 0;
    while (i < matchedTokensSize.size())
    {
        if (i < start)
            realStart += matchedTokensSize[i];
        if ((i >= start) && (i < (head - 1)))
        {
//            std::cout << "i = " << i << " zwiekszam o: " <<  matchedTokensSize[i] << std::endl;
//        if (i < (head))
            realHead += matchedTokensSize[i];
        }
        if (i <= end)
            realEnd += matchedTokensSize[i];
        else
            break;
        i ++;
    }
    //if (realHead > 0)
    //    realHead --;
    realEnd --;
    if (realStart > realEnd)
        return false;
//    if (realEnd != end)
//        std::cout << "Dysonans mam poznawczy, bo wyszlo: " << realEnd << " zamiast wczesniejszego " << end << std::endl;
//    if (realHead != head)
//        std::cout << "Dysonans mam poznawczy glowowy, bo wyszlo: " << realHead << " zamiast wczesniejszego " << head << std::endl;

//    std::cout << "Wymiary: " << std::endl << "Current: " << currentEntity << std::endl << "realStart: " << realStart << std::endl << "realEnd: " << realEnd << std::endl << "realHead: " << realHead << std::endl;

    //Group *gr = new Group(entities, currentEntity + realStart, currentEntity + realEnd, realHead, this->group, ruleName, ACTION_JOIN);
    Group *gr = new Group(entities, currentEntity + realStart, currentEntity + realEnd, currentEntity + realHead, this->group, ruleName, ACTION_JOIN);

//    std::cout << "grupa zrobiona" << std::endl;

//    for (int entIndex = realStart; entIndex < realEnd; entIndex++)
//    {
//        //    std::cerr << "W usuwaniu" << std::endl;
//        Entity *entity = entities[currentEntity + entIndex];
//        if (*(entity->getType()) == "group")
//        {
//            entities.erase(entities.begin() + currentEntity + entIndex);
//        }
//    }


    if ((currentEntity + start) < entities.size())
        entities.insert((entities.begin() + currentEntity + start), gr); //TODO: a nie +realStart?
    else
        entities.insert((entities.begin() + entities.size() - 1), gr); //TODO: a nie +realStart?
//        std::cerr << "====================================" << std::endl;
//        std::cerr << "Entities:"<<std::endl;
//        for (Entities::iterator hu = entities.begin(); hu != entities.end(); hu ++)
//        {
//            if ((*hu)->getType() == "group")
//            {
//                std::cerr << "   " << ((Group*)*hu)->getId() << ": " << ((Group*)*hu)->getGroupType() << std::endl;
//            }
//            else
//            {
//                std::cerr << "   " << ((Token*)*hu)->getId() << ": " << ((Token*)*hu)->getOrth() << std::endl;
//            }
//        }

    TransitionInfo *group = new TransitionInfo("group");
    //std::stringstream ss;
    //ss << std::hex << Group::groupId;
    group->setId(gr->getId()); //TODO id nadawanie!
    group->setLabel(this->group);
    group->setHead(((Token*)(gr->getHeadToken()))->getId());
    std::string startId, endId;
    Entity *left = gr->children.front();
    while (left->getType() == "group")
    {
        left = ((Group*)left)->children.front();
    }
    if (left->getType() == "token")
    {
        startId = ((Token*)left)->getId();
    }
    else if (left->getType() == "syntok")
    {
        startId = ((Syntok*)left)->getId();
//        std::cout << "to nie tokien, tylko: " << left->getType() << "!" << std::endl;
//        std::cout << "On ma id: " << left->getId() << std::endl; // << ", a w iksemelu wyglada tak: " << left->toXml() << std::endl;
    }
//    std::cout << "wielkosc grupy: " << gr->children.size() << " w regule: " << ruleName << std::endl;
    Entity *right = gr->children.back(); //[gr->children.size() - 1];
    while (right->getType() == "group")
        right = ((Group*)right)->children.back(); //[((Group*)right)->children.size() - 1];
    if (right->getType() == "token")
        endId = ((Token*)right)->getId();
    else if (right->getType() == "syntok")
        endId = ((Syntok*)right)->getId();
    
    Edges::iterator e = edges.begin();// + currentEntity; //TODO:czy to jest zawsze prawda, ze taki przeskok mozna se zrobic? na te chwile strzelam, ze tak...
    int startIndex = 0; //currentEntity;
    while (e != edges.end())
    {
        if ((*e)->getId() == startId)
            break;
        startIndex ++;
        e ++;
    }
    if (e == edges.end())
        startIndex = -1;

    if (startIndex == -1)
    {
        std::cerr << "NIE ZNALAZLEM startu: " << startId << "!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        delete gr;
        delete group;
        return false;
    }

//    endId = boost::lexical_cast<std::string>(boost::lexical_cast<int>(endId) + 1);
    e = edges.begin();// + currentEntity; //TODO:czy to jest zawsze prawda, ze taki przeskok mozna se zrobic? na te chwile strzelam, ze tak...
    int endIndex = 0;// currentEntity;
    while (e != edges.end())
    {
        if ((*e)->getId() == endId)
            break;
        endIndex ++;
        e ++;
    }
    if (e == edges.end())
        endIndex = -1;

    if (endIndex == -1)
    {
        std::cerr << "Poczatek: " << startId << " a koniec: " << endId << std::endl;
        std::cerr << "NIE ZNALAZLEM konca: " << endId << " w regule: " << ruleName << "!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        delete gr;
        delete group;
        return false;
    }

    e = edges.begin();// + currentEntity; //TODO:czy to jest zawsze prawda, ze taki przeskok mozna se zrobic? na te chwile strzelam, ze tak...
    int headIndex = 0;// currentEntity;
    while (e != edges.end())
    {
        if ((*e)->getId() == group->getHead())
        {
//            std::vector<int> dsc = (*e)->desc;
//            group->setDesc(dsc);
            break;
        }
        headIndex ++;
        e ++;
    }
    if (e == edges.end())
        headIndex = -1;

    if (headIndex == -1)
    {
        std::cerr << "NIE ZNALAZLEM glowy: " << group->getHead() << " w regule: " << ruleName << "!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        delete gr;
        delete group;
        return false;
    }

    //std::vector<PosInfo> *vars = new std::vector<PosInfo>(edges[headIndex]->variants_);
    for (std::vector<PosInfo>::iterator pi = edges[headIndex]->variants_.begin(); pi != edges[headIndex]->variants_.end(); pi ++)
    {
        group->addMorphology(*pi);
    }
    //delete vars;

    group->setStart(edges[startIndex]->getStart());
    group->setEnd(edges[endIndex]->getEnd());

    //Graph g;
    e = edges.begin();
    int max = 1;
    while (e != edges.end())
    {
        if ((*e)->getStart() == group->getStart())
            if ((*e)->getDepth() > max)
                max = (*e)->getDepth();
    //    add_edge((*e)->getStart(), (*e)->getEnd(), **e, g);
        e ++;
    }
    if (group->getStart() != headIndex)
        max ++;
    std::map<int, int> maxes;
    e = edges.begin();
    while (e != edges.end())
    {
        if ((*e)->getStart() >= group->getStart() && (*e)->getEnd() <= group->getEnd())
        {
            if (maxes.find((*e)->getStart()) != maxes.end())
            {
                if (maxes[(*e)->getStart()] < (*e)->getDepth())
                    maxes[(*e)->getStart()] = (*e)->getDepth();
            }
            else
            {
                maxes.insert(std::pair<int, int>((*e)->getStart(), (*e)->getDepth()));
            }
        }
    //    add_edge((*e)->getStart(), (*e)->getEnd(), **e, g);
        e ++;
    }
    int e_start = group->getStart();
    e = edges.begin();
    while (e != edges.end())
    {
        if ((*e)->getStart() >= group->getStart() && (*e)->getEnd() <= group->getEnd())
        {
            if ((*e)->getStart() == e_start)
            {
                if ((*e)->getDepth() == maxes[e_start])
                {
                    if (maxes[e_start] > 1)
                    {
                        e_start = (*e)->getEnd();
                        if (!((*e)->getType() == "group" && (*e)->getLabel() == "SYNTOK"))
                        {
                            delete *e;
                            e = edges.erase(e);
                        }
                        if (e_start == group->getEnd())
                            break;
                        e = edges.begin();
                        continue;
                    }
                    else
                    {
                        e_start = (*e)->getEnd();
                        if (e_start == group->getEnd())
                            break;
                        continue;
                    }
                }
            }
        }
    //    add_edge((*e)->getStart(), (*e)->getEnd(), **e, g);
        e ++;
    }

    group->setDepth(max);

    //boost::graph_traits<Graph>::vertex_iterator u, v;
    //boost::tie(u, v) = boost::vertices(g);
    //u += (startIndex + 1);

    //while (u != v)
    //{
        //int deg = boost::out_degree(*u, g);
     //   u ++;
    //}

    //group->setDepth(deg);

//    std::cout << "PO mam elementow: " << entities.size() << std::endl;

    edges.push_back(group);

//    std::cout << "W akcji grupowania" << std::endl;
    return true;
}

bool JoinAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
{
    if (entities.size() < head)
    {
        return false;
    }
    if (matchedTokensSize[head - 1] == 0)
    {
//        std::cerr << "Element join: " << head - 1 << " empty!" << std::endl;
        return false;
    }
    return true;
}

std::string JoinAction::getGroup()
{
    return group;
}

int JoinAction::getHead()
{
    return head;
    //return (head + 1); 
}

void JoinAction::setGroup(std::string aGroup)
{
    group = aGroup;
}

void JoinAction::setHead(int aHead)
{
    head = aHead; // - 1;
}

int JoinAction::getStart()
{
    return start;
}

void JoinAction::setStart(int aStart)
{
    start = aStart;
}

int JoinAction::getEnd()
{
    return end;
}

void JoinAction::setEnd(int aEnd)
{
    end = aEnd;
}

std::string JoinAction::getRuleName()
{
    return ruleName;
}

void JoinAction::setRuleName(std::string aRuleName)
{
    ruleName = aRuleName;
}

}

}

}

