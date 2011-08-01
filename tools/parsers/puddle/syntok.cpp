
#include "syntok.hpp"
#include <boost/lexical_cast.hpp>

#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

//int Group::groupId;

Syntok::Syntok()
{
    type = "syntok";
    std::stringstream ss;
    ss << std::hex << Group::groupId;
    id = ss.str();
//    delete ss;

    Group::groupId++;
    compiledSyntok = "";
    ruleName = "";
    orth = "";

//    morphology = new std::vector<StringPtr>;
//    compiledInterpretations = new std::vector<StringPtr>;
//    contents = new Entities;
}

Syntok::Syntok(Entities &entities, int startIndex, int endIndex, std::string aRuleName)
{
    type = "syntok";
    std::stringstream ss;
    ss << std::hex << Group::groupId;
    id = ss.str();
//    delete ss;

    Group::groupId++;

//    morphology = new std::vector<StringPtr>;
//    compiledInterpretations = new std::vector<StringPtr>;
//    contents = new Entities;

    orth = "";

    int i = startIndex;
    Entities::iterator it = entities.begin() + i;
    while (i <= endIndex)
    {
        Entity *entity = *it;
        if (entity->getType() == "token")
        {
            orth += " " + ((Token*)entity)->getOrth();
        }
        else if (entity->getType() == "group")
        {
        }
        else if (entity->getType() == "syntok")
        {
            orth += " " + ((Syntok*)entity)->getOrth();
        }
        else
        {
            //TODO
        }
        //Entity *copy = entity;
        this->contents.push_back(entity);
        entities.erase(it);
        //this->contents.push_back(copy);
        //std::cout << "Do kontentow wrzucam gowno o id: " << copy->getId() << std::endl;
        i ++;
    }
    orth = orth.substr(1, std::string::npos);
    ruleName = aRuleName;
    compiledSyntok = ""; //bo zaraz po stworzeniu dorzucane sa interpretacje dopiero, a po nich updateCompiled
}

Syntok::~Syntok()
{
    Entities::iterator ei = contents.begin();
//    std::cerr << "W slowie:" << std::endl;
    while (ei != contents.end())
    {
        if ((*ei)->getType() == "token")
            delete ((Token*)(*ei));
        else if ((*ei)->getType() == "group")
            delete ((Group*)(*ei));
        else if ((*ei)->getType() == "syntok")
            delete ((Syntok*)(*ei));
        else
        {
//            std::cerr << "tego nieznanego id: " << *((*ei)->getId()) << std::endl;
            delete *ei;
        }
        ei ++;
    }

    //delete contents;

    //delete morphology;
    //delete compiledInterpretations;

    //delete type;
    //delete id;
    //delete orth;
    //delete ruleName;
    //delete compiledSyntok;
//    std::cerr << "destrukcja slowa" << std::endl;
}

void Syntok::setCompiled(std::string aCompiled)
{
    compiledSyntok = aCompiled;
//    delete compiledSyntok;
//    compiledSyntok = new std::string(aCompiled);
}

std::string Syntok::getCompiled()
{
    return compiledSyntok;
}

bool Syntok::addInterpretation(std::string aMorphology, std::string aCompiledInterpretation)
{
     morphology.push_back(aMorphology);
     compiledInterpretations.push_back(aCompiledInterpretation);
     return true;
}

bool Syntok::deleteInterpretation(PatternPtr condition, TransitionInfo* &wd)
{
    bool ret = false;
    std::vector<std::string>::iterator i = compiledInterpretations.begin();
    std::vector<std::string>::iterator m = morphology.begin();
    while (i != compiledInterpretations.end())
    {
        std::string compare = this->orth + "<" + *i;
        //if (boost::u32regex_match(compare, condition))
        if (RE2::FullMatch(compare, *condition)) {
            ret = true;
            i = compiledInterpretations.erase(i);
            for (std::vector<PosInfo>::iterator p = wd->variants_.begin(); p != wd->variants_.end(); p ++)
            {
                if (boost::get<2>(*p) == 0)
                    continue;
                std::string morpho = boost::get<0>(*p) + ":" + boost::get<1>(*p);
                if (morpho == *m)
                {
                    boost::get<2>(*p) = 0;
                    //delete morpho;
                    break;
                }
                //delete morpho;
            }
            m = morphology.erase(m);
           // delete compare;
            continue;
        }
        //delete compare;
        i ++;
        m ++;
    }
    return ret;
}
//bool Syntok::deleteInterpretation

std::vector<std::string> Syntok::getMorphology()
{
    return morphology;
}

