
#include "unify_action.hpp"
#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

UnifyAction::UnifyAction(std::vector<std::string> aUnifiedPatterns,
        std::vector<std::string> aUnifiedAttributes,
        std::vector<int> aTokenIndices, std::vector<std::string> uAttributes) {
//        LatticeWrapperPtr aLatticeWrapper) {
    nullAgreement = true; //@todo: to ma byc parametrem parsera

    //baseMask = "";
    //maskLength = 0;

    type = "unify";
    verbose = false;

    tokenIndices.insert(tokenIndices.begin(), aTokenIndices.begin(),
            aTokenIndices.end());

    attributes_.insert(attributes_.begin(), uAttributes.begin(), uAttributes.end());

    unifiedAttributes = aUnifiedAttributes;
    unifiedPatterns = aUnifiedPatterns;

//    latticeWrapper = aLatticeWrapper;
}

/*UnifyAction::UnifyAction(std::vector<int> aAttributeIndexes, std::vector<int> aTokenIndexes, std::vector<std::string> uAttributes)
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
}*/

UnifyAction::~UnifyAction()
{
    //delete attributeIndexes;
    //delete tokenIndexes;

    //delete commonInterpretations;

    //elete attributes_;
}

//bool UnifyAction::apply(ParseGraphPtr pg, Lattice &lattice, int currentEntity,
bool UnifyAction::apply(Lattice &lattice, int currentEntity,
        std::vector<int> matchedTokensSize) {

   // std::vector<std::string>::iterator attribute_it = unifiedAttributes.begin();
    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
            pattern_it != unifiedPatterns.end(); ++ pattern_it) {

        std::set<std::string> refValues;
        for (std::vector<int>::iterator index_it = tokenIndices.begin();
                index_it != tokenIndices.end(); ++ index_it) {

            int count = matchedTokensSize[*index_it - 1];
            if (count == 0) {
                continue;
            }
            int before = 0;
            int i = 0;
            while (i < (*index_it - 1)) {
                before += matchedTokensSize[i];
                i ++;
            }

            //Lattice::VertexDescriptor vertex = currentEntity + before;
            Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                    currentEntity + before);
            //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
            while (lattice::getTopEdges(lattice, vertex).size() == 0) {
                before ++;
                vertex = currentEntity + before;
            }

            int offset = currentEntity + before;
            int vertexI = 0;
            //for (vertex = currentEntity + before;
            //        vertex < (currentEntity + before + count); vertex ++) {
            while (vertexI < count) {
                vertex = lattice::getVertex(lattice, offset + vertexI);
                std::list<Lattice::EdgeDescriptor> edges =
                    lattice::getTopEdges(lattice, vertex);
                std::set<std::string> values;
                //                TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
                //                for (std::vector<PosInfo>::iterator var_it =
                //                        edge->variants_.begin();
                //                        var_it != edge->variants_.end();
                //                        var_it ++) {
                //                    if (! boost::get<2>(*var_it) )
                //                        continue;
                //                    std::string morphology = boost::get<1>(*var_it);
                for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                        edges.begin();
                        edgeIt != edges.end(); ++ edgeIt) {
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(*edgeIt);
                    if (lattice.getAnnotationItemManager().getValue(
                                annotationItem, "discard") == "1")
                        continue; //skip discarded edges
                    std::string morphology = lattice.getAnnotationItemManager().
                        getValue(annotationItem, "morphology");

                    std::string slot;
                    std::string value;
                    if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
                        values.insert(value);
                    }

                    if (! values.empty()) {
                        if (refValues.empty()) {
                            refValues.insert(values.begin(), values.end());
                        } else {
                            std::set<std::string> intersection;
                            std::set<std::string>::iterator it;
                            std::set_intersection(refValues.begin(), refValues.end(),
                                    values.begin(), values.end(),
                                    std::inserter(intersection, intersection.begin())
                                    );
                            refValues.clear();
                            refValues.insert(intersection.begin(), intersection.end());
                        }
                    }
                }
                vertexI ++;
            }
            }

            for (std::vector<int>::iterator index_it = tokenIndices.begin();
                    index_it != tokenIndices.end(); ++ index_it) {

                int count = matchedTokensSize[*index_it - 1];
                if (count == 0) {
                    continue;
                }
                int before = 0;
                int i = 0;
                while (i < (*index_it - 1)) {
                    before += matchedTokensSize[i];
                    i ++;
                }
                //Lattice::VertexDescriptor vertex = currentEntity + before;
                Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                        currentEntity + before);
                //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
                while (lattice::getTopEdges(lattice, vertex).empty()) {
                    before ++;
                    vertex = currentEntity + before;
                }

                int offset = currentEntity + before;
                int vertexI = 0;
                //for (vertex = currentEntity + before;
                //        vertex < (currentEntity + before + count); vertex ++) {
                while (vertexI < count) {
                    vertex = lattice::getVertex(lattice, offset + vertexI);
                    std::list<Lattice::EdgeDescriptor> edges =
                        lattice::getTopEdges(lattice, vertex);
                    for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                            edges.begin();
                            edgeIt != edges.end(); ++ edgeIt) {
                        AnnotationItem annotationItem =
                            lattice.getEdgeAnnotationItem(*edgeIt);
                        if (lattice.getAnnotationItemManager().getValue(
                                    annotationItem, "discard") == "1")
                            continue; //skip discarded edges
                        std::string morphology = lattice.getAnnotationItemManager().
                            getValue(annotationItem, "morphology");

                        std::string slot;
                        std::string value;
                        if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
                            std::set<std::string>::iterator unifIt = refValues.find(value);
                            if (unifIt == refValues.end()) {
                                //boost::get<2>(*var_it) = 0; //delete the variant
                                //discard the edge
                                lattice.getAnnotationItemManager().setValue(annotationItem,
                                        "discard", "1");
                            }
                        }
                    }
                    vertexI ++;
                }

                //attribute_it ++;
            }
        }
    return true;
