
#include "syntok_action.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

//SyntokAction::SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndexes, std::vector<InterpretationPair> aMorphology, std::string aRuleName, std::string uMorphology)
SyntokAction::SyntokAction(int aStart, int aEnd, std::vector<int> aTokenIndices,
        std::vector<std::string> aMorphology, std::string aRuleName,
        std::string uMorphology) { //, LatticeWrapperPtr aLatticeWrapper) {
    type = "syntok";
    start = aStart;
    end = aEnd;
    ruleName = aRuleName;
    verbose = false;
    syntok = false;
    //tokenIndexes = aTokenIndexes;
    tokenIndices.insert(tokenIndices.begin(), aTokenIndices.begin(),
            aTokenIndices.end());// = new std::vector<int>(aTokenIndexes);
    //morphology = aMorphology;
    morphology.insert(morphology.begin(), aMorphology.begin(), aMorphology.end());// = new std::vector<InterpretationPair>(aMorphology);

    morphology_ = uMorphology;

//    latticeWrapper = aLatticeWrapper;
}

SyntokAction::~SyntokAction()
{
    //delete tokenIndexes;
    //delete morphology;
}

//bool SyntokAction::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
//@todo: opisac dokladnie, co konkretnie po kolei jest robione w ramach implementacji tej akcji (i z kazda akcja co jest robione). pobranie czego, wyliczenie czego, zebranie jakich form, dodanie jakich krawedzi, zastapienie/usuniecie czego, skad
//1. ustalenie skad zaczynamy
//2. zebranie scalonych form bazowych
//3. wstawienie nowej krawedzi typu token albo grup (w zaleznosci od przelacznika) NOWOSC: wczesniej bylo, ze albo dawaj krawedz group albo nic. teraz musi byc krawedz ,jak graf to jedyna struktura. ewentualnei mozna dac na koniec opcje, wywal syntoki z grafu, przeliczyc reszte grafu
//bool SyntokAction::apply(ParseGraphPtr pg, Lattice &lattice, int currentEntity,
bool SyntokAction::apply(Lattice &lattice, int currentEntity,
        std::vector<int> matchedTokensSize) {
    int realStart = 0;// start;
    int realEnd = 0;//start;
    size_t i = 0;
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

    //gets the base forms of all matched tokens. permutes those forms
    //in order to create a list of concatenated base forms.
    //those forms become the base forms of the added syntok
//    std::vector<std::string> base_forms; //concatenated base forms container
//    std::string concatenatedOrth = "";
//    for (int edge_i = realStart; edge_i <= realEnd; edge_i ++) { //to ma byc < realEnd czy <= realEnd
//        TransitionInfo *edge = util::getEdge(pg, currentEntity, edge_i);
//        if (concatenatedOrth != "")
//            concatenatedOrth += " ";
//        concatenatedOrth += edge->getOrth();
//
//        for (std::vector<PosInfo>::iterator var_it = edge->variants_.begin();
//                var_it != edge->variants_.end(); var_it ++) {
//            std::string base = boost::get<0>(*var_it);
//            if (base_forms.size() > 0) {
//                std::vector<std::string> base_forms_tmp;
//                for (std::vector<std::string>::iterator conc_it = base_forms.begin();
//                        conc_it != base_forms.end(); conc_it ++) {
//                    std::string concatenated = *conc_it + " " + base;
//                    base_forms_tmp.push_back(concatenated);
//                }
//                base_forms.clear();
//                base_forms.assign( base_forms_tmp.begin(),
//                                   base_forms_tmp.end() );
//            } else {
//                base_forms.push_back(base);
//            }
//        }
//    }
    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, currentEntity + realStart);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, currentEntity + realEnd);
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, startVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, endVertex);
    std::vector<std::string> baseForms;
    std::list<Lattice::EdgeSequence> edgeSequences =
        lattice::getEdgesRange(
                lattice, startVertex, endVertex
                );
    for (std::list<Lattice::EdgeSequence>::iterator sequenceIt =
            edgeSequences.begin();
            sequenceIt != edgeSequences.end();
            ++ sequenceIt) {
        std::string baseForm = "";
        int index = lattice.getEdgeBeginIndex(sequenceIt->firstEdge());
        for (Lattice::EdgeSequence::Iterator edgeIt = sequenceIt->begin();
                edgeIt != sequenceIt->end();
                ++ edgeIt) {
            AnnotationItem ai = lattice.getEdgeAnnotationItem(*edgeIt);
            if (lattice.getAnnotationItemManager().getValue(
                    ai, "discard") == "1")
                continue;

            //std::string base = lattice.getAnnotationItemManager().getValue(
            //        ai, "base");
            std::string base = lattice::getBase(lattice, *edgeIt);

            if (index != lattice.getEdgeBeginIndex(*edgeIt)) {
                baseForm += " ";
                index ++;
            }
            baseForm += base;
            index += lattice.getEdgeLength(*edgeIt);
//            if (baseForms.size() > 0) {
//                std::vector<std::string> tmpBaseForms;
//                for (std::vector<std::string>::iterator concIt =
//                        baseForms.begin();
//                        concIt != baseForms.end();
//                        concIt ++) {
//                    std::string concatenated = *concIt + " " + base;
//                    tmpBaseForms.push_back(concatenated);
//                }
//                baseForms.clear();
//                baseForms.assign( tmpBaseForms.begin(), tmpBaseForms.end());
//            } else {
//                baseForms.push_back(base);
//            }
        }
        baseForms.push_back(baseForm);
    }
    std::string concatenatedOrth = "";
    int index = lattice.getEdgeBeginIndex(edgeSequences.front().firstEdge());
    for (Lattice::EdgeSequence::Iterator edgeIt = edgeSequences.front().begin();
            edgeIt != edgeSequences.front().end();
            ++ edgeIt) {
        if (index != lattice.getEdgeBeginIndex(*edgeIt)) {
            concatenatedOrth += " ";
            index ++;
        }
        concatenatedOrth += lattice.getEdgeText(*edgeIt);
        index += lattice.getEdgeLength(*edgeIt);
    }
    //std::string concatenatedOrth = lattice.getSequenceText(edgeSequences.front());
    std::string syntokCategory;
    if (syntok) {
        syntokCategory = "SYNTOK";
        LayerTagCollection tags =
            lattice.getLayerTagManager().createSingletonTagCollection("parse");
        lattice::addSyntokEdges(
                lattice,
                startEdges,
                endEdges,
                syntokCategory,
                concatenatedOrth,
                baseForms,
                morphology,
                edgeSequences,
                tags);
    } else {
        syntokCategory = concatenatedOrth;
        LayerTagCollection tags =
            lattice.getLayerTagManager().createSingletonTagCollection("lemma");
        lattice::addSyntokEdges(
                lattice,
                startEdges,
                endEdges,
                syntokCategory,
                concatenatedOrth,
                baseForms,
                morphology,
                edgeSequences,
                tags);
    }
