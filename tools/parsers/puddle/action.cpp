
#include "action.hpp"

#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

//bool Action::test(Entities entities, int currentEntity, std::vector<int> matchedTokensSize)
bool Action::test(ParseGraphPtr pg, int currentEntity,
        std::vector<int> matchedTokensSize) {
//    std::cout << "Test dziada" << std::endl;
    return true;
}

//bool Action::apply(Entities &entities, Edges &edges, int currentEntity, std::vector<int> matchedTokensSize)
bool Action::apply(ParseGraphPtr pg, Lattice &lattice, int currentEntity,
        std::vector<int> matchedTokensSize) {
//    std::cout << "Uruchomienie dziada" << std::endl;
    return true;
}

//std::string Action::getType()
//{
//    return type;
//}

void Action::setType(std::string aType)
{
    type = aType;
}

}

}

}