//    std::vector<std::string>::iterator attribute_it = unifiedAttributes.begin();
//    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
//            pattern_it != unifiedPatterns.end(); pattern_it ++) {
//
//        for (std::vector<int>::iterator index_it = tokenIndices.begin();
//                index_it != tokenIndices.end(); index_it ++) {
//
//            int count = matchedTokensSize[*index_it - 1];
//            if (count == 0) {
//                continue;
//            }
//            int before = 0;
//            int i = 0;
//            while (i < (*index_it - 1)) {
//                before += matchedTokensSize[i];
//                i ++;
//            }
//
//            for (int edge_i = before; edge_i < (before + count); edge_i ++) {
//                TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
//                for (std::vector<PosInfo>::iterator var_it =
//                        edge->variants_.begin();
//                        var_it != edge->variants_.end();
//                        var_it ++) {
//                    if (! boost::get<2>(*var_it) )
//                        continue;
//                    std::string morphology = boost::get<1>(*var_it);
//
//                    std::string slot;
//                    std::string value;
//                    if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
//                        bool valueFound = false;
//                        std::vector<std::string> unified = unifiedValues.find(*attribute_it)->second;
//                        for (std::vector<std::string>::iterator unifIt = unified.begin();
//                                unifIt != unified.end(); unifIt ++) {
//                            if (*unifIt == value) {
//                                valueFound = true;
//                                break;
//                            }
//                        }
//                        if (!valueFound)
//                            boost::get<2>(*var_it) = 0; //delete the variant
//                    }
//                }
//            }
//
//        }
//        attribute_it ++;
//    }
//    return true;
}