/*    std::vector<std::string> concatenated;// = new std::vector<StringPtr>;
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
    }*/

    /*Syntok *syntok = new Syntok(entities, currentEntity + realStart, currentEntity + realEnd, ruleName);

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
        }*/

//    std::cout << "robie regule: " << ruleName << std::endl;

    //syntok->updateCompiled();
    //entities.insert(entities.begin() + currentEntity + realStart, syntok);

    //if (syntok) {
        //TransitionInfo *wd = new TransitionInfo("token"); //byl word, co mi to zmieni?
        //TransitionInfo *wd = new TransitionInfo("group"); //byl word, co mi to zmieni?
//        TransitionInfo *syntok = new TransitionInfo("group");
//        //if 'syntok' flag set adds a "group" edge with the label "SYNTOK"
//        if (! syntok) {
//            //otherwise adds a 'token' edge with the label made of concatenated orth forms of the constituing edges
//            syntok->setType("token");
//        }
//        //wd->setId(syntok->getId());
//        //wd->setLabel(*(syntok->getOrth()));
//        syntok->setId( util::getNewEdgeId(pg) );
//        if (syntok) {
//            syntok->setLabel("SYNTOK");
//        } else {
//            syntok->setLabel(concatenatedOrth);
//        }
//        syntok->setOrth(concatenatedOrth);
        //std::istringstream iss;// = new std::istringstream;

        //iss.str (word->contents[0]->getId());
        //iss.str(syntok->getStart());
        //int num;
        //iss >> std::hex >> num;
        ////    wd->setStart(boost::lexical_cast<int>(word->contents[0]->getId()));
        ////    wd->setEnd(boost::lexical_cast<int>(word->contents[word->contents.size() - 1]->getId()));
        //wd->setStart(num - 1);

//        std::cerr << "ustawiam start na: " << num << std::endl;
        //delete iss;

        //std::istringstream iss2;// = new std::istringstream;
        ////iss2.str (word->contents[word->contents.size() - 1]->getId());
        //iss2.str (syntok->getEnd());
        ////std::cout << iss2.str() << std::endl;
        //int num2;
        //iss2 >> std::hex >> num2;
        //wd->setEnd(num2);
//        std::cerr << "ustawiam koniec na: " << (num2 + 1) << std::endl;
        //delete iss2;
