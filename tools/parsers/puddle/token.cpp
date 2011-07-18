
#include "token.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

Token::Token()
{
    type = "token";
    compiledToken = "";
    orth = "";
    id = "";

//    morphology = new std::vector<StringPtr>;
//    compiledInterpretations = new std::vector<StringPtr>;
}

Token::~Token()
{
//    if (morphology->size() > 0)
//    {
//        std::vector<StringPtr>::iterator ii = morphology->begin();
//        while (ii != morphology->end())
//        {
//            ii = morphology->erase(ii);
//        }
//    }
    morphology.clear();
//    delete morphology;
//    if (compiledInterpretations->size() > 0)
//    {
//        std::vector<StringPtr>::iterator ii = compiledInterpretations->begin();
//        while (ii != compiledInterpretations->end())
//        {
//            ii = compiledInterpretations->erase(ii);
//        }
//    }
    compiledInterpretations.clear();
//    delete compiledInterpretations;

//    delete type;
//    delete compiledToken;
//    delete orth;
//    delete id;
//    std::cerr << "destrukcja tokenu" << std::endl;
}

void Token::setCompiled(std::string aCompiledToken)
{
    compiledToken = aCompiledToken;
}

//void Token::addMorphology(std::string aMorphology)
//{
//    morphology.push_back(aMorphology);
//}

bool Token::addInterpretation(std::string aMorphology, std::string aCompiledInterpretation)
{
    morphology.push_back(aMorphology);
    compiledInterpretations.push_back(aCompiledInterpretation);
    return true;
}

bool Token::deleteInterpretation(boost::u32regex condition, TransitionInfo* &tok)
{
    bool ret = false;
    //boost::regex regCondition(condition);
    std::vector<std::string>::iterator i = compiledInterpretations.begin();
    std::vector<std::string>::iterator m = morphology.begin();
    while (i != compiledInterpretations.end())
    {
//        std::cerr << "kompiled interpretacja: " << **i << std::endl;
        std::string compare = this->orth + "<" + *i;
        if (boost::u32regex_match(compare, condition))
        {
            ret = true;
//            std::cout << "Pasuje, wiec usuwam: " << compare << std::endl;
            i = compiledInterpretations.erase(i);
//            std::cerr << "usunalem" << std::endl;
            for (std::vector<PosInfo>::iterator p = tok->variants_.begin(); p != tok->variants_.end(); p ++)
            {
                if (boost::get<2>(*p) == 0)
                    continue;
//                std::cerr << "przed morfo" << std::endl;
                std::string morpho = boost::get<0>(*p) + ":" + boost::get<1>(*p);
//                std::cerr << "morpho: " << *morpho << std::endl;
//                std::cerr << "m: " << **m << std::endl;
                if (morpho == *m)
                {
                    boost::get<2>(*p) = 0;
//                    std::cerr << "Usuwam: " << *morpho << std::endl;
//                    delete morpho;
                    break;
                }
//                std::cerr << "przed dilit" << std::endl;
//                delete morpho;
//                std::cerr << "po dilit" << std::endl;
            }
//            std::cerr << "usuwam interpretacje: " << *m << std::endl;
            m = morphology.erase(m);
            continue;
        }
//        compare.reset();
        i ++;
        m ++;
    }
    return ret;
}

std::string Token::getCompiled()
{
    return compiledToken;
}

std::vector<std::string> Token::getMorphology()
{
    return morphology;
}

void Token::clearMorphology()
{
    morphology.clear();
//    if (morphology.size() > 0)
//    {
//        std::vector<StringPtr>::iterator ii = morphology->begin();
//        while (ii != morphology->end())
////        for (std::vector<std::string*>::iterator ii = morphology->begin(); ii != morphology->end(); ii++)
//        {
//            ii = morphology->erase(ii);
////            delete *ii;
//        }
//    }
//    morphology->clear();
}

void Token::clearCompiledInterpretations()
{
    compiledInterpretations.clear();
//    if (compiledInterpretations->size() > 0)
//    {
//        std::vector<StringPtr>::iterator ii = compiledInterpretations->begin();
//        while (ii != compiledInterpretations->end())
////        for (std::vector<std::string*>::iterator ii = compiledInterpretations->begin(); ii != compiledInterpretations->end(); ii++)
//        {
//            ii = compiledInterpretations->erase(ii);
////            delete *ii;
//        }
//    }
//    compiledInterpretations->clear();
}

std::vector<std::string> Token::getCompiledInterpretations()
{
    return compiledInterpretations;
}

std::string Token::getOrth()
{
    return orth;
}

void Token::setOrth(std::string aOrth)
{
    orth = aOrth;
//    delete orth;
//    orth = new std::string(aOrth);
}


void Token::updateCompiled()
{
    std::string s = "";
    std::vector<std::string>::iterator i = compiledInterpretations.begin();
    while (i != compiledInterpretations.end())
    {
        s += "<" + *i;
        i ++;
    }
    compiledToken = "<<t<" + id + "<" + orth + s + ">";
//    s.reset();
    //delete s;
}

void Token::setId(std::string aId)
{
    id = aId;
//    delete id;
//    id = new std::string(aId);
}

void Token::setId(int aId)
{
    id = boost::lexical_cast<std::string>(aId);
//    delete id;
//    id = new std::string(boost::lexical_cast<std::string>(aId));
}

std::string Token::getId()
{
    return id;
}

std::string Token::toXml()
{
    boost::u32regex regLpar = boost::make_u32regex("&lpar;");
    boost::u32regex regRpar = boost::make_u32regex("&rpar;");
    std::stringstream ss;
    ss << "<tok id=\"" << id << "\">" << std::endl;
    std::string porth = boost::u32regex_replace(orth, regLpar, "(", boost::match_default | boost::format_sed);
    porth = boost::u32regex_replace(porth, regRpar, ")", boost::match_default | boost::format_sed);
    ss << "<orth>" << porth << "</orth>" << std::endl;
//    porth.reset();
    std::vector<std::string>::iterator m = morphology.begin();
    while (m != morphology.end())
    {
        std::string morpho = *m;
        int pos = morpho.find(":", 1);  //TODO: to trzeba zrobić sprytniej, bo token może być dwukropkiem na przykład!
        std::string base = morpho.substr(0, pos);
        base = boost::u32regex_replace(base, regLpar, "(", boost::match_default | boost::format_sed);
        base = boost::u32regex_replace(base, regRpar, ")", boost::match_default | boost::format_sed);
        morpho = morpho.substr(pos + 1, std::string::npos);
        ss << "<lex><base>" << base << "</base><ctag>" << morpho << "</ctag></lex>" << std::endl;
        m ++;
//        morpho.reset();
//        base.reset();
        //delete morpho;
        //delete base;
    }
    if (morphology.size() == 0)
    {
        ss << "<lex><base>" << orth << "</base><ctag>ign</ctag></lex>" << std::endl;
    }
    //delete porth;
    ss << "</tok>" << std::endl;
    return ss.str();
}

std::string Token::getReadable()
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