/*bool UnifyAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
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
            //std::set<boost::regex>::iterator c = commonInterpretations.begin();
            std::set<PatternPtr>::iterator c = commonInterpretations.begin();
            while (c != commonInterpretations.end())
            {
                std::vector<std::string>::iterator ic = compiled.begin();
                std::vector<std::string>::iterator im = morpho.begin();
                while (ic != compiled.end())
                {

                    //if (boost::u32regex_match(*ic, *c))
                    //boost::regex *r = new boost::regex(*c);
                    PatternPtr r = *c;
                    //if (boost::regex_match(*ic, *r))
                    if (RE2::FullMatch(*ic, *r)) {
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

                        //delete r;
                        ic = compiled.erase(ic);
                        im = morpho.erase(im);
                        continue;
                    }
                    //delete r;

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
                //std::set<boost::regex>::iterator c = commonInterpretations.begin();
                std::set<PatternPtr>::iterator c = commonInterpretations.begin();
                while (c != commonInterpretations.end())
                {
                    std::vector<std::string>::iterator ic = compiled.begin();
                    std::vector<std::string>::iterator im = morpho.begin();
                    while (ic != compiled.end())
                    {
                        //if (boost::u32regex_match(*ic, *c))
                        //boost::regex *r = new boost::regex(*c);
                        PatternPtr r = *c;
                        //if (boost::regex_match(*ic, *r))
                        if (RE2::FullMatch(*ic, *r)) {
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

                            //delete r;
                            ic = compiled.erase(ic);
                            im = morpho.erase(im);
                            continue;
                        }
                        //delete r;
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
                for (e = edges.begin(); e != edges.end(); e ++) {
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
                //std::set<boost::regex>::iterator c = commonInterpretations.begin();
                std::set<PatternPtr>::iterator c = commonInterpretations.begin();
                while (c != commonInterpretations.end())
                {
                    std::vector<std::string>::iterator ic = compiled.begin();
                    std::vector<std::string>::iterator im = morpho.begin();
                    while (ic != compiled.end())
                    {
                        //if (boost::u32regex_match(*ic, *c))
                        //boost::regex *r = new boost::regex(*c);
                        PatternPtr r = *c;
                        //if (boost::regex_match(*ic, *r))
                        if (RE2::FullMatch(*ic, *r))
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

                            //delete r;
                            ic = compiled.erase(ic);
                            im = morpho.erase(im);
                            continue;
                        }
                        //delete r;
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
            //std::set<boost::regex>::iterator c = commonInterpretations.begin();
            std::set<PatternPtr>::iterator c = commonInterpretations.begin();
            while (c != commonInterpretations.end())
            {
                std::vector<std::string>::iterator ic = compiled.begin();
                std::vector<std::string>::iterator im = morpho.begin();
                while (ic != compiled.end())
                {
                    //if (boost::u32regex_match(*ic, *c))
                    //boost::regex *r = new boost::regex(*c);
                    PatternPtr r = *c;
                    //if (boost::regex_match(*ic, *r))
                    if (RE2::FullMatch(*ic, *r))
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

                        //delete r;
                        ic = compiled.erase(ic);
                        im = morpho.erase(im);
                        continue;
                    }
                    //delete r;
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
}*/