//        TransitionInfo *edgeStart = util::getEdge(pg, currentEntity, realStart);
//        TransitionInfo *edgeEnd = util::getEdge(pg, currentEntity, realEnd);
//        syntok->setStart(edgeStart->getStart());
//        syntok->setEnd(edgeEnd->getEnd());
//        syntok->setDepth(edgeStart->getDepth() + 1);
//        if (syntok) {
//            //note: tu sztucznie wymuszam numerowanie od 2. glebokosc 1 maja miec krawedzie typu 'pos', ale one sa dodawane dopiero po zakonczeniu parsingu, wiec trzeba nie jako zalozyc tu, ze takowe istnieja
//            if (syntok->getDepth() == 1) //note: tu nastepuje wspomniany wyzej trik
//                syntok->setDepth(2);
//        }
//
//        //for (std::vector<std::string>::iterator conc = concatenated.begin(); conc != concatenated.end(); conc ++)
//        for (std::vector<std::string>::iterator base_it = base_forms.begin();
//                base_it != base_forms.end(); base_it ++) {
//            //for (std::vector<InterpretationPair>::iterator morpho_it = morphology.begin();
//            for (std::vector<std::string>::iterator morpho_it = morphology.begin();
//                    morpho_it != morphology.end(); morpho_it ++) {
//                syntok->addMorphology(PosInfo(*base_it, *morpho_it, 1));
//            }
//        }

/*        Edges::iterator e = edges.begin();
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
        wd->setDepth(2);*/

        //edges.push_back(wd);
//        pg->add_edge( syntok->getStart(), syntok->getEnd(), *syntok);
    /*} else {
        //NEW: adds a 'token' edge with the label made of concatenated orth forms of the constituing edges
        //old version below in comment - to be deleted
        for (std::vector<std::string>::iterator conc = concatenated.begin(); conc != concatenated.end(); conc ++)
//        {
//            for (std::vector<InterpretationPair>::iterator ip = morphology.begin(); ip != morphology.end(); ip ++)
//           {
//                std::string morph = *conc + ":" + ip->first;
//                std::string comp = ip->second + *conc;
//                syntok->addInterpretation(morph, comp);
//                //delete morph;
//                //delete comp;
//            }
//        }
        TransitionInfo *syntok = new TransitionInfo("group");
        syntok->setId( util::getNewEdgeId(pg) );
        syntok->setLabel("SYNTOK");
        TransitionInfo *edgeStart = util::getEdge(pg, currentEntity, realStart);
        TransitionInfo *edgeEnd = util::getEdge(pg, currentEntity, realEnd);
        syntok->setStart(edgeStart->getStart());
        syntok->setEnd(edgeEnd->getEnd());
        //note: tu sztucznie wymuszam numerowanie od 2. glebokosc 1 maja miec krawedzie typu 'pos', ale one sa dodawane dopiero po zakonczeniu parsingu, wiec trzeba nie jako zalozyc tu, ze takowe istnieja
        syntok->setDepth(edgeStart->getDepth() + 1);
        if (syntok->getDepth() == 1) //note: tu nastepuje wspomniany wyzej trik
            syntok->setDepth(2);

        for (std::vector<std::string>::iterator base_it = base_forms.begin();
                base_it != base_forms.end(); base_it ++) {
            for (std::vector<InterpretationPair>::iterator morpho_it = morphology.begin();
                    morpho_it != morphology.end(); morpho_it ++) {
                syntok->addMorphology(PosInfo(*base_it, morpho_it->first, 1));
            }
        }

        pg->add_edge( syntok->getStart(), syntok->getEnd(), *syntok);
        //@todo: najlepiej by bylo to wszystko wyzej wpisac w jednym miejscu, bo to jest to samo. rozni sie tylko typem krawedzi i etykieta! oraz nie wymuszaniem glebokosci (czyt. wymuszeniem na 1 prawdopodobnie
    }*/

    //delete concatenated;

    return true;
}

//bool SyntokAction::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
//bool SyntokAction::test(ParseGraphPtr pg, Lattice &lattice, int currentEntity,
bool SyntokAction::test(Lattice &, int,
        std::vector<int> matchedTokensSize) {
    //int sum = 0;
    for (std::vector<int>::iterator i = matchedTokensSize.begin();
            i != matchedTokensSize.end(); ++ i) {
        if (*i > 0)
            return true;
    }
    return false;
}

int SyntokAction::getStart() const {
    return start;
}

void SyntokAction::setStart(int aStart)
{
    start = aStart;
}

int SyntokAction::getEnd() const {
    return end;
}

void SyntokAction::setEnd(int aEnd)
{
    end = aEnd;
}

//std::vector<InterpretationPair> SyntokAction::getMorphology()
std::vector<std::string> SyntokAction::getMorphology() const {
    return morphology;
}

void SyntokAction::setMorphology(std::vector<std::string> aMorphology) {
    morphology.clear();
    morphology.insert(morphology.begin(), aMorphology.begin(), aMorphology.end());
    //morphology = &aMorphology;
}

std::vector<int> SyntokAction::getTokenIndices() const {
    return tokenIndices;
}

void SyntokAction::setTokenIndices(std::vector<int> aTokenIndices)
{
    tokenIndices.clear();
    tokenIndices.insert(tokenIndices.begin(), aTokenIndices.begin(),
            aTokenIndices.end());
    //tokenIndexes = &aTokenIndexes;
}

std::string SyntokAction::getRuleName() const {
    return ruleName;
}

void SyntokAction::setRuleName(std::string aRuleName)
{
    ruleName = aRuleName;
}

std::string SyntokAction::getUMorphology() const {
    return morphology_;
}

}

}

}
