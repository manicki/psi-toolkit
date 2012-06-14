#include "join_action.hpp"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace poleng {

namespace bonsai {
    namespace puddle {

JoinAction::JoinAction(std::string aGroup, int aStart, int aEnd, int aHead,
        std::string aRuleName) {
    init(aGroup, aStart, aEnd, aHead, aRuleName);
}

bool JoinAction::apply(Lattice &lattice, std::string langCode,
        int matchedStartIndex, RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence> &rulePartitions) {
    int realStart;
    int realEnd;
    int realHead;
    if (! util::getGroupActionParams(ruleTokenSizes, start, head, end,
                realStart, realHead, realEnd)) {
        throw PuddleRuleElementNotMatchedException(
                "Group head element " + boost::lexical_cast<std::string>(head)
                + " is empty"
                );
    }

    Lattice::VertexDescriptor startVertex = lattice::getVertex(
            lattice, langCode, realStart, matchedStartIndex);
    Lattice::VertexDescriptor headVertex = lattice::getVertex(
            lattice, langCode, realHead, matchedStartIndex);
    Lattice::VertexDescriptor endVertex = lattice::getVertex(
            lattice, langCode, realEnd, matchedStartIndex);
    lattice::removeParseEdges(lattice, langCode, startVertex, endVertex);
    //@todo: nie jestem przekonany,
    //czy to jest dobre miejsce. addParseEdges moze sie wowczas nie powiesc.
    //z drugiej strony, jak krawedzie nie sa usuniete tylko discarded,
    //to moze sie nic nie stac.
    //co tylko z groupPartitions? nie powinno byc generowane po usunieciu?
    std::list<Lattice::EdgeDescriptor> startEdges = lattice::getTopEdges(
            lattice, langCode, startVertex);
    std::list<Lattice::EdgeDescriptor> headEdges = lattice::getTopEdges(
            lattice, langCode, headVertex);
    std::list<Lattice::EdgeDescriptor> endEdges = lattice::getTopEdges(
            lattice, langCode, endVertex);
    if (startEdges.empty() || headEdges.empty() || endEdges.empty()) {
        return false;
    }
    rulePartitions =  lattice::getEdgesRange(lattice, langCode, startVertex, endVertex);
    lattice::addParseEdges(
            lattice,
            langCode,
            startEdges,
            endEdges,
            this->group,
            headEdges,
            rulePartitions,
            realHead
            );
    std::cerr << "PO JOIN ADD: " << rulePartitions.size() << std::endl;
    return true;
}

bool JoinAction::test(Lattice &lattice, std::string, int,
        RuleTokenSizes &ruleTokenSizes,
        std::list<Lattice::EdgeSequence>&) {
    if ( ( (size_t) lattice.getLastVertex() ) < head ) {
        return false;
    }
    if (ruleTokenSizes[head - 1] == 0) {
        throw PuddleRuleElementNotMatchedException(
                "Group head element " + boost::lexical_cast<std::string>(head)
                + " is empty"
                );
    }
    return true;
}

void JoinAction::init(std::string aGroup, int aStart, int aEnd, int aHead,
        std::string aRuleName) {
    group = aGroup;
    start = aStart;
    end = aEnd;
    head = aHead;
    if ((head > (end+1)) || (head <= start)) {
        if (start == end)
            head = 0;
        else
            head = (head % (end+1)) - start;
    }
    type = "join";
    ruleName = aRuleName;
}


}

}

}
