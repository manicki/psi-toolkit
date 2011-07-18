
#include <iostream>
#include <sstream>
#include "group.hpp"
#include "token.hpp"
#include "unify_action.hpp"
//#include "TransitionInfo.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

int Group::groupId;

Group::Group()
{
    type = "group";
    compiledGroup = "";
    ruleName = "";
    groupType = "";
    std::stringstream ss;
    ss << std::hex << groupId;
    id = ss.str();
//    delete ss;
    groupId++;
//    children = new Entities;
//    unify = false;
}

Group::Group(Entities &entities, int startIndex, int endIndex, int aHeadIndex, std::string aGroupType, std::string aRuleName, int action_type)
{
    std::stringstream ss;
    ss << std::hex << groupId;
    id = ss.str();
//    delete ss;
    groupId++;
//    unify = false;

    type = "group";

//    children = new Entities;

//    std::cout << "Grupe robie typu " << aGroupType << " od " << startIndex << " do " << endIndex << " z glowa w " << aHeadIndex << " z reguly: " << aRuleName << std::endl;

    //std::cerr << "aheadindex: " << aHeadIndex << std::endl;
    //std::cerr << "entitisow: " << entities.size() << std::endl;

    bool head_is_group = false;
    if (action_type == ACTION_GROUP)
    {
        int i = startIndex;
        Entities::iterator it = entities.begin() + i;
        while (i <= endIndex)
        {
            Entity *entity = *it;//entities[i];
//        if (!entity)
//        {
//            std::cout << "Na poczatku mam nula" << std::endl;
//        }
//        else
//        {
//            std::cout << "Na poczatku nie mam nula" << std::endl;
//        }

//        std::cout << "Ciskam do dzieciokow typa: " << entity->getType() << std::endl;
            this->children.push_back(entity);

            it = entities.erase(it);

//        if (!entity)
//        {
//            std::cout << "Mam nula po usunieciu" << std::endl;
//        }
//        else
//        {
//            std::cout << "Nie mam nula po usunieciu" << std::endl;
//        }

            i ++;
        }
    }
    else if (action_type == ACTION_JOIN)
    {
        int i = startIndex;
        int deleted = 0;
        int added = 0;
        Entities::iterator it = entities.begin() + i - deleted;
        while (i <= endIndex)
        {
            Entity *entity = *it;//entities[i];
            if (entity->getType() == "group")
            {
                for (Entities::iterator ii = ((Group*)entity)->children.begin(); ii != ((Group*)entity)->children.end(); ii ++)
                {
                    this->children.push_back(*ii);
                }

                if (i == aHeadIndex)
                {
                    headIndex = ((Group*)entity)->getHeadIndex() + added;
                    head_is_group = true;
                    //headIndex = ((Group*)entity)->getHeadIndex();
                }

                deleted ++;

                ((Group*)(*it))->children.clear();
//                std::cerr << "usuwam j: " << ((Group*)(*it))->getGroupType() << std::endl;
                delete ((Group*)(*it));
            }
            else
            {
                this->children.push_back(entity);
                added ++;
            }

            it = entities.erase(it);
            i ++;
        }
    }
    else if (action_type == ACTION_ATTACH)
    {
        int i = startIndex;
        int deleted = 0;
        int added = 0;
        //Entities::iterator it = entities.begin() + i;
        Entities::iterator it = entities.begin() + i - deleted;
        while (i <= endIndex)
        {
            Entity *entity = *it;//entities[i];
            if ((entity->getType() == "group") && (i == aHeadIndex))
            {
                for (Entities::iterator ii = ((Group*)entity)->children.begin(); ii != ((Group*)entity)->children.end(); ii ++)
                {
                    this->children.push_back(*ii);
                }
                    headIndex = ((Group*)entity)->getHeadIndex() + added;
                    head_is_group = true;
                //headIndex = ((Group*)entity)->getHeadIndex();
                ((Group*)(*it))->children.clear();
//                std::cerr << "usuwam a: " << ((Group*)(*it))->getGroupType() << std::endl;
                delete ((Group*)(*it));
            }
            else
            {
                this->children.push_back(entity);
                added ++;
            }

            it = entities.erase(it);
            i ++;
        }
    }

    if (action_type == ACTION_GROUP)
        headIndex = aHeadIndex;
    else
    {
        if (!head_is_group)
        {
            headIndex = 0;
        }
    }
    //if (children[headIndex]->getType() == "group")
    //    headIndex = ((Group*)children[headIndex])->getHeadIndex();
    //else
    //    headIndex = (children[headIndex])->getId();

    groupType = aGroupType;
    ruleName = aRuleName;

    //Token *token = (Token*)(children[headIndex]);
    //compiledGroup = "<<g<" + id + "<" + groupType + ">" + token->getCompiled();

    //compiledGroup = "<<g<" + id + "<" + groupType + ">" + ((Token*)this->getHeadToken())->getCompiled();
    compiledGroup = "<<g<" + id + "<" + groupType;
    std::string head = ((Token*)this->getHeadToken())->getCompiled();
    //std::cerr << "heduwa: " << head << std::endl;
    //std::cerr << "id head tokenu dla czytelnosci: " << ((Token*)this->getHeadToken())->getId() << std::endl;
    compiledGroup += head.substr(3, std::string::npos);


//    head.reset();
}

