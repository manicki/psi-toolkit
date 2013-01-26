#include "RuleSymbolNumberMap.hpp"

namespace poleng
{

namespace bonsai
{

using namespace rules;

RuleSymbol::RuleSymbol() : full_symbol(std::string()), index(-1) {}

RuleSymbol::RuleSymbol(std::string symbol_) : full_symbol(symbol_), index(-1) {
    boost::algorithm::split(labels, full_symbol, boost::is_any_of("|"));
    
    RegExp re("<[^<>]+>\\[(\\d+)\\]");
    if(labels.size() > 0) {
        int tempIndex;
        if(RegExp::PartialMatch(labels[0], re, &tempIndex))
            index = tempIndex;
    }
}

RuleSymbol::RuleSymbol(std::vector<std::string> labels_) : full_symbol(std::string()), labels(labels_), index(-1) {
    for(std::vector<std::string>::iterator it = labels.begin(); it != labels.end(); it++) {
        full_symbol.append(*it);
        if(it != labels.end()-1)
          full_symbol.append("|||");
    }
    
    RegExp re("<[^<>]+>\\[(\\d+)\\]");
    if(labels.size() > 0) {
        int tempIndex;
        if(RegExp::PartialMatch(labels[0], re, &tempIndex))
            index = tempIndex;
    }
}

std::string RuleSymbol::label() const {
    if(labels.size() > 0)
        return labels[0];
    return full_symbol;
}

std::string RuleSymbol::label(int i) const {
    if(labels.size() > i)
        return labels[i];
    return full_symbol;
}

std::string RuleSymbol::str() const {
    return full_symbol;
}

int RuleSymbol::src_index() {
    return index;
}

Labels& RuleSymbol::get_labels() {
    return labels;
}

/************** MAP ***********************/

int RuleSymbolNumberMap::size() {
    return bm.size();
}

void RuleSymbolNumberMap::add(int i, RuleSymbol s) {
    bm.insert( RuleSymbolNumber(i, s) );
}

int RuleSymbolNumberMap::get_number_by_string(std::string s) {
    RuleSymbol r(s);
    return get_number_by_symbol(r);
}

int RuleSymbolNumberMap::get_number_by_symbol(RuleSymbol s) {
    if(bm.right.count(s) > 0)
        return bm.right.at(s);
    else
        return -1;
}

RuleSymbol RuleSymbolNumberMap::get_symbol_by_number(int i) {
    if(bm.left.count(i) > 0)
        return bm.left.at(i);
    else
        return RuleSymbol();
}    

std::string RuleSymbolNumberMap::get_label_by_number(int i) {
    if(bm.left.count(i) > 0)
        return bm.left.at(i).label();
    else
        return std::string("ERROR");
}    

std::string RuleSymbolNumberMap::get_label_by_number(int i, int j) {
    if(bm.left.count(i) > 0)
        return bm.left.at(i).label(j);
    else
        return std::string("ERROR");
}    

void RuleSymbolNumberMap::save(std::string filename) {
    std::ofstream ofs(filename.c_str());
    boost::archive::binary_oarchive oa(ofs);

    oa << const_cast<const RuleSymbolBimap&>(bm);    
}

void RuleSymbolNumberMap::open(std::string filename) {
    boost::posix_time::ptime pt_start1 = boost::posix_time::microsec_clock::local_time();
    
    std::ifstream ifs(filename.c_str(), std::ios::binary);
    open(ifs);
    
    boost::posix_time::ptime pt_start2 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration delta1 = pt_start2 - pt_start1;
    
    std::cerr << "Loaded symbol map '" << filename << "' in " << delta1.total_milliseconds() << " ms." << std::endl;
}

}

}