//bool UnifyAction::test(ParseGraphPtr pg, Lattice &lattice, int currentEntity,
bool UnifyAction::test(Lattice &lattice, int currentEntity,
        std::vector<int> matchedTokensSize) {

    //unifiedValues.clear();
    bool toApply = true;
    std::vector<std::string>::iterator attribute_it = unifiedAttributes.begin();
    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
            pattern_it != unifiedPatterns.end(); ++ pattern_it) {

        bool wasAllNulls = false;
//        bool wasNotNull = false;

        std::set<std::string> refValues;
        for (std::vector<int>::iterator index_it = tokenIndices.begin();
                index_it != tokenIndices.end(); ++ index_it) {

            int count = matchedTokensSize[*index_it - 1];
            if (count == 0) {
                continue;
            }
            int before = 0;
            int i = 0;
            while (i < (*index_it - 1)) {
                before += matchedTokensSize[i];
                i ++;
            }

            //Lattice::VertexDescriptor vertex = currentEntity + before;
            Lattice::VertexDescriptor vertex = lattice::getVertex(lattice,
                    currentEntity + before);
            //@todo: czy to sprawdzenie jest nadal konieczne? ta funkcja getVertex nie robi czegos takiego?
            while (lattice::getTopEdges(lattice, vertex).size() == 0) {
                before ++;
                vertex = currentEntity + before;
            }

            int offset = currentEntity + before;
            int vertexI = 0;
            //for (vertex = currentEntity + before;
            //        vertex < (currentEntity + before + count); vertex ++) {
            while (vertexI < count) {
                vertex = lattice::getVertex(lattice, offset + vertexI);
                //TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
                std::list<Lattice::EdgeDescriptor> edges =
                    lattice::getTopEdges(lattice, vertex);
                bool allValuesNull = true;
                std::set<std::string> values;
                for (std::list<Lattice::EdgeDescriptor>::iterator edgeIt =
                        edges.begin();
                        edgeIt != edges.end(); ++ edgeIt) {
                    AnnotationItem annotationItem =
                        lattice.getEdgeAnnotationItem(*edgeIt);
                    if (lattice.getAnnotationItemManager().getValue(
                                annotationItem, "discard") == "1")
                        continue; //skip discarded edges
                //for (std::vector<PosInfo>::iterator var_it =
                //        edge->variants_.begin();
                //        var_it != edge->variants_.end();
                //        var_it ++) {
                //    if (! boost::get<2>(*var_it) )
                //        continue;
                    //std::string morphology = boost::get<1>(*var_it);
                    std::string morphology = lattice.getAnnotationItemManager().
                        getValue(annotationItem, "morphology");

                    std::string slot;
                    std::string value;
                    if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
                        allValuesNull = false;
                        values.insert(value);
                    }
                }
                if (! values.empty()) {
//                    wasNotNull = true;
                    if (refValues.empty()) {
                        refValues.insert(values.begin(), values.end());
                    } else {
                        std::set<std::string> intersection;
                        std::set<std::string>::iterator it;
                        std::set_intersection(refValues.begin(), refValues.end(),
                                values.begin(), values.end(),
                                std::inserter(intersection, intersection.begin())
                                );
                        refValues.clear();
                        refValues.insert(intersection.begin(), intersection.end());
                    }
                }
                if (allValuesNull && (! wasAllNulls)) {
                    wasAllNulls = true;
                }
                vertexI ++;
            }

        }
        if (! refValues.empty()) { //there are some common values of the attribute
            if (wasAllNulls) { //there is a token with no value of the attribute defined
                if (! nullAgreement)
                    toApply = false; //cannot apply
            }
        } else { //no common values of the attribute, cannot unify
            toApply = false;
        }
        if (! toApply)
            break;
//        else {
//            std::vector<std::string> attributeUnifiedValues(
//                    refValues.begin(), refValues.end());
//            unifiedValues.insert(std::pair<std::string, std::vector<std::string> >(
//                        *attribute_it, attributeUnifiedValues
//                        ));
//        }
        ++ attribute_it;
    }
    return toApply;
