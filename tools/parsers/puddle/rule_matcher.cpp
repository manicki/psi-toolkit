
#include "rule_matcher.hpp"

//#include <boost/regex.hpp>
#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

int RuleMatcher::sentencesCount = 0;
boost::posix_time::ptime RuleMatcher::start_time = boost::posix_time::microsec_clock::local_time();

RuleMatcher::RuleMatcher()
{
//    rules = new Rules;
    syntok = true;
    disamb = false;
    norepeats = false;
//    noxml = true;
}

RuleMatcher::~RuleMatcher()
{
//    delete rules;
}

//void RuleMatcher::setRules(Rules aRules)
void RuleMatcher::setRules(RulesPtr aRules)
{
//    delete rules;
    //rules = //new Rules(aRules);
    rules = aRules;
}

//void RuleMatcher::applyRules(std::string sentence, Entities &entities, Edges &lattice)
//{
//    //trzeba jechać po kolei tokenami ze zdania - zdanie to nie bedzie string, tylko obiekt: co najmniej wektor tokenow (elementow tych od grafu) oraz napis bedacy tym 'skompilowanym' ciagiem
//    //dla pierwszego tokena, dla drugiego, etc, zrob: ...
//    //
//
//    //while (1)
//    //{
//    bool anything = false;
//
//    Rules::iterator ir = rules.begin();
//    bool changed = false;
//    while (ir != rules.end())
//    {
//        //        std::cout << "Zdanie: " << sentence << std::endl;
//        //        std::cout << "Token: " << (*it)->getCompiled() << std::endl;
//        //        std::cout << "Token na tapecie taki: " << it->getOrth() << std::endl;
//        //        std::vector<std::string> morpho = it->getMorphology();
//        //        std::vector<std::string> comp = it->getCompiledInterpretations();
//        //        std::vector<std::string>::iterator m = morpho.begin();
//        //        std::vector<std::string>::iterator c = comp.begin();
//        //        std::cout << "Interpretacje: " << std::endl;
//        //        while (m != morpho.end())
//        //        {
//        //            std::cout << *m << " --> " << *c << std::endl;
//        //            m ++;
//        //            c ++;
//        //        }
//        Entities::iterator it = entities.begin();
//        int currentEntity = 0;
//        while (it != entities.end())
//        {
//             //           std::cout << "Reguly wzor: " << ir->getPattern() << std::endl;
//            if (ir->test(sentence, entities, currentEntity))
//            {
//                if (ir->apply(sentence, entities, lattice, currentEntity))
//                {
//                    std::cout << "Dopasowalem!!!!" << std::endl;
//                    changed = true;
//                }
//            }
//
//            if (changed)
//            {
//                it = entities.begin() + currentEntity;
//                anything = true;
//                break;
//            }
//
//            boost::regex regTok;
//            //        if ((*it)->getType() == "token")
//            //        {
//            //            Token *token = (Token*) *it;
//            regTok = "^" + (*it)->getCompiled();
//            //        }
//            //else
//            //{
//            //    std::cout << "Nie token, tylko taki typ: " << (*it)->getType() << std::endl;
//            //}
//            //sentence = boost::regex_replace(sentence, regTok, "", boost::match_default | boost::format_sed);
//            it ++;
//        }
//
//        if (changed)
//        {
//            changed = false;
//            continue;
//        }
//        currentEntity ++;
//        ir ++;
//    }
//
//    // xmlWriter->setEntities(entities);
//    xmlWriter->writeSentence(entities);
//
//    //if (!anything)
//    //    break;
//    //}
//}

