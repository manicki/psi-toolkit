#include "action.hpp"

#include <iostream>

namespace poleng {

    namespace bonsai {
        namespace puddle {

            bool Action::test(Lattice&, std::string, int,
                    RuleTokenSizes&, std::list<Lattice::EdgeSequence>&) {
                return true;
            }

            bool Action::apply(Lattice&, std::string, int,
                    RuleTokenSizes&, std::list<Lattice::EdgeSequence>&) {
                return true;
            }

            void Action::setType(std::string aType) {
                type = aType;
            }

        }

    }

}
