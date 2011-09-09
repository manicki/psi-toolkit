#ifndef ACTION_H__
#define ACTION_H__

#include "puddle_types.hpp"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "puddle_util.hpp"
#include "lattice_wrapper.hpp"

namespace poleng
{

    namespace bonsai
    {

        namespace puddle
        {

            class Action
            {
                public:
                    Action() { type = "abstract"; verbose = false; }
                    virtual ~Action() {}
                    virtual bool test(Lattice&, int, RuleTokenSizes&);
                    virtual bool apply(Lattice&, int, RuleTokenSizes&);
                    virtual std::string getType() { return type; }
                    virtual void setType(std::string aType);

                    virtual void setVerbose() { verbose = true; }

                private:
                    std::string type;
                    bool verbose;
            };

            typedef boost::shared_ptr<Action> ActionPtr;
            typedef std::vector<ActionPtr> Actions;
            typedef boost::shared_ptr<Actions> ActionsPtr;

        }

    }

}

#endif