ParseGraphPtr RuleMatcher::applyRules(std::string &sentence, Entities &entities, Edges &lattice)
{
//    std::cerr << "ZDANIE!" << std::endl;
    //trzeba jechać po kolei tokenami ze zdania - zdanie to nie bedzie string, tylko obiekt: co najmniej wektor tokenow (elementow tych od grafu) oraz napis bedacy tym 'skompilowanym' ciagiem
    //dla pierwszego tokena, dla drugiego, etc, zrob: ...
    //

//    std::cerr << sentence << std::endl;

    //Entities::iterator it = entities.begin();
    //int currentEntity = 0;
    //while (1)
    //{
    bool anything = false;

    Rules::iterator ir = rules->begin();

    while (ir != rules->end())
    //for (Rules::iterator ir = rules.begin(); ir != rules.end(); ir ++)
    {
//        std::cerr << "Regula: " << (*ir)->getName() << std::endl;
        int currentMatch = 0;
        int currentEntity;
        bool first_match = true;
        std::string before = "";
        std::string prev_before = "";
        //@todo: przerobic to tak, zeby nie bylo miliard razy dopasowywane ten lancuch, tylko zeby go dopasowac tyle razy ile pasuje i odpowiednio odpalac reguly tam, gdzie pasuja
        while ((currentEntity = (*ir)->matchPattern(sentence, currentMatch, before)) > -1)
        {
            currentMatch ++;

            //OLD:
            //if (norepeats)
            //    if (prev_before == before)
            //        continue;
            if (!first_match)
            {
                if (!(*ir)->getRepeat())
                {
                    if (prev_before == before)
                        continue;
                }
            }
            first_match = false;
                    prev_before = before;
//                std::cerr << "Pasuje!" << std::endl;
//       std::cerr << "Zdanie: " << sentence << std::endl;
        bool structureChanged = false;
        //int currentEntity = 0;
        //for (Entities::iterator it = entities.begin(); it != entities.end(); it ++)
       // {
        std::string oldSentence = sentence;
            if ((*ir)->test(sentence, entities, currentEntity))
            {
//                std::cerr << "test zdany" << std::endl;
                if ((*ir)->apply(sentence, entities, lattice, currentEntity))
                {
//                    prev_before = before;
//                    std::cerr << "zaaplikowany" << std::endl;
                    if (sentence != oldSentence)
                        currentMatch --;
                    structureChanged = true;
              //      break;
                }
            }

        //    currentEntity ++;
        //}

        if (structureChanged)
        {
        //    ir = rules.begin();
        //    currentMatch = 0;
            continue;
        }
        }
        ir ++;
    }

//    if (!noxml)
//        xmlWriter->writeSentence(entities); //TODO:ten xml to tylko w trybie testowym ostatecznie ma byc!

    Edges posEdges;

    for (Edges::iterator e = lattice.begin(); e != lattice.end(); e++)
    {
        if (!(*e)->isLexical())
        //if ((*e)->isPhrasal())
            continue;
            //break;
//        if ((*e)->getHead() != "")
//            break; //zabezpieczenie przed potraktowaniem word jako token?

        if ((*e)->getLabel().find("&rpar;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&rpar;") != std::string::npos)
            {
                int pos = label.find("&rpar;");
                label = label.substr(0, pos) + ")" + label.substr(pos + 6, std::string::npos);
            }
            (*e)->setLabel(label);
        }
        if ((*e)->getLabel().find("&lpar;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&lpar;") != std::string::npos)
            {
                int pos = label.find("&lpar;");
                label = label.substr(0, pos) + "(" + label.substr(pos + 6, std::string::npos);
            }
            (*e)->setLabel(label);
        }
        if ((*e)->getLabel().find("&amp;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&amp;") != std::string::npos)
            {
                int pos = label.find("&amp;");
                label = label.substr(0, pos) + "&" + label.substr(pos + 5, std::string::npos);
            }
            (*e)->setLabel(label);
        }
        if ((*e)->getLabel().find("&lt;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&lt;") != std::string::npos)
            {
                int pos = label.find("&lt;");
                label = label.substr(0, pos) + "<" + label.substr(pos + 4, std::string::npos);
            }
            (*e)->setLabel(label);
        }
        if ((*e)->getLabel().find("&gt;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&gt;") != std::string::npos)
            {
                int pos = label.find("&gt;");
                label = label.substr(0, pos) + ">" + label.substr(pos + 4, std::string::npos);
            }
            (*e)->setLabel(label);
        }
        if ((*e)->getLabel().find("&bar;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&bar;") != std::string::npos)
            {
                int pos = label.find("&bar;");
                label = label.substr(0, pos) + "|" + label.substr(pos + 5, std::string::npos);
            }
            (*e)->setLabel(label);
        }
        if ((*e)->getLabel().find("&plus;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&plus;") != std::string::npos)
            {
                int pos = label.find("&plus;");
                label = label.substr(0, pos) + "+" + label.substr(pos + 6, std::string::npos);
            }
            (*e)->setLabel(label);
        }
        if ((*e)->getLabel().find("&astr;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&astr;") != std::string::npos)
            {
                int pos = label.find("&astr;");
                label = label.substr(0, pos) + "*" + label.substr(pos + 6, std::string::npos);
            }
            (*e)->setLabel(label);
        }
        if ((*e)->getLabel().find("&qmark;") != std::string::npos)
        {
            std::string label = (*e)->getLabel();
            while (label.find("&qmark;") != std::string::npos)
            {
                int pos = label.find("&qmark;");
                label = label.substr(0, pos) + "?" + label.substr(pos + 7, std::string::npos);
            }
            (*e)->setLabel(label);
        }

        for (int vari = 0; vari < (*e)->variants_.size(); vari ++)
        {
            std::string var_base = boost::get<0>((*e)->variants_[vari]);
            if (var_base.find("&rpar;") != std::string::npos)
            {
                while (var_base.find("&rpar;") != std::string::npos)
                {
                    int pos = var_base.find("&rpar;");
                    var_base = var_base.substr(0, pos) + ")" + var_base.substr(pos + 6, std::string::npos);
                }
            }
            if (var_base.find("&lpar;") != std::string::npos)
            {
                while (var_base.find("&lpar;") != std::string::npos)
                {
                    int pos = var_base.find("&lpar;");
                    var_base = var_base.substr(0, pos) + "(" + var_base.substr(pos + 6, std::string::npos);
                }
            }
            if (var_base.find("&amp;") != std::string::npos)
            {
                while (var_base.find("&amp;") != std::string::npos)
                {
                    int pos = var_base.find("&amp;");
                    var_base = var_base.substr(0, pos) + "&" + var_base.substr(pos + 5, std::string::npos);
                }
            }
            if (var_base.find("&lt;") != std::string::npos)
            {
                while (var_base.find("&lt;") != std::string::npos)
                {
                    int pos = var_base.find("&lt;");
                    var_base = var_base.substr(0, pos) + "<" + var_base.substr(pos + 4, std::string::npos);
                }
            }
            if (var_base.find("&gt;") != std::string::npos)
            {
                while (var_base.find("&gt;") != std::string::npos)
                {
                    int pos = var_base.find("&gt;");
                    var_base = var_base.substr(0, pos) + ">" + var_base.substr(pos + 4, std::string::npos);
                }
            }
            if (var_base.find("&bar;") != std::string::npos) // @todo: ustandaryzowac te nomenklaturre
            {
                while (var_base.find("&bar;") != std::string::npos)
                {
                    int pos = var_base.find("&bar;");
                    var_base = var_base.substr(0, pos) + "|" + var_base.substr(pos + 5, std::string::npos);
                }
            }
            if (var_base.find("&plus;") != std::string::npos)
            {
                while (var_base.find("&plus;") != std::string::npos)
                {
                    int pos = var_base.find("&plus;");
                    var_base = var_base.substr(0, pos) + "+" + var_base.substr(pos + 6, std::string::npos);
                }
            }
            if (var_base.find("&astr;") != std::string::npos)
            {
                while (var_base.find("&astr;") != std::string::npos)
                {
                    int pos = var_base.find("&astr;");
                    var_base = var_base.substr(0, pos) + "*" + var_base.substr(pos + 6, std::string::npos);
                }
            }
            if (var_base.find("&qmark;") != std::string::npos)
            {
                while (var_base.find("&qmark;") != std::string::npos)
                {
                    int pos = var_base.find("&qmark;");
                    var_base = var_base.substr(0, pos) + "?" + var_base.substr(pos + 6, std::string::npos);
                }
            }

            if (var_base != boost::get<0>((*e)->variants_[vari]))
            {
                PosInfo new_var = PosInfo(var_base, boost::get<1>((*e)->variants_[vari]), boost::get<2>((*e)->variants_[vari]));
                (*e)->variants_[vari] = new_var;
            }
        }
