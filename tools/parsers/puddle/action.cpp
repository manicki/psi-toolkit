
#include "action.hpp"

#include <iostream>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

        bool Action::test(Lattice&, int,
                RuleTokenSizes& ) {
            return true;
        }

        bool Action::apply(Lattice&, int,
                RuleTokenSizes& ) {
            return true;
        }

        void Action::setType(std::string aType)
        {
            type = aType;
        }

    }

}

}
