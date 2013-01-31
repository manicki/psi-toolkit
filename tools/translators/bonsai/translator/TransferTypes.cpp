#include <TransferTypes.hpp>
#include <iostream>

using namespace poleng::bonsai;

/**************************** Symbol *****************************/

Symbol::Symbol( Labels labels_, Range range_, bool nt_ ) : labels(labels_), range(range_), nt(nt_) {};
Symbol::Symbol( Labels labels_, Range range_) : labels(labels_), range(range_), nt(false) {};

Symbol::Symbol( Label label_, Range range_, bool nt_ ) : range(range_), nt(nt_) {
    labels.push_back(label_);
};

Symbol::Symbol( Label label_, Range range_ ) : range(range_), nt(false) {
    labels.push_back(label_);
};
    
Label Symbol::label() const {
    if(labels.size() > 0)
	return labels[0];
    else
	return Label("ign");
};
    
Label Symbol::label(size_t i) const {
    if(labels.size() > i)
	return labels[i];
    else
	return Label("ign");
};

Labels Symbol::labelList() const {
    return labels;
}
    
SymbolIndex Symbol::start() const { return range.first; }
SymbolIndex Symbol::end() const { return range.second; }
bool Symbol::is_nt() const { return nt; }
    
std::string Symbol::str() const {
    std::stringstream ss;
    for(Labels::const_iterator it = labels.begin(); it != labels.end(); it++) {
	if(it != labels.begin())
	    ss << "|";
	ss << *it;
    }
    ss << "(" << start()  << "," << end() << ")";
    return ss.str();	
}

bool Symbol::operator!=(Symbol &s1) {
    return this->str() != s1.str();
}

bool Symbol::operator<(Symbol s1) const {
    int size1 = labels.size();
    int size2 = s1.labels.size();
    
    int csize = (size1 < size2) ? size1 : size2;
    for(int i = 0; i < csize; i++) {
	if(label(i) < s1.label(i))
	    return true;
	else if(label(i) > s1.label(i))
	    return false;
    }
    return false;
}


/**************************** SList *****************************/

void SList::push_back(Symbol s) {    
    if( s.is_nt() ) 
	nt.push_back(std::vector<Symbol>::size());
    std::vector<Symbol>::push_back(s);
}

void SList::pop_back() {
    if( back().is_nt() )
	nt.pop_back();	    
    std::vector<Symbol>::pop_back();
}
  
std::string SList::str() {
    std::stringstream sstr;
    for(SList::iterator pIt = SList::begin(); pIt != SList::end(); pIt++) {
	if(pIt != SList::begin()) {
	    sstr << " ";
	}
	if(pIt->is_nt()) {
	    sstr << pIt->str();
	}
	else {
	    sstr << pIt->label();
	}
    }
    return sstr.str();
}

std::string SList::surface(int i) {
    std::stringstream sstr;
    for(SList::iterator pIt = SList::begin(); pIt != SList::end(); pIt++) {
	if(pIt != SList::begin()) {
	    sstr << " ";
	}
	sstr << pIt->label(i);
    }
    return sstr.str();
}

int SList::nt_index(size_t i) {
    if(i < nt.size())
	return nt[i];
    else 
	return -1;
}

int SList::nt_size() {
    return nt.size();
}