//        if ((*e)->getLabel().find("\"") != std::string::npos)
//        {
//            int b = 0;
//            std::string label = (*e)->getLabel();
//            while (label.find("\"", b) != std::string::npos)
//            {
//                int pos = label.find("\"", b);
//                label = label.substr(0, pos) + "\"" + label.substr(pos + 1, std::string::npos);
//                b = pos + 2;
//            }
//            (*e)->setLabel(label);
//        }
//        if (!syntok)
//        {
//            std::cerr << "jestem tu" << std::endl;
//            if (end != (start + 1))
//                continue; // to tu zabezpieczenie przed potraktowaniem word jako token?
//        }

        bool lowFlag = false;
        std::string id = (*e)->getId();
        int start = (*e)->getStart();
        int end = (*e)->getEnd();

        std::string lowcase = (*e)->getLabel();
        boost::to_lower(lowcase);
//        UnicodeString valL = icuConverter::fromUtf((*e)->getLabel());
//        valL.toLower();
//        StringCharacterIterator itL(valL);
//        std::stringstream ss;
//        while (itL.hasNext())
//        {
//            UnicodeString tl = itL.current();
//            ss << icuConverter::toUtf(tl);
//            itL.next();
//        }
//        lowcase = ss.str();
        if (lowcase != (*e)->getLabel())
            lowFlag = true;

        if (lowFlag)
        {
            for (int vari = 0; vari < (*e)->variants_.size(); vari ++)
            {
                std::string var_base = boost::get<0>((*e)->variants_[vari]);

                std::string lowbase = var_base;
                boost::to_lower(lowbase);
//                UnicodeString valL = icuConverter::fromUtf(var_base);
//                valL.toLower();
//                StringCharacterIterator itL(valL);
//                std::stringstream ss;
//                while (itL.hasNext())
//                {
//                    UnicodeString tl = itL.current();
//                    ss << icuConverter::toUtf(tl);
//                    itL.next();
//                }
//                lowbase = ss.str();
                if (lowbase == var_base) //istnieje forma bazowa pisana mala litera
                {
                    (*e)->setLabel(lowcase);
                    break;
                }
            }
        }

        std::set<std::string> parts;

        std::vector<PosInfo> vars = (*e)->variants_;
        for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
        {
            if (!boost::get<2>(*pi))
                continue;
            std::string morpho = boost::get<1>(*pi);
            std::string part = morpho.substr(0, morpho.find(":", 1));
            if (parts.find(part) == parts.end())
            {
                TransitionInfo *pos = new TransitionInfo("pos");
                pos->setLabel(part);
                pos->setDepth(1);
                pos->setStart(start);
                pos->setEnd(end);
                pos->variants_ = (*e)->variants_;
                pos->setHead((*e)->getId());

                std::stringstream *ss = new std::stringstream;
                *ss << std::hex << Group::groupId;
                std::string pos_id = ss->str();
                delete ss;
                pos->setId(pos_id);
                Group::groupId++;


//                std::vector<int> dsc = (*e)->desc;
//                pos->setDesc(dsc);

                //pos->variants_ = vars;
                posEdges.push_back(pos);
         //       lattice.push_back(pos);
                parts.insert(part);
                if (!disamb)
                    break;
            }
        }
        if (parts.size() == 0) //gdy nie bylo zadnych interpretacji, bo wyrznely wszystko reguly
        {
            if ((*e)->variants_.size() == 0)
            {
                (*e)->addMorphology(PosInfo((*e)->getLabel(), "ign", 1));
            }
            PosInfo pi = (*e)->variants_.front();
            std::string base = boost::get<0>(pi);
            (*e)->addMorphology(PosInfo(base, "ign", 1));
            TransitionInfo *pos = new TransitionInfo("pos");
            pos->setLabel("ign");
            pos->setDepth(1);
            pos->setStart((*e)->getStart());
            pos->setEnd((*e)->getEnd());
            pos->setHead((*e)->getId());
            pos->variants_ = (*e)->variants_;

//                std::vector<int> dsc = (*e)->desc;
//                pos->setDesc(dsc);

            posEdges.push_back(pos);
        }
    }

    for (Edges::iterator ei = posEdges.begin(); ei != posEdges.end(); ei++)
        lattice.push_back(*ei);

    //ParseGraph *g = new ParseGraph();
    ParseGraphPtr pg = ParseGraphPtr(new ParseGraph());

    if (!syntok)
    {
        std::map<int, int> begins_mapped;
        for (Edges::iterator e = lattice.begin(); e != lattice.end(); e ++)
        {
                if ((*e)->getType() == "group" && (*e)->getLabel() == "SYNTOK")
                {
                    std::map<int, int>::iterator it = begins_mapped.find((*e)->getStart());
                    if (it != begins_mapped.end())
                    {
                        begins_mapped[(*e)->getStart()] = it->second + 1;
                    }
                    else
                    {
                        begins_mapped.insert(std::pair<int, int>((*e)->getStart(), 1));
                    }
                    //if (((*e)->getEnd() - (*e)->getStart()) > 1)
                    //{
                    //e ++;
                    //}
                }
        }

        for (Edges::iterator e = lattice.begin(); e != lattice.end(); e ++)
        {
            if ((*e)->getType() == "group" && (*e)->getLabel() == "SYNTOK")
                continue;
            std::map<int, int>::iterator it = begins_mapped.find((*e)->getStart());
            if (it != begins_mapped.end())
            {
                (*e)->setDepth((*e)->getDepth() - it->second);
            }

            pg->add_edge((*e)->getStart(), (*e)->getEnd(), **e);
        }
    }
    else
    {
        for (Edges::iterator e = lattice.begin(); e != lattice.end(); e ++)
            pg->add_edge((*e)->getStart(), (*e)->getEnd(), **e);
    }
