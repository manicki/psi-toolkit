
#include "unify_action.hpp"
#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

UnifyAction::UnifyAction(std::vector<int> aAttributeIndexes, std::vector<int> aTokenIndexes, std::vector<std::string> uAttributes)
{
    nullAgreement = true;

    baseMask = "";
    maskLength = 0;

    type = "unify";
    verbose = false;

    attributeIndexes.insert(attributeIndexes.begin(), aAttributeIndexes.begin(), aAttributeIndexes.end());// = new std::vector<int>(aAttributeIndexes);
    tokenIndexes.insert(tokenIndexes.begin(), aTokenIndexes.begin(), aTokenIndexes.end());// = new std::vector<int>(aTokenIndexes);

    //commonInterpretations = new std::set<boost::regex>;

    attributes_.insert(attributes_.begin(), uAttributes.begin(), uAttributes.end()); // = new std::vector<std::string>(uAttributes);
}

UnifyAction::~UnifyAction()
{
    //delete attributeIndexes;
    //delete tokenIndexes;

    //delete commonInterpretations;

    //elete attributes_;
}

bool UnifyAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
{
//    std::cout << "rozpoczynam aplikacje" << std::endl;
    if (commonInterpretations.size() == 0)
    {
//        std::cout << "Tutaj pusto" << std::endl;
        return true;
    }
//    else
//    {
//        std::cout << "Rozmiar komon interpretacji: " << commonInterpretations->size() << std::endl;
//        std::cout << "sa to: " << std::endl;
//        for (std::set<boost::regex>::iterator iii = commonInterpretations->begin(); iii != commonInterpretations->end(); iii ++)
//        {
//            std::cout << (*iii).str() << std::endl;
//        }
//    }

    bool sthToDelete = false;
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
            Entity *entity = entities[currentEntity + index];
            if (entity->getType() == "token")
            {
                Token *token = (Token*) entity;
                if (token->getCompiledInterpretations().size() != commonInterpretations.size())
                {
                    sthToDelete = true;
                    break;
                }

            }
            else if ((entity->getType()) == "group")
            {
                Group *group = (Group*) entity;
                while (group->children.at(group->getHeadIndex())->getType() == "group")
                {
                    group = (Group*) (group->children.at(group->getHeadIndex()));
                }

                if (group->children.at(group->getHeadIndex())->getType() == "token")
                {
                    Token *token = (Token*) (group->children.at(group->getHeadIndex()));
                    if (token->getCompiledInterpretations().size() != commonInterpretations.size())
                    {
                        sthToDelete = true;
                        break;
                    }
                }
                else
                {
                    Syntok *syntok = (Syntok*) (group->children.at(group->getHeadIndex()));
                    if (syntok->getCompiledInterpretations().size() != commonInterpretations.size())
                    {
                        sthToDelete = true;
                        break;
                    }
                }

            }
            else if (entity->getType() == "syntok")
            {
                Syntok *syntok = (Syntok*)entity;
                if (syntok->getCompiledInterpretations().size() != commonInterpretations.size())
                {
                    sthToDelete = true;
                    break;
                }
            }
        }
        if (sthToDelete)
            break;
        ie ++;
    }
    if (!sthToDelete)
    {
//        std::cout << "koncze aplikacje, bo nie ma co usunac" << std::endl;
        return false;
    }

    bool ret = false;
    ie = tokenIndexes.begin();
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
        //Entity *entity = entities[currentEntity + *ie - 1];
        Entity *entity = entities[currentEntity + index];
        if (entity->getType() == "token")
        {
            Token *token = (Token*)entity;
            StringPtr id = StringPtr(new std::string(token->getId()));
            Edges::iterator e;
            for (e = edges.begin(); e != edges.end(); e ++)
            {
                if ((*e)->getId() == *id)
                    break;
            }

            std::vector<std::string> compiled = token->getCompiledInterpretations();
            std::vector<std::string> morpho = token->getMorphology();
            token->clearMorphology();
            token->clearCompiledInterpretations();
            for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
                boost::get<2>(*pi) = 0;

            //std::set<boost::u32regex>::iterator c = commonInterpretations.begin();
            std::set<boost::regex>::iterator c = commonInterpretations.begin();
            while (c != commonInterpretations.end())
            {
                std::vector<std::string>::iterator ic = compiled.begin();
                std::vector<std::string>::iterator im = morpho.begin();
                while (ic != compiled.end())
                {

                    //if (boost::u32regex_match(*ic, *c))
                    boost::regex *r = new boost::regex(*c);
                    if (boost::regex_match(*ic, *r))
                    {
                        token->addInterpretation(*im, *ic);
                        for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
                        {
                            std::string morpho = boost::get<0>(*pi) + ":" + boost::get<1>(*pi);
                            if (morpho == *im)
                            {
                                boost::get<2>(*pi) = 1;
                                break;
                            }
                        }
                        ret = true;
                        //token->deleteInterpretation(boost::regex(*ic));
                        //ic = compiled.erase(ic);
                        //im = morpho.erase(im);
                        //continue;
                        delete r;
                        ic = compiled.erase(ic);
                        im = morpho.erase(im);
                        continue;
                    }
                    delete r;

                    ic ++;
                    im ++;
                }
                c ++;
            }

            //entities[currentEntity + *ie - 1] = token;
            token->updateCompiled();
            entities[currentEntity + index] = token;
        }
        else if (entity->getType() == "group")
        {
//            std::cout << "ELO" << std::endl;
            Group *group = (Group*) entity;
            while (group->children.at(group->getHeadIndex())->getType() == "group")
            {
                group = (Group*) (group->children.at(group->getHeadIndex()));
            }

            if (group->children.at(group->getHeadIndex())->getType() == "token")
            {
                Token *token = (Token*) (group->children.at(group->getHeadIndex()));
                StringPtr id = StringPtr(new std::string(token->getId()));
                Edges::iterator e;
                for (e = edges.begin(); e != edges.end(); e ++)
                {
                    if ((*e)->getId() == *id)
                        break;
                }

                std::vector<std::string> compiled = token->getCompiledInterpretations();
                std::vector<std::string> morpho = token->getMorphology();
                token->clearMorphology();
                token->clearCompiledInterpretations();
                for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
                    boost::get<2>(*pi) = 0;

                //std::set<boost::u32regex>::iterator c = commonInterpretations.begin();
                std::set<boost::regex>::iterator c = commonInterpretations.begin();
                while (c != commonInterpretations.end())
                {
                    std::vector<std::string>::iterator ic = compiled.begin();
                    std::vector<std::string>::iterator im = morpho.begin();
                    while (ic != compiled.end())
                    {
                        //if (boost::u32regex_match(*ic, *c))
                        boost::regex *r = new boost::regex(*c);
                        if (boost::regex_match(*ic, *r))
                        {
                            token->addInterpretation(*im, *ic);
                            for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
                            {
                                std::string morpho = boost::get<0>(*pi) + ":" + boost::get<1>(*pi);
                                if (morpho == *im)
                                {
                                    boost::get<2>(*pi) = 1;
                                    break;
                                }
                            }

                            ret = true;
                            //token->deleteInterpretation(boost::regex(*ic));
                            //ic = compiled.erase(ic);
                            //im = morpho.erase(im);
                            //continue;
                            delete r;
                            ic = compiled.erase(ic);
                            im = morpho.erase(im);
                            continue;
                        }
                        delete r;
                        ic ++;
                        im ++;
                    }
                    c ++;
                }

                std::vector<PosInfo> var = (*e)->variants_;
                for (e = edges.begin(); e != edges.end(); e ++)
                {
                    if (((*e)->isPhrasal()) && ((*e)->getHead() == *id))
                    {
                        (*e)->variants_ = var;
                    }
                }

                group->children.at(group->getHeadIndex()) = token;
            }
            else
            {
                Syntok *syntok = (Syntok*) (group->children.at(group->getHeadIndex()));
                StringPtr id = StringPtr(new std::string(syntok->getId()));
                Edges::iterator e;
                for (e = edges.begin(); e != edges.end(); e ++)
                {
                    if ((*e)->getId() == *id)
                        break;
                }

                std::vector<std::string> compiled = syntok->getCompiledInterpretations();
                std::vector<std::string> morpho = syntok->getMorphology();
                syntok->clearMorphology();
                syntok->clearCompiledInterpretations();
                for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
                    boost::get<2>(*pi) = 0;

                //std::set<boost::u32regex>::iterator c = commonInterpretations.begin();
                std::set<boost::regex>::iterator c = commonInterpretations.begin();
                while (c != commonInterpretations.end())
                {
                    std::vector<std::string>::iterator ic = compiled.begin();
                    std::vector<std::string>::iterator im = morpho.begin();
                    while (ic != compiled.end())
                    {
                        //if (boost::u32regex_match(*ic, *c))
                        boost::regex *r = new boost::regex(*c);
                        if (boost::regex_match(*ic, *r))
                        {
                            syntok->addInterpretation(*im, *ic);
                            for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
                            {
                                std::string morpho = boost::get<0>(*pi) + ":" + boost::get<1>(*pi);
                                if (morpho == *im)
                                {
                                    boost::get<2>(*pi) = 1;
                                    break;
                                }
                            }

                            ret = true;
                            //token->deleteInterpretation(boost::regex(*ic));
                            //ic = compiled.erase(ic);
                            //im = morpho.erase(im);
                            //continue;
                            delete r;
                            ic = compiled.erase(ic);
                            im = morpho.erase(im);
                            continue;
                        }
                        delete r;
                        ic ++;
                        im ++;
                    }
                    c ++;
                }

                std::vector<PosInfo> var = (*e)->variants_;
                for (e = edges.begin(); e != edges.end(); e ++)
                {
                    if (((*e)->isPhrasal()) && ((*e)->getHead() == *id))
                    {
                        (*e)->variants_ = var;
                    }
                }

                group->children.at(group->getHeadIndex()) = syntok;

            }
            //            if (!group->getUnify())
            //            {
//                std::cout << "USTAWIAM!!!!!!!!!!!!!!!!!!!1111111111111111" << std::endl;
//                group->setUnify();
//                group->setUnifyAttributes(attributeIndexes);
//                group->setUnifyTokens(tokenIndexes);
//            }

        }
        else if (entity->getType() == "syntok")
        {
            Syntok *syntok = (Syntok*)entity;
            StringPtr id = StringPtr(new std::string(syntok->getId()));
            Edges::iterator e;
            for (e = edges.begin(); e != edges.end(); e ++)
            {
                if ((*e)->getId() == *id)
                    break;
            }

            std::vector<std::string> compiled = syntok->getCompiledInterpretations();
            std::vector<std::string> morpho = syntok->getMorphology();
            syntok->clearMorphology();
            syntok->clearCompiledInterpretations();
            for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
                boost::get<2>(*pi) = 0;

            //std::set<boost::u32regex>::iterator c = commonInterpretations.begin();
            std::set<boost::regex>::iterator c = commonInterpretations.begin();
            while (c != commonInterpretations.end())
            {
                std::vector<std::string>::iterator ic = compiled.begin();
                std::vector<std::string>::iterator im = morpho.begin();
                while (ic != compiled.end())
                {
                    //if (boost::u32regex_match(*ic, *c))
                    boost::regex *r = new boost::regex(*c);
                    if (boost::regex_match(*ic, *r))
                    {
                        syntok->addInterpretation(*im, *ic);
                        for (std::vector<PosInfo>::iterator pi = (*e)->variants_.begin(); pi != (*e)->variants_.end(); pi ++)
                        {
                            std::string morpho = boost::get<0>(*pi) + ":" + boost::get<1>(*pi);
                            if (morpho == *im)
                            {
                                boost::get<2>(*pi) = 1;
                                break;
                            }
                        }
                        ret = true;
                        //token->deleteInterpretation(boost::regex(*ic));
                        //ic = compiled.erase(ic);
                        //im = morpho.erase(im);
                        //continue;
                        delete r;
                        ic = compiled.erase(ic);
                        im = morpho.erase(im);
                        continue;
                    }
                    delete r;
                    ic ++;
                    im ++;
                }
                c ++;
            }
            //entities[currentEntity + *ie - 1] = token;
            entities[currentEntity + index] = syntok;
 }

        }

        ie ++;
    }

