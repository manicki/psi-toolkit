#ifndef TRANSFERTYPES_H__
#define TRANSFERTYPES_H__

#include <vector>
#include <set>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <boost/shared_ptr.hpp>

namespace poleng
{

namespace bonsai
{

typedef std::string Label;
typedef std::vector<Label> Labels;

typedef short int SymbolIndex;
typedef std::pair<SymbolIndex, SymbolIndex> Range;
typedef std::vector<SymbolIndex> Indexes;
typedef std::map<SymbolIndex, SymbolIndex> IndexMap;

typedef std::vector<float> Floats;

typedef std::pair<int, int> AlignmentPoint;
typedef std::set<AlignmentPoint> Alignment;
typedef boost::shared_ptr<Alignment> AlignmentPtr;


class Symbol {
  protected: 
    Labels labels;
    Range  range;
    bool   nt;
    
  public:
    Symbol(Labels, Range, bool);
    Symbol(Labels, Range);
    Symbol(Label, Range, bool);
    Symbol(Label, Range);
    Symbol() : range(Range(-1,-1)), nt(false) {}
    
    Label label() const;
    Label label(size_t i) const;
    Labels labelList() const;
    
    SymbolIndex start() const;
    SymbolIndex end() const;
    bool is_nt() const;
    
    std::string str() const;
        
    template<typename Archive>
    void serialize(Archive & ar, const unsigned int) {
	ar & labels;
	ar & range;
	ar & nt;
    }	

    bool operator!=(Symbol &);

    bool operator<(Symbol) const; 
};

typedef boost::shared_ptr<Symbol> SymbolPtr;

struct SymbolSorterMap {
    bool operator()(const Symbol &sym1, const Symbol &sym2) const {
        return sym1.str().compare(sym2.str()) < 0;
    }
};

struct SymbolSorterMap2 {
    bool operator()(const Symbol &sym1, const Symbol &sym2) const {
        if(sym1.str() != sym2.str())
	    return sym1.str() < sym2.str();
	else if(sym1.start() != sym2.start())
	    return sym1.start() < sym2.start();
	else
	    return sym1.end() < sym2.end();  
    }
};

//bool operator!=(Symbol& s1, Symbol& s2) {
//    return s1.str() != s2.str();
//}

class SList;
typedef boost::shared_ptr<SList> SListPtr; 

class SList : public std::vector<Symbol> {
  public:
    SList() {}
    void push_back(Symbol s);
    void pop_back();
    std::string str();
    std::string surface(int);
    int nt_index(size_t i);
    int nt_size();
    
  private:
    std::vector<int> nt;

};

}

}

#endif