//    e ++;

    sentencesCount ++;
    if ((sentencesCount % 1000) == 0)
    {
        boost::posix_time::ptime end_time = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration delta = end_time - start_time;
        std::cerr << sentencesCount << " sentences parsed (" << delta.total_milliseconds() << " ms)" << std::endl;
        start_time = boost::posix_time::microsec_clock::local_time();
    }

//    while (it != entities.end())
//    {
////        std::cout << "Zdanie: " << sentence << std::endl;
////        std::cout << "Token: " << (*it)->getCompiled() << std::endl;
////        std::cout << "Token na tapecie taki: " << it->getOrth() << std::endl;
////        std::vector<std::string> morpho = it->getMorphology();
////        std::vector<std::string> comp = it->getCompiledInterpretations();
////        std::vector<std::string>::iterator m = morpho.begin();
////        std::vector<std::string>::iterator c = comp.begin();
////        std::cout << "Interpretacje: " << std::endl;
////        while (m != morpho.end())
////        {
////            std::cout << *m << " --> " << *c << std::endl;
////            m ++;
////            c ++;
////        }
//        Rules::iterator ir = rules.begin();
//        bool changed = false;
//        while (ir != rules.end())
//        {
////            std::cout << "Reguly wzor: " << ir->getPattern() << std::endl;
//            if (ir->test(sentence, entities, currentEntity))
//            {
//                if (ir->apply(sentence, entities, lattice, currentEntity))
//                    changed = true;
//            }
//            ir ++;
//        }
//
//        if (changed)
//        {
//            it = entities.begin() + currentEntity;
//            anything = true;
//            continue;
//        }
//
//        boost::regex regTok;
////        if ((*it)->getType() == "token")
////        {
////            Token *token = (Token*) *it;
//            regTok = "^" + (*it)->getCompiled();
////        }
//        //else
//        //{
//        //    std::cout << "Nie token, tylko taki typ: " << (*it)->getType() << std::endl;
//        //}
//        sentence = boost::regex_replace(sentence, regTok, "", boost::match_default | boost::format_sed);
//        it ++;
//        currentEntity ++;
//    }

   // xmlWriter->setEntities(entities);

    return pg;
    //if (!anything)
    //    break;
    //}
}

void RuleMatcher::setSyntok()
{
    syntok = true;
}

void RuleMatcher::setNoSyntok()
{
    syntok = false;
}

void RuleMatcher::setDisamb()
{
    disamb = true;
}

void RuleMatcher::setNoRepeats()
{
    norepeats = true;
}

}

}

}