Group::~Group()
{
    Entities::iterator ei = children.begin();
//    std::cerr << "W grupie:" << std::endl;
    while (ei != children.end())
    {
        if ((*ei)->getType() == "token")
            delete ((Token*)(*ei));
        else if ((*ei)->getType() == "group")
//        {
//            std::cerr << "usuwam g: " << ((Group*)(*ei))->getGroupType() << std::endl;
            delete ((Group*)(*ei));
//        }
        else if ((*ei)->getType() == "syntok")
            delete ((Syntok*)(*ei));
        else
        {
            std::cerr << "tego nieznanego id: " << (*ei)->getId() << std::endl;
            delete *ei;
        }
        ei ++;
    }

//    delete children;

//    delete type;
//    delete ruleName;
//    delete id;
//    delete compiledGroup;
//    delete groupType;
//    std::cerr << "destrukcja grupy" << std::endl;
}

std::string Group::getType()
{
    return type;
}

void Group::updateCompiled()
{
    compiledGroup = "<<g<" + id + "<" + groupType;// + ">";

    std::string head = ((Token*)this->getHeadToken())->getCompiled();
    compiledGroup += head.substr(3, std::string::npos);
//    head.reset();
//    std::string head = ((Token*)this->getHeadToken())->getCompiled();
//    compiledGroup += head.substr(3, std::string::npos); // opuszczanie <<t;
////    compiledGroup += ((Token*)this->getHeadToken())->getCompiled();
}

void Group::setCompiled(std::string aCompiledGroup)
{
    compiledGroup = aCompiledGroup;
//    delete compiledGroup;
//    compiledGroup = new std::string(aCompiledGroup);
}

std::string Group::getCompiled()
{
    return compiledGroup;
}

void Group::setId(std::string aId)
{
    id = aId;
//    delete id;
//    id = new std::string(aId);
}

std::string Group::getId()
{
    return id;
}

void Group::setGroupType(std::string aGroupType)
{
    groupType = aGroupType;
//    delete groupType;
//    groupType = new std::string(aGroupType);
}

std::string Group::getGroupType()
{
    return groupType;
}

void Group::setHeadIndex(int aHeadIndex)
{
    headIndex = aHeadIndex;
}

int Group::getHeadIndex()
{
    return headIndex;
//    int index = headIndex;
//    if ((children[index])->getType() == "group")
//        index = ((Group*)children[index])->getHeadIndex();
//    else
//        inde = (children[index])->getId();
//    return index;
}

void Group::setRuleName(std::string aRuleName)
{
    ruleName = aRuleName;
//    delete ruleName;
//    ruleName = new std::string(aRuleName);
}

std::string Group::getRuleName()
{
    return ruleName;
}

std::string Group::toXml()
{
    std::stringstream ss;
//    if (children[headIndex]->getType() == "group")
//    else
//        head = children[headIndex]->getId();
    ss << "<group id=\"" << id << "\" rule=\"" << ruleName << "\" head=\"" << getHead() << "\" type=\"" << groupType << "\">" << std::endl;
    Entities::iterator i = children.begin();
    while (i != children.end())
    {
        ss << (*i)->toXml();
        i ++;
    }
    ss << "</group>" << std::endl;
    return ss.str();
}

std::string Group::getHead()
{
    if ((children[headIndex])->getType() == "group")
        return ((Group*)(children[headIndex]))->getHead();
    else
        return (children[headIndex])->getId();
}

Entity* Group::getHeadToken()
{
//    std::cerr << "moj head index: " << headIndex << std::endl;
//    std::cerr << "moje dzieciatka: " << children.size() << std::endl;
//    for (Entities::iterator hu = children.begin(); hu != children.end(); hu ++)
//     {
////         std::cerr << "coś" << std::endl;
//         Entity *ent = *hu;
////         std::cerr << "mam ent" << std::endl;
////         std::cerr << "typ: " << ent->getType() << std::endl;
//         if (ent->getType() == "group")
//         {
////             std::cerr << "grupa" << std::endl;
////             std::cerr << "grupa " << ((Group*)ent)->getGroupType() << "; id :" << ((Group*)ent)->getId() << std::endl;
//         }
//         else if (ent->getType() == "token")
//         {
////             std::cerr << "tokien" << std::endl;
////             std::cerr << "tokien " << ((Token*)ent)->getOrth() << "; id :" << ((Token*)ent)->getId() << std::endl;
//         }
//     }
    //std::cerr << "robie ght, typ: " << children[headIndex]->getType() << std::endl;
    if ((children[headIndex])->getType() == "group")
        return ((Group*)(children[headIndex]))->getHeadToken();
    else
        return children[headIndex];
}

std::string Group::getReadable()
{
    std::stringstream ss;
    std::string orth;
    std::vector<std::string> morphology;
    if (this->getHeadToken()->getType() == "token")
    {
        ss << ((Token*)(this->getHeadToken()))->getReadable();
    }
    else if (this->getHeadToken()->getType() == "syntok")
    {
        ss << ((Syntok*)(this->getHeadToken()))->getReadable();
    }
    return ss.str();
}

}

}

}


