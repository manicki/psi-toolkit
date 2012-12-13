#ifndef RULESTRUCT_H__
#define RULESTRUCT_H__

#include <string>
#include <vector>

namespace poleng
{

namespace bonsai
{

class RuleStruct {
    private:
        std::string src_sentence;
        std::string trg_sentence;
        std::string alignment;
        std::vector<std::string> rules;

    public:
        RuleStruct();
        RuleStruct(std::string aSrcSentence, std::string aTrgSentence, std::string aAlignment);
        ~RuleStruct();

        std::string getSrcSentence();
        std::string getTrgSentence();
        std::string getAlignment();
        std::vector<std::string> getRules();

        void addRule(std::string &rule);
        void setSrcSentence(std::string &aSentence);
        void setTrgSentence(std::string &aSentence);
        void setAlignment(std::string aAlignment);
};

}

}

#endif