//    std::cout << "koncze aplikacje, z wartoscia: " << std::endl;
//    if (ret)
//        std::cout << "tru" << std::endl;
//    else
//        std::cout << "nietru" << std::endl;
    return ret;
}

bool UnifyAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
{
//    if (commonInterpretations.size() > 0)
//    {
//        for (std::set<boost::regex>::iterator ii = commonInterpretations.begin(); ii != commonInterpretations.end(); ii++)
//        {
//            delete *ii;
//        }
//    }

//    if (commonInterpretations->size() > 0)
//    {
//        for (std::set<boost::regex*>::iterator ii = commonInterpretations->begin(); ii != commonInterpretations->end(); ii++)
//        {
//            delete *ii;
//        }
//    }
    commonInterpretations.clear();

    //std::vector<std::string
    //std::vector<std::set<std::string> > allInterpretations;
    std::vector<std::set<std::string> > allInterpretations;// = new std::vector<std::set<StringPtr> *>;

    std::vector<int>::iterator ie = tokenIndexes.begin();

    //std::vector<std::string> common;
    std::vector<std::string> common;// = new std::vector<StringPtr>;
    int token_count = 0;
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
//        std::cout << "Taki indeks:" << *ie << std::endl;
        //Entity *entity = entities[currentEntity + *ie - 1];
        Entity *entity = entities[currentEntity + index];
        if (entity->getType() == "token")
        {
            //std::set<std::string> tokenInterpretations;
            std::set<std::string> tokenInterpretations;// = new std::set<StringPtr>;
            Token *token = (Token*)entity;
            std::vector<std::string> interps = token->getCompiledInterpretations();
            std::vector<std::string>::iterator ii = interps.begin();
            while (ii != interps.end())
            {
                //std::string mask = baseMask;
                std::string mask = baseMask;
                std::string interpretation = ii->substr(0, maskLength);
                if (token_count > 0)
                    mask = interpretation;
                    if (token_count == 0)
                    {
                        std::vector<std::string> mask_vec(maskLength, ".");
                std::vector<int>::iterator ia = attributeIndexes.begin();
                while (ia != attributeIndexes.end())
                {
                    //if (mask[*ia + 1] == '0')
                    if (interpretation.at(*ia + 1) == '0')
                    {
                        if (this->nullAgreement)
                        {
                            //mask[*ia + 1] = '.';
                            mask.at(*ia + 1) = '.';
                        }
                    }
                    else
                    {
                    //mask[(*ia) + 1] = interpretation[(*ia) + 1];
                        mask.at((*ia) + 1) = interpretation.at((*ia) + 1);
                    }
                    if (this->nullAgreement)
                    {
                            mask_vec[*ia + 1] = interpretation.at((*ia) + 1);
                            mask_vec[*ia + 1] = "[" + mask_vec[*ia + 1];
                            mask_vec[*ia + 1] += "0]";
                    }
                    else
                    {
                        mask_vec[*ia + 1] = interpretation.at((*ia) + 1);
                    }
                    mask = "";
                    for (std::vector<std::string>::iterator vit = mask_vec.begin(); vit != mask_vec.end(); vit ++)
                        mask += *vit;
                    ia ++;
                }
                    }
                if (this->nullAgreement)
                {
                    //if (mask.find(".") != std::string::npos)
                        tokenInterpretations.insert(mask);
                    //else
                     //   common.push_back(mask);
                }
                else
                    tokenInterpretations.insert(mask);
                ii ++;
                //mask.reset();
                //interpretation.reset();
            }
            interps.clear();
            //delete interps;
            allInterpretations.push_back(tokenInterpretations);
        }
        else if (entity->getType() == "group")
        {
//            std::cout << "DO GRUPY na test wpadlem" << std::endl;
            Group *group = (Group*) entity;
            while (group->children.at(group->getHeadIndex())->getType() == "group")
            {
//            std::cout << "pyk" << std::endl;
                group = (Group*) (group->children.at(group->getHeadIndex()));
            }

            if (group->children.at(group->getHeadIndex())->getType() == "token")
            {
                Token *token = (Token*) (group->children.at(group->getHeadIndex()));
                //std::set<std::string> tokenInterpretations;
                std::set<std::string> tokenInterpretations;// = new std::set<StringPtr>;
                //            std::cout << "AJDI: " << token->getId() << std::endl;
                std::vector<std::string> interps = token->getCompiledInterpretations();
                std::vector<std::string>::iterator ii = interps.begin();
                while (ii != interps.end())
                {
                    //std::string mask = baseMask;
                    std::string mask = baseMask;
                    std::string interpretation = ii->substr(0, maskLength);
                    if (token_count > 0)
                        mask = interpretation;
                    if (token_count == 0)
                    {
                        std::vector<std::string> mask_vec(maskLength, ".");
                        std::vector<int>::iterator ia = attributeIndexes.begin();
                        while (ia != attributeIndexes.end())
                        {
                            //if (mask[*ia + 1] == '0')
                            if (interpretation.at(*ia + 1) == '0')
                            {
                                if (this->nullAgreement)
                                    //mask[*ia + 1] = '.';
                                    mask.at(*ia + 1) = '.';
                            }
                            else
                                //mask[(*ia) + 1] = interpretation[(*ia) + 1];
                                mask.at((*ia) + 1) = interpretation.at((*ia) + 1);
                            if (this->nullAgreement)
                            {
                                mask_vec[*ia + 1] = interpretation.at((*ia) + 1);
                                mask_vec[*ia + 1] = "[" + mask_vec[*ia + 1];
                                mask_vec[*ia + 1] += "0]";
                            }
                            else
                            {
                                mask_vec[*ia + 1] = interpretation.at((*ia) + 1);
                            }
                            mask = "";
                            for (std::vector<std::string>::iterator vit = mask_vec.begin(); vit != mask_vec.end(); vit ++)
                                mask += *vit;
                            ia ++;
                        }
                    }
                    if (this->nullAgreement)
                    {
                        //         if (mask.find(".") != std::string::npos)
                        tokenInterpretations.insert(mask);
                        //       else
                        //         common.push_back(mask);
                    }
                    else
                        tokenInterpretations.insert(mask);
                    ii ++;
                    //mask.reset();
                    //interpretation.reset();
                }
                interps.clear();
                //delete interps;
                allInterpretations.push_back(tokenInterpretations);
            }
            else
            {
                Syntok *syntok = (Syntok*) (group->children.at(group->getHeadIndex()));
                //std::set<std::string> tokenInterpretations;
                std::set<std::string> tokenInterpretations;// = new std::set<StringPtr>;
                //            std::cout << "AJDI: " << token->getId() << std::endl;
                std::vector<std::string> interps = syntok->getCompiledInterpretations();
                std::vector<std::string>::iterator ii = interps.begin();
                while (ii != interps.end())
                {
                    //std::string mask = baseMask;
                    std::string mask = baseMask;
                    std::string interpretation = ii->substr(0, maskLength);
                    if (token_count > 0)
                        mask = interpretation;
                    if (token_count == 0)
                    {
                        std::vector<std::string> mask_vec(maskLength, ".");
                        std::vector<int>::iterator ia = attributeIndexes.begin();
                        while (ia != attributeIndexes.end())
                        {
                            //if (mask[*ia + 1] == '0')
                            if (interpretation.at(*ia + 1) == '0')
                            {
                                if (this->nullAgreement)
                                    //mask[*ia + 1] = '.';
                                    mask.at(*ia + 1) = '.';
                            }
                            else
                                //mask[(*ia) + 1] = interpretation[(*ia) + 1];
                                mask.at((*ia) + 1) = interpretation.at((*ia) + 1);
                            if (this->nullAgreement)
                            {
                                mask_vec[*ia + 1] = interpretation.at((*ia) + 1);
                                mask_vec[*ia + 1] = "[" + mask_vec[*ia + 1];
                                mask_vec[*ia + 1] += "0]";
                            }
                            else
                            {
                                mask_vec[*ia + 1] = interpretation.at((*ia) + 1);
                            }
                            mask = "";
                            for (std::vector<std::string>::iterator vit = mask_vec.begin(); vit != mask_vec.end(); vit ++)
                                mask += *vit;
                            ia ++;
                        }
                    }
                    if (this->nullAgreement)
                    {
                        //         if (mask.find(".") != std::string::npos)
                        tokenInterpretations.insert(mask);
                        //       else
                        //         common.push_back(mask);
                    }
                    else
                        tokenInterpretations.insert(mask);
                    ii ++;
                    //mask.reset();
                    //interpretation.reset();
                }
                interps.clear();
                //delete interps;
                allInterpretations.push_back(tokenInterpretations);
            }

        }
        else if (entity->getType() == "syntok")
        {
            //std::set<std::string> tokenInterpretations;
            std::set<std::string> tokenInterpretations;// = new std::set<StringPtr>;
            Syntok *syntok = (Syntok*)entity;
            std::vector<std::string> interps = syntok->getCompiledInterpretations();
            std::vector<std::string>::iterator ii = interps.begin();
            while (ii != interps.end())
            {
                //std::string mask = baseMask;
                std::string mask = baseMask;
                std::string interpretation = ii->substr(0, maskLength);
                if (token_count > 0)
                    mask = interpretation;
                if (token_count == 0)
                {
                    std::vector<std::string> mask_vec(maskLength, ".");
                    std::vector<int>::iterator ia = attributeIndexes.begin();
                    while (ia != attributeIndexes.end())
                    {
                        //if (mask[*ia + 1] == '0')
                        if (interpretation.at(*ia + 1) == '0')
                        {
                            if (this->nullAgreement)
                            {
                                //mask[*ia + 1] = '.';
                                mask.at(*ia + 1) = '.';
                            }
                        }
                        //mask[(*ia) + 1] = interpretation[(*ia) + 1];
                        mask.at((*ia) + 1) = interpretation.at((*ia) + 1);
                        if (this->nullAgreement)
                        {
                            mask_vec[*ia + 1] = interpretation.at((*ia) + 1);
                            mask_vec[*ia + 1] = "[" + mask_vec[*ia + 1];
                            mask_vec[*ia + 1] += "0]";
                        }
                        else
                        {
                            mask_vec[*ia + 1] = interpretation.at((*ia) + 1);
                        }
                        mask = "";
                        for (std::vector<std::string>::iterator vit = mask_vec.begin(); vit != mask_vec.end(); vit ++)
                            mask += *vit;
                        ia ++;
                    }
                }
                if (this->nullAgreement)
                {
                    //if (mask.find(".") != std::string::npos)
                    tokenInterpretations.insert(mask);
                    //else
                    //   common.push_back(mask);
                }
                else
                    tokenInterpretations.insert(mask);
                ii ++;
                //mask.reset();
                //interpretation.reset();
            }
            interps.clear();
            //delete interps;
            allInterpretations.push_back(tokenInterpretations);
        }

        }
        ie ++;
        token_count ++;
    }

    if (allInterpretations.size() == 0)
    {
//        std::vector<std::set<StringPtr> >::iterator i = allInterpretations.begin();
//        while (i != allInterpretations->end())
//        {
//            delete *i;
//            i ++;
//        }
//        delete allInterpretations;
        return false;
    }
    //std::vector<std::set<std::string> >::iterator i = allInterpretations.begin();
    std::vector<std::set<std::string> >::iterator i = allInterpretations.begin();
    //std::set<std::string>::iterator it = i->begin();
    std::set<std::string>::iterator it = i->begin();
    while (it != i->end())
    {
        common.push_back(*it);
        it ++;
    }
    bool ret = false;
    bool diffrent = false;
    i ++; //przejscie do interpretacji tokenu o indeksie 1 (w tablicy indeksow tokenow)
    //std::vector<std::string>::iterator ic = common.begin();
    std::vector<std::string>::iterator ic = common.begin();
    while (ic != common.end())
    {
        bool found = false;
        int interpsFound = 1;
        //std::string znajda;
        i = allInterpretations.begin();
        i ++ ;
        if (i == allInterpretations.end()) // jak sa tylko jednego tokenu interpretacje
        {
            //std::vector<std::set<std::string> >::iterator i = allInterpretations.begin();
            //while (i != allInterpretations.end())
            //{
            //    delete *i;
            //    i ++;
            //}
            //delete allInterpretations;
            return true;
        }

        while (i != allInterpretations.end())
        {
            bool foundInToken = false;
            //std::set<std::string>::iterator s = i->begin();
            std::set<std::string>::iterator s = i->begin();
            while (s != i->end())
            {
                //if (this->nullAgreement)
                //{
                //if (ic->find(".") != std::string::npos)
                //{
                //if (boost::u32regex_match(*s, boost::make_u32regex(*ic)))
//                std::cout << "robie wyrazenie z: " << *s << ". Wzorzec: " << *ic<< std::endl;
                //if (boost::regex_match(*s, boost::regex(*ic)))
//                std::cerr << "666takie wyrazenie: " << **ic << std::endl;
//                std::cerr << "555a taka interpretacja: " << **s << std::endl;
                boost::regex *r = new boost::regex(*ic);
                if (boost::regex_match(*s, *r))
                {
//                    std::cerr << "pasuje " << **s << " do " << r->str() << std::endl;
                    foundInToken = true;
                    //znajda = *s;
                    if (diffrent)
                    {
                        delete r;
                        //       ret = true;
                        break;
                    }
                }
                else
                {
//                    std::cerr << "nei pasuje " << **s << " do " << r->str() << std::endl;
                    diffrent = true;
                }
                delete r;
//                std::cout << "przezylem" << std::endl;
                s ++;
            }

//            if (!foundInToken)
//            {
//                if (i->size() <= common.size())
//                    return false;
//            }

            if (foundInToken)
                interpsFound ++;
                //if (diffrent)
                  //  break;
            //if (s == i->end())
            //{
            //   std::cout << "uszuwam " << *ic << std::endl;
            //       diffrent = true;
            //   ret = true;
            //   ic = common.erase(ic);
            //   continue;
            //}
            //}
            //else
            //{
            //    if (i->find(*ic) == i->end())
            //    {
            //        ret = true; // test zrobi true, tylko jak sa jeszcze interpretacje, ktorych nie trzeba usuwac!
            //        ic = common.erase(ic);
            //        continue;
            //    }
            //}
            //}
            //else
            //{
            //    if (i->find(*ic) == i->end())
            //    {
            //        ret = true; // test zrobi true, tylko jak sa jeszcze interpretacje, ktorych nie trzeba usuwac!
            //        ic = common.erase(ic);
            //        continue;
            //    }
            //}
            i ++;
        }
        if (interpsFound == allInterpretations.size())
            found = true;
        if (!found)
        {
//            std::cerr << "Nie jest wspolny lajdak: " << **ic << std::endl;
            ic = common.erase(ic);
            continue;
        }

        ic ++;
    }

