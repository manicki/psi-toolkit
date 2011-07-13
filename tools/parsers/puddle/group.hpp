#ifndef GROUP_H__
#define GROUP_H__

#include <string>
#include "entity.hpp"

#define ACTION_GROUP    0
#define ACTION_JOIN     1
#define ACTION_ATTACH   2


namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Group : public Entity
{
    public:
        Group();
        Group(Entities &entities, int startIndex, int endIndex, int aHeadIndex, std::string aGroupType, std::string aRuleName, int action_type = ACTION_GROUP);
        ~Group();

        void setId(std::string aId);
        std::string getId();
        void setGroupType(std::string aGroupType);
        std::string getGroupType();
        void setHeadIndex(int aHeadIndex);
        int getHeadIndex();
        void setRuleName(std::string aRuleName);
        std::string getRuleName();

        void setCompiled(std::string aCompiledGroup);
        std::string getCompiled();
        void updateCompiled();

        Entity* getHeadToken();

        static int groupId;

        std::string getType();

        std::string toXml();
        std::string getHead();

        std::string getReadable();

        Entities children;

//        bool getUnify();
//        void setUnify();
//        void reunify();
//
//        void setUnifyAttributes(std::vector<int> aUnifyAttributes);
//        std::vector<int> getUnifyAttributes();
//        void setUnifyTokens(std::vector<int> aUnifyTokens);
//        std::vector<int> getUnifyTokens();

    private:
        std::string type;

        std::string compiledGroup;

        std::string id;
        std::string groupType; //e.g. NP, VP, etc.
        int headIndex;
        std::string ruleName;

//        bool unify;
//        std::vector<int> unifyAttributes;
//        std::vector<int> unifyTokens;

};

}

}

}

#endif