std::vector<std::string> Syntok::getCompiledInterpretations()
{
    return compiledInterpretations;
}

std::string Syntok::getOrth()
{
    return orth;
}

void Syntok::setOrth(std::string aOrth)
{
    orth = aOrth;
//    delete orth;
//    orth = new std::string(aOrth);
}

std::string Syntok::getId()
{
    return id;
}

void Syntok::setId(std::string aId)
{
    id = aId;
//    delete id;
//    id = new std::string(aId);
}

void Syntok::setId(int aId)
{
    id = boost::lexical_cast<std::string>(aId);
//    delete id;
//    id = new std::string(boost::lexical_cast<std::string>(aId));
}

void Syntok::clearMorphology()
{
    morphology.clear();
}

void Syntok::clearCompiledInterpretations()
{
    compiledInterpretations.clear();
}

void Syntok::updateCompiled()
{
    std::string s = "";
    std::vector<std::string>::iterator i = compiledInterpretations.begin();
    while (i != compiledInterpretations.end())
    {
        s += "<" + *i;
        i ++;
    }
    compiledSyntok = "<<t<" + id + "<" + orth + s + ">";
    //delete s;
}

std::string Syntok::getStart()
{
    if (contents.front()->getType() == "token")
        return ((Token*)contents.front())->getId();
    else if (contents.front()->getType() == "syntok")
        return ((Syntok*)contents.front())->getStart();
    else
    {
        //TODO: co jak grupa?
    }
}

std::string Syntok::getEnd()
{
    if (contents.at(contents.size() - 1)->getType() == "token")
        return ((Token*)contents.at(contents.size() - 1))->getId();
    else if (contents.at(contents.size() - 1)->getType() == "syntok")
        return ((Syntok*)contents.at(contents.size() - 1))->getStart();
    else
    {
        //TODO: co jak grupa?
    }
}

std::string Syntok::toXml()
{
    //boost::u32regex regLpar = boost::make_u32regex("&lpar;");
    //boost::u32regex regRpar = boost::make_u32regex("&rpar;");
    RE2 regLpar("&lpar;");
    RE2 regRpar("&rpar;");
    std::stringstream ss;
    ss << "<syntok id=\"" << id << "\" rule=\"" << ruleName << "\">" << std::endl;
    std::string porth = orth;
    RE2::GlobalReplace(&porth, regLpar, "(");
    RE2::GlobalReplace(&porth, regRpar, ")");
    ss << "<orth>" + porth + "</orth>" << std::endl;
//    porth.reset();
    std::vector<std::string>::iterator m = morphology.begin();
    while (m != morphology.end())
    {
        std::string morpho = *m;
        int pos = morpho.find(":", 1);  //TODO: to trzeba zrobić sprytniej, bo token może być dwukropkiem na przykład!
        std::string base = morpho.substr(0, pos);
        RE2::GlobalReplace(&base, regLpar, "(");
        RE2::GlobalReplace(&base, regRpar, ")");
        morpho = morpho.substr(pos + 1, std::string::npos);
        ss << "<lex><base>" << base << "</base><ctag>" << morpho << "</ctag></lex>" << std::endl;
        m ++;
        //morpho.reset();
        //base.reset();
    }
    if (morphology.size() == 0)
    {
        ss << "<lex><base>" << orth << "</base><ctag>ign</ctag></lex>" << std::endl;
    }
    Entities::iterator e = contents.begin();
    while (e != contents.end())
    {
        if ((*e)->getType() == "token")
            ss << ((Token*)*e)->toXml();
        else if ((*e)->getType() == "group")
            ss << ((Group*)*e)->toXml();
        else if ((*e)->getType() == "syntok")
            ss << ((Group*)*e)->toXml();
        else
        {
            //TODO
        }
        e ++;
    }
    ss << "</syntok>" << std::endl;
    return ss.str();
}

std::string Syntok::getReadable()
{
    std::stringstream ss;
    ss << this->getOrth();
    ss << "|||";
    if (this->morphology.size() > 0)
    {
        bool first = true;
        for (std::vector<std::string>::iterator mit = morphology.begin(); mit != morphology.end(); mit ++)
        {
            if (! first)
                ss << "|";
            std::string morpho_string = *mit;
            int pos = morpho_string.find(":", 1);
            ss << morpho_string.substr(0, pos) + "#" + morpho_string.substr(pos + 1, std::string::npos);
            first = false;
        }
    }
    else
    {
        ss << this->getOrth() << "#" << "ign";
    }
    return ss.str();
}

}

}

}