//    i = allInterpretations.begin();
//    while (i != allInterpretations.end())
//    {
//        delete *i;
//        i ++;
//    }
//    delete allInterpretations;

    if (common.size() > 1)
    if (!diffrent)
    {
//        std::cerr << "nie ma roznych!" << std::endl;
 //       delete common;
//        return false;
    }
    //if (!diffrent)
    //{
    //    std::cout << "tu falszuje" << std::endl;
    //    return false;
    //}

    if (common.size() == 0)
    {
        //delete common;
//       std::cout << "wychodze bo pusto" << std::endl;
        return false;
    }
    else
    {
    //    if (ret)
      //  {
            //std::vector<std::string>::iterator ic = common.begin();
            std::vector<std::string>::iterator ic = common.begin();
            while (ic != common.end())
            {
                //commonInterpretations.insert(boost::make_u32regex("^" + *ic + "[^<>]+"));
//                std::cerr << "robie wyrazenie2: " << *ic << std::endl;
                //commonInterpretations.insert(boost::regex("^" + *ic + "[^<>]+"));
                commonInterpretations.insert(boost::regex("^" + *ic + "[^<>]+"));
//                std::cout << "przezylem" << std::endl;
                ic++;
            }

            //delete common;

       //     return true;
       // }
      //  else
      //return false;
      return true;
    }
}

std::vector<int> UnifyAction::getAttributeIndexes()
{
    return attributeIndexes;
}

void UnifyAction::setAttributeIndexes(std::vector<int> aAttributeIndexes)
{
    attributeIndexes.clear();
    attributeIndexes.insert(attributeIndexes.begin(), aAttributeIndexes.begin(), aAttributeIndexes.end());
    //attributeIndexes = &aAttributeIndexes;
}

std::vector<int> UnifyAction::getTokenIndexes()
{
    return tokenIndexes;
}

void UnifyAction::setTokenIndexes(std::vector<int> aTokenIndexes)
{
    tokenIndexes.clear();
    tokenIndexes.insert(tokenIndexes.begin(), aTokenIndexes.begin(), aTokenIndexes.end());
    //tokenIndexes = &aTokenIndexes;
}

void UnifyAction::setBaseMask(std::string aBaseMask)
{
    baseMask = aBaseMask;
    maskLength = baseMask.length();
}

std::vector<std::string> UnifyAction::getUAttributes()
{
    return attributes_;
}

}

}

}
