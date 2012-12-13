#ifndef TRANSITIONINFO_H__
#define TRANSITIONINFO_H__

#include <string>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>

//using namespace std;
//using namespace boost;

typedef boost::tuple<std::string, std::string, int> PosInfo; 

namespace boost
{
    namespace serialization
    {

template<typename Archive>
void serialize(Archive & ar, boost::tuple<std::string, std::string, int> &t, const unsigned int)
{
    ar & boost::serialization::make_nvp("element",t.get<0>());
    ar & boost::serialization::make_nvp("element",t.get<1>());
    ar & boost::serialization::make_nvp("element",t.get<2>());
}
}
}

namespace poleng
{

namespace bonsai
{

class TransitionInfo {
  public:
    TransitionInfo() { orth_ = ""; }
    TransitionInfo(std::string type) : type_(type) { orth_ = ""; }
    
    void setType(std::string type) { type_ = type; }
    void setId(std::string id) { id_ = id; }
    void setLabel(std::string label) { label_ = label; if (orth_ == "") orth_ = label_; }
    void setHead(std::string head) { head_ = head; }
    void setStart(unsigned int start) { start_ = start; }
    void setEnd(unsigned int end) { end_ = end; }
    void setDepth(int depth) { depth_ = depth; };
  
    std::string getType() { return type_; };  
    std::string getId() { return id_; };
    std::string getLabel() { return label_; };
    std::string getHead() { return head_; };
    unsigned int getStart() { return start_; }
    unsigned int getEnd() { return end_; };
    int getDepth() { return depth_; };
    std::string getSymbol() { return (!isLexical()) ? "<" + getLabel() + ">" : getLabel(); }
    
    bool isLexical() { return type_.compare("token") == 0; }
    bool isPhrasal() { return type_.compare("group") == 0; }
    bool isPos() { return type_.compare("pos") == 0; }
  
    void addMorphology(PosInfo);
    std::vector<PosInfo> getPosInfo() { return variants_; }
    std::vector<TransitionInfo> getPosTransitions();
    std::string getMorphologyString();
    std::vector<std::string> getReadableMorphology();
//    void deleteMorphology(string &dBase, string &dMorpho);

    void setDesc(std::vector<int> aDesc);
    void unsetDesc();
//    void setLabelDesc(std::string aDesc);
//    std::string getLabelDesc();
//    std::string label_desc_;
    std::vector<int> desc;
    bool desc_on;
    std::string desc_sep;

    template<typename Archive>
        void serialize(Archive & ar, const unsigned int)
        {
            ar & type_;
            ar & label_;
            ar & id_;
            ar & head_;
            ar & start_;
            ar & end_;
            ar & depth_;
            ar & variants_;
            ar & orth_;
        }

    //LM: z powodu wyciagarki fraz - nazwa reguly potrzebna do normalizacji?
    void setRuleName(std::string rulename) { rulename_ = rulename; }
    std::string getRuleName() { return rulename_; }

  protected:
    std::string type_;
    std::string label_;
    std::string id_;
    std::string head_;
    unsigned int start_;
    unsigned int end_;
    int depth_;

    std::string orth_;

    //LM: z powodu wyciagarki fraz - nazwa reguly potrzebna do normalizacji?
    std::string rulename_;

  public:
    std::vector<PosInfo> variants_;              
};

typedef std::vector<TransitionInfo*> Edges;

}

}

#endif