//    bool toApply = true;
//    std::vector<std::string>::iterator attribute_it = unifiedAttributes.begin();
//    for (std::vector<std::string>::iterator pattern_it = unifiedPatterns.begin();
//            pattern_it != unifiedPatterns.end(); pattern_it ++) {
//
//        bool wasAllNulls = false;
//        bool wasNotNull = false;
//
//        std::set<std::string> refValues;
//        for (std::vector<int>::iterator index_it = tokenIndices.begin();
//                index_it != tokenIndices.end(); index_it ++) {
//
//            int count = matchedTokensSize[*index_it - 1];
//            if (count == 0) {
//                continue;
//            }
//            int before = 0;
//            int i = 0;
//            while (i < (*index_it - 1)) {
//                before += matchedTokensSize[i];
//                i ++;
//            }
//
//            for (int edge_i = before; edge_i < (before + count); edge_i ++) {
//                TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
//                bool allValuesNull = true;
//                std::set<std::string> values;
//                for (std::vector<PosInfo>::iterator var_it =
//                        edge->variants_.begin();
//                        var_it != edge->variants_.end();
//                        var_it ++) {
//                    if (! boost::get<2>(*var_it) )
//                        continue;
//                    std::string morphology = boost::get<1>(*var_it);
//
//                    std::string slot;
//                    std::string value;
//                    if (RE2::FullMatch(morphology, *pattern_it, &slot, &value)) {
//                        allValuesNull = false;
//                        values.insert(value);
//                    }
//                }
//                if (values.size() > 0) {
//                    wasNotNull = true;
//                    if (refValues.size() == 0) {
//                        refValues.insert(values.begin(), values.end());
//                    } else {
//                        std::set<std::string> intersection;
//                        std::set<std::string>::iterator it;
//                        std::set_intersection(refValues.begin(), refValues.end(),
//                                values.begin(), values.end(),
//                                std::inserter(intersection, intersection.begin())
//                                );
//                        refValues.clear();
//                        refValues.insert(intersection.begin(), intersection.end());
//                    }
//                }
//                if (allValuesNull && (! wasAllNulls)) {
//                    wasAllNulls = true;
//                }
//            }
//
//        }
//        if (refValues.size() > 0) { //there are some common values of the attribute
//            if (wasAllNulls) { //there is a token with no value of the attribute defined
//                if (! nullAgreement)
//                    toApply = false; //cannot apply
//            }
//        } else { //no common values of the attribute, cannot unify
//            toApply = false;
//        }
//        if (! toApply)
//            break;
//        else {
//            std::vector<std::string> attributeUnifiedValues(
//                    refValues.begin(), refValues.end());
//            unifiedValues.insert(std::pair<std::string, std::vector<std::string> >(
//                        *attribute_it, attributeUnifiedValues
//                        ));
//        }
//        attribute_it ++;
//    }
//    return toApply;
}

/*bool UnifyAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
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
                //boost::regex *r = new boost::regex(*ic);
                RE2 r(*ic);
                //if (boost::regex_match(*s, *r))
                if (RE2::FullMatch(*s, r))
                {
//                    std::cerr << "pasuje " << **s << " do " << r->str() << std::endl;
                    foundInToken = true;
                    //znajda = *s;
                    if (diffrent)
                    {
                        //delete r;
                        //       ret = true;
                        break;
                    }
                }
                else
                {
//                    std::cerr << "nei pasuje " << **s << " do " << r->str() << std::endl;
                    diffrent = true;
                }
                //delete r;
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
                //commonInterpretations.insert(boost::regex("^" + *ic + "[^<>]+"));
                commonInterpretations.insert(PatternPtr(new RE2( "^" + *ic + "[^<>]+" )));
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
}*/

//std::vector<int> UnifyAction::getAttributeIndexes()
//{
//    return attributeIndexes;
//}
//
//void UnifyAction::setAttributeIndexes(std::vector<int> aAttributeIndexes)
//{
//    attributeIndexes.clear();
//    attributeIndexes.insert(attributeIndexes.begin(), aAttributeIndexes.begin(), aAttributeIndexes.end());
//    //attributeIndexes = &aAttributeIndexes;
//}

std::vector<std::string> UnifyAction::getPatterns() {
    return unifiedPatterns;
}

void UnifyAction::setPatterns(std::vector<std::string> aPatterns) {
    unifiedPatterns = aPatterns;
}

std::vector<std::string> UnifyAction::getAttributes() {
    return unifiedAttributes;
}

void UnifyAction::setAttributes(std::vector<std::string> aAttributes) {
    unifiedAttributes = aAttributes;
}

std::vector<int> UnifyAction::getTokenIndices() {
    return tokenIndices;
}

void UnifyAction::setTokenIndices(std::vector<int> aTokenIndices) {
    tokenIndices.clear();
    tokenIndices.insert(tokenIndices.begin(), aTokenIndices.begin(),
            aTokenIndices.end());
}

//void UnifyAction::setBaseMask(std::string aBaseMask)
//{
//    baseMask = aBaseMask;
//    maskLength = baseMask.length();
//}

std::vector<std::string> UnifyAction::getUAttributes() {
    return attributes_;
}

}

}

}
