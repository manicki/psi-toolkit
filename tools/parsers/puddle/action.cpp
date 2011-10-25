
#include "action.hpp"

#include <iostream>

namespace poleng
{

    namespace bonsai
    {
        namespace puddle
        {

            bool Action::test(Lattice&, int,
                    RuleTokenSizes&, std::list<Lattice::EdgeSequence>&) {
                return true;
            }

            bool Action::apply(Lattice&, int,
                    RuleTokenSizes&, std::list<Lattice::EdgeSequence>&) {
                return true;
            }

            void Action::setType(std::string aType)
            {
                type = aType;
            }

        }

    }

}
